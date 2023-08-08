#include "Arduino.h"
#include "esp_camera.h"
#include "ESPAsyncWebServer.h"
#include <WiFi.h>
#include "index.h"
#include "soc/soc.h" //disable brownour problems
#include "soc/rtc_cntl_reg.h"  //disable brownour problems
#include <SPIFFS.h>
#include <EEPROM.h>
#include "FS.h"       // SD Card ESP32
#include "SD_MMC.h"   // SD Card ESP32
#include "ESP32_MailClient.h"
#include <AsyncTCP.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#define EEPROM_SIZE 512

#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#include "fd_forward.h"
#include "fr_forward.h"
#include "fr_flash.h"

String emailSenderAccount;
String emailSenderPassword;
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32-CAM Motion Captured"
String emailRecipient;

// The Email Sending data object contains config and data to send
SMTPData smtpData;

TaskHandle_t Task1;
// Callback function to get the Email sending status
void sendCallback(SendStatus info);

//const char* ssid = "BiliKaWifiMo"; //remember change your SSID 
//const char* password = "abcd12345!";// remember change your SSID PW

const char* ssid = "Guest"; //remember change your SSID 
const char* password = "Ted-Eva123456!";// remember change your SSID PW
boolean Login = false;
boolean SDcardAvailable = false;
boolean Auto = false;
const char* spiffs_status;
const char* camera_status;
String list_name;

int image_captured = 0;
int failed_captured = 0;
int total_image= 0;
String folder_name;
char dirname[64]; // 19
char *text_file = (char*)malloc(19+ sizeof(folder_name));
char bat_file[64];

String slice_string;
String trim_files;
char serverlog_file[64];
char spiffs_serverlog_file[64];
char spiffs_folder_path[64];

boolean is_on_Motion;
boolean is_on_Timelapse;
boolean Serial_available = false;
bool err_captured;
//primary ip address
IPAddress local_IP(10, 10, 10, 26);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
String dataMessage;
char logmessage[100];
typedef struct {
        camera_fb_t * fb;
        size_t index;
} camera_frame_t;

#define PART_BOUNDARY "123456789000000000000987654321"
static const char* STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* STREAM_PART = "Content-Type: %s\r\nContent-Length: %u\r\n\r\n";

static const char * JPG_CONTENT_TYPE = "image/jpeg";
static const char * BMP_CONTENT_TYPE = "image/x-windows-bmp";

class AsyncBufferResponse: public AsyncAbstractResponse {
    private:
        uint8_t * _buf;
        size_t _len;
        size_t _index;
    public:
        AsyncBufferResponse(uint8_t * buf, size_t len, const char * contentType){
            _buf = buf;
            _len = len;
            _callback = nullptr;
            _code = 200;
            _contentLength = _len;
            _contentType = contentType;
            _index = 0;
        }
        ~AsyncBufferResponse(){
            if(_buf != nullptr){
                free(_buf);
            }
        }
        bool _sourceValid() const { return _buf != nullptr; }
        virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override{
            size_t ret = _content(buf, maxLen, _index);
            if(ret != RESPONSE_TRY_AGAIN){
                _index += ret;
            }
            return ret;
        }
        size_t _content(uint8_t *buffer, size_t maxLen, size_t index){
            memcpy(buffer, _buf+index, maxLen);
            if((index+maxLen) == _len){
                free(_buf);
                _buf = nullptr;
            }
            return maxLen;
        }
};

class AsyncFrameResponse: public AsyncAbstractResponse {
    private:
        camera_fb_t * fb;
        size_t _index;
    public:
        AsyncFrameResponse(camera_fb_t * frame, const char * contentType){
            _callback = nullptr;
            _code = 200;
            _contentLength = frame->len;
            _contentType = contentType;
            _index = 0;
            fb = frame;
        }
        ~AsyncFrameResponse(){
            if(fb != nullptr){
                esp_camera_fb_return(fb);
            }
        }
        bool _sourceValid() const { return fb != nullptr; }
        virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override{
            size_t ret = _content(buf, maxLen, _index);
            if(ret != RESPONSE_TRY_AGAIN){
                _index += ret;
            }
            return ret;
        }
        size_t _content(uint8_t *buffer, size_t maxLen, size_t index){
            memcpy(buffer, fb->buf+index, maxLen);
            if((index+maxLen) == fb->len){
                esp_camera_fb_return(fb);
                fb = nullptr;
            }
            return maxLen;
        }
};

class AsyncJpegStreamResponse: public AsyncAbstractResponse {
    private:
        camera_frame_t _frame;
        size_t _index;
        size_t _jpg_buf_len;
        uint8_t * _jpg_buf;
        uint64_t lastAsyncRequest;
    public:
        AsyncJpegStreamResponse(){
            _callback = nullptr;
            _code = 200;
            _contentLength = 0;
            _contentType = STREAM_CONTENT_TYPE;
            _sendContentLength = false;
            _chunked = true;
            _index = 0;
            _jpg_buf_len = 0;
            _jpg_buf = NULL;
            lastAsyncRequest = 0;
            memset(&_frame, 0, sizeof(camera_frame_t));
        }
        ~AsyncJpegStreamResponse(){
            if(_frame.fb){
                if(_frame.fb->format != PIXFORMAT_JPEG){
                    free(_jpg_buf);
                }
                esp_camera_fb_return(_frame.fb);
            }
        }
        bool _sourceValid() const {
            return true;
        }
        virtual size_t _fillBuffer(uint8_t *buf, size_t maxLen) override {
            size_t ret = _content(buf, maxLen, _index);
            if(ret != RESPONSE_TRY_AGAIN){
                _index += ret;
            }
            return ret;
        }
        size_t _content(uint8_t *buffer, size_t maxLen, size_t index){
            if(!_frame.fb || _frame.index == _jpg_buf_len){
                if(index && _frame.fb){
                    uint64_t end = (uint64_t)micros();
                    int fp = (end - lastAsyncRequest) / 1000;
                    log_printf("Size: %uKB, Time: %ums (%.1ffps)\n", _jpg_buf_len/1024, fp);
                    lastAsyncRequest = end;
                    if(_frame.fb->format != PIXFORMAT_JPEG){
                        free(_jpg_buf);
                    }
                    esp_camera_fb_return(_frame.fb);
                    _frame.fb = NULL;
                    _jpg_buf_len = 0;
                    _jpg_buf = NULL;
                }
                if(maxLen < (strlen(STREAM_BOUNDARY) + strlen(STREAM_PART) + strlen(JPG_CONTENT_TYPE) + 8)){
                    //log_w("Not enough space for headers");
                    return RESPONSE_TRY_AGAIN;
                }
                //get frame
                _frame.index = 0;

                _frame.fb = esp_camera_fb_get();
                if (_frame.fb == NULL) {
                    log_e("Camera frame failed");
                    return 0;
                }

                if(_frame.fb->format != PIXFORMAT_JPEG){
                    unsigned long st = millis();
                    bool jpeg_converted = frame2jpg(_frame.fb, 80, &_jpg_buf, &_jpg_buf_len);
                    if(!jpeg_converted){
                        log_e("JPEG compression failed");
                        esp_camera_fb_return(_frame.fb);
                        _frame.fb = NULL;
                        _jpg_buf_len = 0;
                        _jpg_buf = NULL;
                        return 0;
                    }
                    log_i("JPEG: %lums, %uB", millis() - st, _jpg_buf_len);
                } else {
                    _jpg_buf_len = _frame.fb->len;
                    _jpg_buf = _frame.fb->buf;
                }

                //send boundary
                size_t blen = 0;
                if(index){
                    blen = strlen(STREAM_BOUNDARY);
                    memcpy(buffer, STREAM_BOUNDARY, blen);
                    buffer += blen;
                }
                //send header
                size_t hlen = sprintf((char *)buffer, STREAM_PART, JPG_CONTENT_TYPE, _jpg_buf_len);
                buffer += hlen;
                //send frame
                hlen = maxLen - hlen - blen;
                if(hlen > _jpg_buf_len){
                    maxLen -= hlen - _jpg_buf_len;
                    hlen = _jpg_buf_len;
                }
                memcpy(buffer, _jpg_buf, hlen);
                _frame.index += hlen;
                return maxLen;
            }

            size_t available = _jpg_buf_len - _frame.index;
            if(maxLen > available){
                maxLen = available;
            }
            memcpy(buffer, _jpg_buf+_frame.index, maxLen);
            _frame.index += maxLen;

            return maxLen;
        }
};

void sendJpg(AsyncWebServerRequest *request){
    camera_fb_t * fb = esp_camera_fb_get();
    if (fb == NULL) {
        log_e("Camera frame failed");
        request->send(501);
        return;
    }

    if(fb->format == PIXFORMAT_JPEG){
        AsyncFrameResponse * response = new AsyncFrameResponse(fb, JPG_CONTENT_TYPE);
        if (response == NULL) {
            log_e("Response alloc failed");
            request->send(501);
            return;
        }
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
        return;
    }

    size_t jpg_buf_len = 0;
    uint8_t * jpg_buf = NULL;
    unsigned long st = millis();
    bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
    esp_camera_fb_return(fb);
    if(!jpeg_converted){
        log_e("JPEG compression failed: %lu", millis());
        request->send(501);
        return;
    }
    log_i("JPEG: %lums, %uB", millis() - st, jpg_buf_len);

    AsyncBufferResponse * response = new AsyncBufferResponse(jpg_buf, jpg_buf_len, JPG_CONTENT_TYPE);
    if (response == NULL) {
        log_e("Response alloc failed");
        request->send(501);
        return;
    }
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}


void streamJpg(AsyncWebServerRequest *request){
    AsyncJpegStreamResponse *response = new AsyncJpegStreamResponse();
    if(!response){
        request->send(501);
        return;
    }

    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void getCameraStatus(AsyncWebServerRequest *request){
    static char json_response[1024];

    sensor_t * s = esp_camera_sensor_get();
    if(s == NULL){
        request->send(501);
        return;
    }
    char * p = json_response;
    *p++ = '{';

    p+=sprintf(p, "\"framesize\":%u,", s->status.framesize);
    p+=sprintf(p, "\"quality\":%u,", s->status.quality);
    p+=sprintf(p, "\"brightness\":%d,", s->status.brightness);
    p+=sprintf(p, "\"contrast\":%d,", s->status.contrast);
    p+=sprintf(p, "\"saturation\":%d,", s->status.saturation);
    p+=sprintf(p, "\"sharpness\":%d,", s->status.sharpness);
    p+=sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
    p+=sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
    p+=sprintf(p, "\"awb\":%u,", s->status.awb);
    p+=sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
    p+=sprintf(p, "\"aec\":%u,", s->status.aec);
    p+=sprintf(p, "\"aec2\":%u,", s->status.aec2);
    p+=sprintf(p, "\"denoise\":%u,", s->status.denoise);
    p+=sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
    p+=sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
    p+=sprintf(p, "\"agc\":%u,", s->status.agc);
    p+=sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
    p+=sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
    p+=sprintf(p, "\"bpc\":%u,", s->status.bpc);
    p+=sprintf(p, "\"wpc\":%u,", s->status.wpc);
    p+=sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
    p+=sprintf(p, "\"lenc\":%u,", s->status.lenc);
    p+=sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
    p+=sprintf(p, "\"vflip\":%u,", s->status.vflip);
    p+=sprintf(p, "\"dcw\":%u,", s->status.dcw);
    p+=sprintf(p, "\"colorbar\":%u", s->status.colorbar);
    *p++ = '}';
    *p++ = 0;

    AsyncWebServerResponse * response = request->beginResponse(200, "application/json", json_response);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

void setCameraVar(AsyncWebServerRequest *request){
    if(!request->hasArg("var") || !request->hasArg("val")){
        request->send(404);
        return;
    }
    String var = request->arg("var");
    const char * variable = var.c_str();
    int val = atoi(request->arg("val").c_str());

    sensor_t * s = esp_camera_sensor_get();
    if(s == NULL){
        request->send(501);
        return;
    }


    int res = 0;
    if(!strcmp(variable, "framesize")) res = s->set_framesize(s, (framesize_t)val);
    else if(!strcmp(variable, "quality")) res = s->set_quality(s, val);
    else if(!strcmp(variable, "contrast")) res = s->set_contrast(s, val);
    else if(!strcmp(variable, "brightness")) res = s->set_brightness(s, val);
    else if(!strcmp(variable, "saturation")) res = s->set_saturation(s, val);
    else if(!strcmp(variable, "sharpness")) res = s->set_sharpness(s, val);
    else if(!strcmp(variable, "gainceiling")) res = s->set_gainceiling(s, (gainceiling_t)val);
    else if(!strcmp(variable, "colorbar")) res = s->set_colorbar(s, val);
    else if(!strcmp(variable, "awb")) res = s->set_whitebal(s, val);
    else if(!strcmp(variable, "agc")) res = s->set_gain_ctrl(s, val);
    else if(!strcmp(variable, "aec")) res = s->set_exposure_ctrl(s, val);
    else if(!strcmp(variable, "hmirror")) res = s->set_hmirror(s, val);
    else if(!strcmp(variable, "vflip")) res = s->set_vflip(s, val);
    else if(!strcmp(variable, "awb_gain")) res = s->set_awb_gain(s, val);
    else if(!strcmp(variable, "agc_gain")) res = s->set_agc_gain(s, val);
    else if(!strcmp(variable, "aec_value")) res = s->set_aec_value(s, val);
    else if(!strcmp(variable, "aec2")) res = s->set_aec2(s, val);
    else if(!strcmp(variable, "denoise")) res = s->set_denoise(s, val);
    else if(!strcmp(variable, "dcw")) res = s->set_dcw(s, val);
    else if(!strcmp(variable, "bpc")) res = s->set_bpc(s, val);
    else if(!strcmp(variable, "wpc")) res = s->set_wpc(s, val);
    else if(!strcmp(variable, "raw_gma")) res = s->set_raw_gma(s, val);
    else if(!strcmp(variable, "lenc")) res = s->set_lenc(s, val);
    else if(!strcmp(variable, "special_effect")) res = s->set_special_effect(s, val);
    else if(!strcmp(variable, "wb_mode")) res = s->set_wb_mode(s, val);
    else if(!strcmp(variable, "ae_level")) res = s->set_ae_level(s, val);

    else {
        log_e("unknown setting %s", var.c_str());
        request->send(404);
        return;
    }
    log_d("Got setting %s with value %d. Res: %d", var.c_str(), val, res);

    AsyncWebServerResponse * response = request->beginResponse(200);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}

AsyncWebServer server(80);

// Photo File Name to save in 
#define SERVER_PIC "/captured.jpg"
String FILE_PHOTO;
camera_fb_t * captured = NULL;
void capture(AsyncWebServerRequest * request){
  camera_fb_t * fb = esp_camera_fb_get();
  bool ok = 0;
  String FILE_NAME = randomName();
  FILE_PHOTO = "/"+FILE_NAME+".jpg";
  do {
    if(failed_captured == 3){
      err_captured = true;
      Serial.println("Failed to captured image!");
      break;
    }
    Serial.println("Taking a photo...");
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }
    captured = fb;
    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_NAME);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
    File s_pic = SPIFFS.open("/captured.jpg", FILE_WRITE);
    // Insert the data in the photo file
    if (!file){Serial.println("Failed to open file in writing mode");}
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      s_pic.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");

      Serial.print(SERVER_PIC);
      Serial.print(" - Size: ");
      Serial.print(s_pic.size());
      Serial.println(" bytes");
    }
    file.close();  s_pic.close();
    esp_camera_fb_return(fb);
    ok = checkPhoto(SPIFFS);
    failed_captured ++;
  } while ( !ok );
  failed_captured = 0;
  if(err_captured){
    request->send(200 , "text/html", "false");
    sprintf(logmessage,"Error: Failed to capture image due to insufficient storage");
    err_captured = false;
  }
  else{
    request->send(200 , "text/html", "true");
    sprintf(logmessage,"Capture has been taken! Capture file name: %s",FILE_PHOTO.c_str());
  }
  if(SDcardAvailable)
    writeLog(SD_MMC, serverlog_file, logmessage);
  else
    writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
  String read_autosave_state = read_String(20);
  String read_auto_delete = read_String(45);
  if(read_autosave_state == "ON"){
      Serial.println("SAVE");
      save_to_SDCard();
      if(read_auto_delete == "ON")
        SPIFFS.remove(FILE_PHOTO);
  }
}

void motion_capture(){
  camera_fb_t * fb = NULL; // pointer
  bool ok = 0;
  String FILE_NAME = randomName();
  FILE_PHOTO = "/"+FILE_NAME+".jpg";
  do {
    if(failed_captured == 3){
      err_captured = true;
      Serial.println("Failed to captured image!");
      break;
    }
    Serial.println("Taking a photo...");
    digitalWrite(2, HIGH);
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }
    Serial.printf("Picture file name: %s\n", FILE_NAME);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
    if (!file){Serial.println("Failed to open file in writing mode");}
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    file.close();
    esp_camera_fb_return(fb);
    ok = checkPhoto(SPIFFS);
    failed_captured++;
  } while ( !ok );
   failed_captured = 0;
    if(err_captured){
     sprintf(logmessage,"Error: Failed to capture image due to insufficient storage");
     err_captured = false;
   }
   else
   sprintf(logmessage,"Motion capture has been triggered! Capture file name: %s",FILE_PHOTO.c_str());
   writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
}

// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );
}

boolean abort_sending = true;
void sendPhoto( void ) {
  // Preparing email
  Serial.println("Sending email...");
  sprintf(logmessage,"Sending all image to %s",emailRecipient.c_str());
  if(SDcardAvailable)
    writeLog(SD_MMC, serverlog_file, logmessage);
  else
    writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
  // Set the SMTP Server Email host, port, account and password
  //smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword); 
  // Set the sender name and Email
  smtpData.setSender("ESP32-CAM", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);   
  // Set the email message in HTML format
  smtpData.setMessage("<h3 style='color:cyan;'>All Photo captured with ESP32-CAM has been sent in this email view them!.</h3>", true);
  // Set the email message in text format
  //smtpData.setMessage("Photo captured with ESP32-CAM and attached in this email.", false);
  // Add recipients, can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient(emailRecipient2);
  File root = SPIFFS.open("/"); 
  File file = root.openNextFile();
  while(file){
    slice_string = file.name();
    if(slice_string.substring(19) != ".txt" && slice_string != "/gmail_account.txt")
      smtpData.addAttachFile(file.name(), "image/jpg");     
    file = root.openNextFile(); delay(1);
  }
  // Set the storage type to attach files in your email (SPIFFS) 
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  smtpData.setSendCallback(sendCallback);
  // Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData)){
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    sprintf(logmessage,"Error sensing Email to %s, Reason %s: ",emailRecipient.c_str(),MailClient.smtpErrorReason().c_str());
    if(SDcardAvailable)
      writeLog(SD_MMC, serverlog_file, logmessage);
    else
      writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
    abort_sending == false;
  }
  smtpData.clearAttachment();
  sprintf(logmessage,"Email successfully sent!");
  if(SDcardAvailable)
      writeLog(SD_MMC, serverlog_file, logmessage);
  else
      writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
      
  String delete_state = read_String(45);  
  if(delete_state == "ON" && abort_sending == true)
    clear_spiffs_files(SPIFFS); 
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  //Print the current status
  Serial.println(msg.info());
}

boolean enable_motion_mode;
static void IRAM_ATTR detectsMovement(void * arg) {
  Serial.println("Motion Detected");
  enable_motion_mode = true;
}
 
void enableInterrupt(){
  esp_err_t err = gpio_isr_handler_add(GPIO_NUM_14, &detectsMovement, (void *) 1);
  if (err != ESP_OK) {
    Serial.printf("handler add failed with error 0x%x \r\n", err);
   }
 
  err = gpio_set_intr_type(GPIO_NUM_14, GPIO_INTR_POSEDGE);
  if (err != ESP_OK) {
    Serial.printf("set intr type failed with error 0x%x \r\n", err);
  }
}

void notifyIPAddress(IPAddress current_IPaddress){
  String comp1 = String(current_IPaddress[0])+"."+String(current_IPaddress[1])+"."+String(current_IPaddress[2])+"."+String(current_IPaddress[3]);
  String comp2 = String(EEPROM.read(259))+"."+String(EEPROM.read(260))+"."+String(EEPROM.read(261))+"."+String(EEPROM.read(262));
  if(comp1 != comp2){
      Serial.println("Not Matched");
      Serial.println("Sending email...");
      smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
      smtpData.setSender("ESP32-CAM NEW IP Address", emailSenderAccount);
      smtpData.setPriority("High");
      smtpData.setSubject("NEW IP ADDRESS");   
      smtpData.setMessage("<h3 style='color: cyan;'>New IP Address has been set to the ESP32 Device! use this IP to access to the webpages. <br></h3><h3 style='color:red;'>IP Address: "+comp1+"</h3>", true);
      smtpData.addRecipient(emailRecipient);
      smtpData.setSendCallback(sendCallback);
      if (!MailClient.sendMail(smtpData)){
        Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
      }
      
      EEPROM.write(259,current_IPaddress[0]);
      EEPROM.write(260,current_IPaddress[1]);
      EEPROM.write(261,current_IPaddress[2]);
      EEPROM.write(262,current_IPaddress[3]);    
      EEPROM.commit(); 
  }
  Serial.println(comp1);
  Serial.println(comp2);
}
String read_motion_state;
String read_autosend_state;
String read_timelapse_state;
long capture_interval;
String temp_log;
boolean offline_mode = false;

void setup(){        
    // Turn-off the 'brownout detector'
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    pinMode(33, OUTPUT);
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;   
    config.frame_size = FRAMESIZE_VGA; 
    //jpeg_quality set 10-63 
    config.jpeg_quality = 12; //10-63 lower number means higher quality
    // fb_count set 2 for jpg
    config.fb_count = 2;
    ledcSetup(5, 50, 16);
    ledcAttachPin(3, 5);
    ledcSetup(6, 50, 16);
    ledcAttachPin(1, 6);
    //Serial_available = false;
    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      camera_status = "Unavailable";
      //return;
      ESP.restart();
    }else{ camera_status = "OK"; }
    
    EEPROM.begin(EEPROM_SIZE);
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      spiffs_status = "Not Mounted";
    }
    else {
      delay(500);
      Serial.println("SPIFFS mounted successfully");
      spiffs_status = "Mounted";
    }
    /*
     * If the ESP Failed to start several times
     * the EEPROM will reset to default configuration
     * 
     */
    int EEPROM_init_failed_times = EEPROM.read(263);
    if(EEPROM_init_failed_times >= EEPROM.read(250)){
       EEPROM_Reset();
    }
    else{
      EEPROM.write(263, EEPROM_init_failed_times+1);
      EEPROM.commit();
    }
    Serial.print("Attempt Counter: ");
    Serial.println(EEPROM.read(263));
    
    //3. Wifi connectd
    WiFi.begin(ssid, password);
    Serial.println("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {
      //delay(500); Serial.print(".");
      if(millis() == 15000){ //15 secs if doesn't connect
          Serial.println("Couldn't not connect to network, Running in offline mode");
        offline_mode = true;
        break; 
      }
    }
    Serial.println();

    Serial.println(EEPROM.read(0));
    Serial.println(EEPROM.read(1));
    Serial.println(EEPROM.read(2));
    Serial.println(EEPROM.read(3));

    Serial.println(read_String(20));
    Serial.println(read_String(25));
    Serial.println(read_String(30));
    Serial.println(read_String(35));
    Serial.println(read_String(40));

    Serial.println(read_String(45));
    Serial.println(read_String(50));
    Serial.println(read_String(100));
    Serial.println(read_String(120));
  
    Serial.println(read_String(130));
    Serial.println(read_String(135));
    Serial.println(read_String(140));
    Serial.println(read_String(150));
    Serial.println(read_String(160));
    
    Serial.println(read_String(190));
    Serial.println(read_String(195));
    Serial.println(read_String(200));
    Serial.println(read_String(210));
    Serial.println(read_String(220));
    Serial.println(read_String(340));
    Serial.println(read_String(369));
    Serial.println(EEPROM.read(240));
    Serial.println(EEPROM.read(241));
    Serial.println(EEPROM.read(242));
    Serial.println(EEPROM.read(243));
    Serial.println(EEPROM.read(244));
    Serial.println(EEPROM.read(245));
    Serial.println(EEPROM.read(246));
    Serial.println(EEPROM.read(247));
    Serial.println(EEPROM.read(248));
    Serial.println(EEPROM.read(249));
    Serial.println(EEPROM.read(250));
    Serial.println(EEPROM.read(255));
    Serial.println(EEPROM.read(256));
    Serial.println(EEPROM.read(257));
    Serial.println(EEPROM.read(258));
    Serial.println(EEPROM.read(259));
    Serial.println(EEPROM.read(260));
    Serial.println(EEPROM.read(261));
    Serial.println(EEPROM.read(262));
    Serial.println(read_String(251));
    Serial.println(EEPROM.read(426));

     /*
      * Setting up the gmail account variable information
      */
      
     gmail_acc();
     
    /*
     *  Enabling mition mode will cause SD card function to malfunction
     *  to prevent the code from getting error I disabled it
     *  until the motion mode is disable.
     * 
     *  - Sending captured images via smtp google
     *  - Automatically sends image when 5 images were captured.
     */
     String read_target_email = read_String(50);
     emailRecipient = read_target_email;
     read_motion_state = read_String(25);
     read_autosend_state = read_String(40);
     if(!offline_mode){
       smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
       if(!MailClient.sendMail(smtpData))
          if(Serial_available)
            Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
     }
     if(read_motion_state == "ON"){
      /*
       *  Use GPIO_NUM_14 for the pir data pin
       */
      enableInterrupt();     
      pinMode(4, OUTPUT);
      digitalWrite(4, LOW); 
      Serial.println("Setting up Motion Mode, Please wait!");
      Serial.println("Server unavailable");     
      writeString(200,"Motion");
      if(read_autosend_state == "ON" && check_spiffs(SPIFFS)){
        sendPhoto();
        abort_sending = true; 
      }     
      if(EEPROM.read(341) == 1){
        EEPROM.write(341,0);
        writeString(130,read_String(190));
        writeString(140,read_String(200));
        writeString(150,read_String(210));
        writeString(160,read_String(220));
        writeString(190,"0");
      }else
        EEPROM.write(341,1);    
      EEPROM.commit();
      is_on_Motion = true;
      create_SPIFFSserverlogs(SPIFFS);
    }
    else{
      if(!SD_MMC.begin()){
          Serial.println("SD Card Mount Failed");
        return;
      }else{ SDcardAvailable = true; 
          Serial.println("SD Card Mounted"); }
          
      uint8_t cardType = SD_MMC.cardType();
      if(cardType == CARD_NONE){
          Serial.println("No SD Card attached");
        return;
      }else{
          Serial.println("SD Card Attached");
      }
    }
    
    if(!offline_mode){
      /*
       * Pre configured network ip address 
      */
      if(read_String(251) == "ON"){
        IPAddress local_IP_STA(EEPROM.read(242), EEPROM.read(243), EEPROM.read(244), EEPROM.read(245));
        IPAddress subnet_STA(EEPROM.read(246), EEPROM.read(247), EEPROM.read(248), EEPROM.read(249));
        IPAddress gateway_STA(EEPROM.read(255), EEPROM.read(256), EEPROM.read(257), EEPROM.read(258));
        if (!WiFi.config(local_IP_STA, gateway_STA, subnet_STA)) {
          Serial.println("STA Failed to configure");
          ESP.restart();
        }
      }
      /*
       * If the IP Address has been changed it will send the new ip address to the gmail account has been set
       */
      notifyIPAddress(WiFi.localIP());
      
      Serial.println("");
      Serial.println("WiFi connected");
      digitalWrite(33, HIGH);
      Serial.print("IP Address: http://");
      Serial.println(WiFi.localIP());
      WiFi.localIP();
      timeClient.begin();
      Init_server_Data();
      POST_server_data();  

      EEPROM.write(263, 0);
      EEPROM.commit();
      Serial.print("Attempt Counter: ");
      Serial.println(EEPROM.read(263));
      server.begin();    
    }
      
    read_timelapse_state = read_String(30);
    folder_name = read_String(100);
    if(read_timelapse_state == "ON"){   
       Serial.println("Setting up Timelapse, Please wait!");
       is_on_Timelapse = true;
       writeString(200,"Timelapse");
       // Creating directory folder
       sprintf(dirname,"/Timelapse/%s", folder_name);
       Serial.print("Creating DIR: ");
       Serial.println(dirname);
       createDir(SD_MMC, dirname);
       
       capture_interval = read_String(120).toInt() * 1000;
       Serial.print("Seconds: ");
       Serial.println(capture_interval);
       if(EEPROM.read(341) == 1){
          EEPROM.write(341,0);
          writeString(135,read_String(195));
          writeString(140,read_String(200));
          writeString(150,read_String(210));
          writeString(160,read_String(220));
          writeString(195,"0");
       }else
          EEPROM.write(341,1);
       EEPROM.commit();
    }  
    if(SDcardAvailable){
      create_SDserverlogs(SD_MMC);
      SpiffsToSD(SD_MMC);
    }  
    if(EEPROM.read(341) == 1 && (!is_on_Timelapse && !is_on_Motion)){
      temp_log = read_String(220);
      if(read_String(200) == "Motion"){
        writeString(130,read_String(190));
        writeString(190,"0");
      }
      else if(read_String(200) == "Timelapse"){
        writeString(135,read_String(195));
        writeString(195,"0");
      }
      writeString(140,read_String(200));
      writeString(150,read_String(210));
      writeString(160,temp_log);
      EEPROM.write(341,0);
      EEPROM.commit();
    }
      //Serial.println(init_FolderTimelapse_table());
      list_spiffs_files(SPIFFS);
      //clear_spiffs_files(SPIFFS);  
      //spiffs_array(SPIFFS);
      Serial.println(read_String(220));
      Serial.println(read_String(160));
}
void gmail_acc(){
    String text_account;
    File file = SPIFFS.open("/gmail_account.txt");
    while(file.available()){
      text_account += file.readString();
    }
    String s_email;
    String s_password;
    int len = 0;
    if(text_account != ""){
      text_account += " ";
      while(true){   //Read until null character    
        if(text_account.charAt(len) == ' ')
          break;
        s_email += text_account.charAt(len);
        len++;
      }
      len++;
      while(true){   //Read until null character  
        if(text_account.charAt(len) == ' ')
          break;  
        s_password += text_account.charAt(len);
        len++;
      }
      emailSenderAccount = s_email;
      emailSenderPassword = s_password;
      Serial.println(s_email);
      Serial.println(s_password);
    }
}
void send_mail(void * parameter){
    sendPhoto();
    vTaskDelay(10);
    vTaskDelete(NULL);
}
String spiffs_photo;
void delete_spiffs(void * parameter){
    SPIFFS.remove(spiffs_photo);
    Serial.print("Removed: ");
    Serial.println(spiffs_photo);
    vTaskDelete(NULL);
}

void SpiffsToSD(fs::FS &fs){
  int count = 0;
  File root = SPIFFS.open("/"); 
  File file = root.openNextFile();
  while(file){
    slice_string = file.name();
    while(true){
      sprintf(spiffs_folder_path, "/SPIFFSLogs/serverlog_%d.txt",count);
      Serial.print("Checking: ");
      Serial.println(spiffs_folder_path);
      if(!fs.exists(spiffs_folder_path))
        break;
      else
        count++;
    }
    if(slice_string.substring(19) == ".txt"){
        File SD_root = fs.open(spiffs_folder_path, FILE_WRITE); 
        Serial.println(spiffs_folder_path);
        while(file.available()){
            SD_root.print(file.readString());
        }
        SD_root.close();
        SPIFFS.remove(file.name());
    }
    file = root.openNextFile();
  }    
}

void writeLog(fs::FS &fs,const char * path, const char * message){
  getTimeStamp();
  File file = fs.open(path, FILE_APPEND);
  if(!offline_mode)
    dataMessage = String(timeStamp) + " " + String(message);
  else
    dataMessage = String(message);
  Serial.println(dataMessage);
  if(!file){ 
      Serial.println("Failed to open file for appending");
  }
  if(file.println(dataMessage.c_str())){ 
      Serial.println("Message appended");
  }
  else{
      Serial.println("Append failed");
  }
  file.close();
}

void create_SDserverlogs(fs::FS &fs){
  File folder = fs.open("/ServerLogs");
  if(!folder){
    Serial.println("Folder doesn't exist! creating...");
    if(fs.mkdir("/ServerLogs")){
       Serial.println("Dir created!");
    }
    else{
      Serial.println("Dir make failed!");
    }
  }  
  else Serial.println("Folder already created");
  int count = 0;
  while(true){
    sprintf(serverlog_file,"/ServerLogs/serverlog_%d.txt",count);
    File file = fs.open(serverlog_file);
    if(!file){Serial.println(serverlog_file); break;}
    else count++;
  }
  writeLog(SD_MMC, serverlog_file, "SERVER LOG FILE");
  trim_files = String(serverlog_file);   
  if(is_on_Motion != false || is_on_Timelapse != false){
      writeString(220,trim_files.substring(12));
  }
}

void create_SPIFFSserverlogs(fs::FS &fs){
  File folder = fs.open("/SPIFFSLogs");
  if(!folder){
    Serial.println("Folder doesn't exist! creating...");
    if(fs.mkdir("/SPIFFSLogs")){ 
        Serial.println("Dir created!"); 
    }
    else{
        Serial.println("Dir make failed!");
    }
  }  
  else Serial.println("Folder already created");
  int count = 0;
  while(true){
    sprintf(spiffs_serverlog_file,"/spiffs_serverlog_%d.txt",count);
    if(!fs.exists(spiffs_serverlog_file)){
      File file = fs.open(spiffs_serverlog_file);
      Serial.println(spiffs_serverlog_file); 
      file.close(); 
      break;
    }
    else count++;
  }
  writeLog(SPIFFS, spiffs_serverlog_file, "SPIFFS LOG FILE");
  trim_files = String(spiffs_serverlog_file);
  if(is_on_Motion != false || is_on_Timelapse != false){
      writeString(220,trim_files.substring(8));
  }
}

void getTimeStamp() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
}

void logSDCard(const char * path, const char * message) {
  getTimeStamp();
  File file = SD_MMC.open(path, FILE_APPEND);
  if(!offline_mode)
    dataMessage = ">>" + String(timeStamp) + " " + String(message);
  else
    dataMessage = ">>" "log: " + String(message);
  Serial.println(dataMessage);
  if(!file){ 
      Serial.println("Failed to open file for appending");
  }
  if(file.print(dataMessage.c_str())){ 
      Serial.println("Message appended");
  }
  else{
      Serial.println("Append failed");
  }
  file.close();
}

//Create a dir in SD card
void createDir(fs::FS &fs, const char * path){
    File root = fs.open(path);
    if(root || folder_name == ""){
        Serial.println("Dir already Exist! creating new dir");
      folder_name = randomName();
      sprintf(dirname,"/Timelapse/%s", folder_name.c_str());
    }
    
    sprintf(text_file, "/Logs/%s.txt",folder_name.c_str());
    sprintf(bat_file, "%s/encode.bat",dirname);
    File logs_dir = fs.open("/Logs");
    File log_text = fs.open(text_file, FILE_WRITE);
    if(logs_dir){
      if(log_text.print("TIMELAPSE LOG EVENTS\n")){
          Serial.println("File Writen");
      }
      else {
          Serial.println("Write failed");
      }
    }
    else{
      if(Serial_available)
        Serial.println("Folder doesn't exist! Creating...");
      if(fs.mkdir("/Logs")){
          Serial.println("Logs Dir created!");
        if(log_text.print("=====TIMELAPSE LOG EVENTS=====\n\n\n")){
            Serial.println("File written");
        }
        else {
            Serial.println("Write failed");
        }
      }
      else{
          Serial.println("Logs failed");
      }
    }
    if(offline_mode) log_text.print(">>Wifi unavailable, Running in offline mode!\n");
    else log_text.print(">>Wifi avaiable! Webserver is ready\n");
    if(fs.mkdir(dirname)){
        Serial.print("Dir created: "); 
        Serial.println(dirname);
    } 
    else{
        Serial.println("mkdir failed");
    }
    
    File bat_format = fs.open(bat_file, FILE_WRITE);
    if(bat_format.print("ffmpeg -r 30 -f image2 -i capture_%%d.jpg -vcodec libx264 -crf 10 -pix_fmt yuv420p timelapse.mp4")){
        Serial.println("Bat File Writen");
    }
    else{
        Serial.println("Bat file Write failed");
    }
    log_text.close();
    bat_format.close();
}

static esp_err_t save_photo_numbered()
{
  ++image_captured;
  if(is_on_Motion != false || is_on_Timelapse != false){
    writeString(195,String(image_captured));
  }
  camera_fb_t *fb = esp_camera_fb_get();
  char *filename = (char*)malloc(24+ sizeof(image_captured)+sizeof(dirname));
  sprintf(filename, "/sdcard%s/capture_%d.jpg",dirname, image_captured);
  FILE *file = fopen(filename, "w");
  if (file != NULL)  {
    size_t err = fwrite(fb->buf, 1, fb->len, file);
    Serial.printf("File saved: %s\n", filename);
    char message[64];
    sprintf(message, "Imaged captured! File name captured_%d.jpg \n",image_captured);
    logSDCard(text_file, message);
  }  else  {
    Serial.println("Could not open file");
  }
  File items = SD_MMC.open("/"+String(dirname)+"/Items.txt", "w");
  if(items.print(image_captured)) 
    Serial.println("Count written");
  else
    Serial.println("Count failed written");
  items.close();
  fclose(file);
  esp_camera_fb_return(fb);
  free(filename);
}

bool check_spiffs(fs::FS &fs){
   uint8_t spiffs_ctr = 0;
   File root = fs.open("/"); 
   File file = root.openNextFile();
   while(file){
    list_name = file.name();
    if(list_name != "/captured.jpg"){
       Serial.println(spiffs_ctr = spiffs_ctr + 1);
    }
    file = root.openNextFile();
  }
  return spiffs_ctr > 0;
}

void wipe_out_spiffs_files(fs::FS &fs){
  File root = fs.open("/"); 
   File file = root.openNextFile();
   Serial.println("----====ERASING SPIFFS DATA====----");
   while(file){
        slice_string = file.name();
        if(slice_string != "/gmail_account.txt"){
          Serial.print("Removing: ");
          Serial.print(file.name());
          Serial.print(" from SPIFFS... ");
          SPIFFS.remove(file.name());
          Serial.println(" DONE! ");
          Serial.println("----============================----");
        }
        file = root.openNextFile(); delay(1);
   }
   delay(1);
}
void clear_spiffs_files(fs::FS &fs){
   File root = fs.open("/"); 
   File file = root.openNextFile();
   Serial.println("----====REMOVING SPIFFS FILES====----");
   while(file){
    slice_string = file.name();
    if(slice_string.substring(19) != ".txt" && slice_string.substring(20) != ".txt" && slice_string != "/gmail_account.txt"){
        Serial.print("Removing: ");
        Serial.print(file.name());
        Serial.print(" from SPIFFS... ");
        SPIFFS.remove(file.name());
        Serial.println(" DONE! ");
        Serial.println("----============================----");
    }
    file = root.openNextFile(); delay(1);
  }
  delay(10);
}
 
void list_spiffs_files(fs::FS &fs){
  float memory_size = 0;
  File root = fs.open("/"); 
  File file = root.openNextFile();
  Serial.println("----====LIST SPIFFS FILES====----");
  while(file){
    slice_string = file.name();
      if(file.size() == 0){
        Serial.print("REMOVING: ");
        Serial.println(file.name());
        SPIFFS.remove(file.name());
      }
      else if(slice_string.substring(19) != ".txt"){
        Serial.print("FILE: ");
        Serial.print(file.name());
        Serial.print("  Size: ");
        Serial.println(file.size());
        memory_size += file.size();
     }
    file = root.openNextFile();
  }
  Serial.print("Total SPIFFS used: ");
  Serial.print(memory_size/10000,2);
  Serial.println(" KB");
  Serial.print("Remaining SPIFFS MB: ");
  Serial.print(float(1000-(memory_size/10000)));
  Serial.println(" KB");
  Serial.println("----=========================----");
}

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 180)
{
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);
  ledcWrite(channel, duty);
}

boolean executed = false;
boolean reverse_tilt = false;
boolean pinModeEnable = false;

int motionctr;
long interval;
long set_timer = 0;

long previousMillis = 0;
unsigned long currentMillis = 0;
boolean sending_in_progress = false;

float endTime(){
  float endTimer = millis()/1000;
  return endTimer;
}
void loop(){
  if(is_on_Motion != false || is_on_Timelapse != false){
    writeString(210,String(endTime()));
  }
  if(!(WiFi.status() != WL_CONNECTED))
      digitalWrite(33, HIGH);

  interval = EEPROM.read(2)*1*1000;
  currentMillis = millis();
  if(read_timelapse_state == "ON"){
    if(currentMillis - previousMillis > capture_interval) {
        previousMillis = currentMillis;   
        save_photo_numbered();
    }
  }
  
  if(read_motion_state == "ON"){
    if(!executed){ //Execute once
      // 1 tilt value
      Serial.println(interval);
      Serial.print("Seconds: ");
      Serial.println(interval/1000);
      ledcAnalogWrite(6,90); 
      EEPROM.write(1,90);
      EEPROM.commit();     
      executed = true;
    }
    if(!sending_in_progress){ //disabling when sending image
      if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;   
            if(!reverse_tilt && (EEPROM.read(1) >= 0 && EEPROM.read(1) <= 44)){
              ledcAnalogWrite(6, 45); 
              EEPROM.write(1, 45);            
            }
            else if(!reverse_tilt && (EEPROM.read(1) >= 45 && EEPROM.read(1) <= 89)){
              ledcAnalogWrite(6,90); 
              EEPROM.write(1,90);            
            }
            else if(!reverse_tilt && (EEPROM.read(1) >= 90 && EEPROM.read(1) <= 134)){
              ledcAnalogWrite(6,135); 
              EEPROM.write(1,135);            
            }
            else if(!reverse_tilt && (EEPROM.read(1) >= 135)){
              ledcAnalogWrite(6,180); 
              EEPROM.write(1,180);            
            }
            
            if(reverse_tilt && (EEPROM.read(1) <= 45)){
              ledcAnalogWrite(6,0); 
              EEPROM.write(1,0);            
            }
            else if(reverse_tilt && (EEPROM.read(1) >= 46 && EEPROM.read(1) <= 90)){
              ledcAnalogWrite(6,45); 
              EEPROM.write(1,45);            
            }
            else if(reverse_tilt && (EEPROM.read(1) >= 91 && EEPROM.read(1) <= 135)){
              ledcAnalogWrite(6,90); 
              EEPROM.write(1,90);            
            }
            else if(reverse_tilt && (EEPROM.read(1) >= 136 && EEPROM.read(1) <= 180)){
              ledcAnalogWrite(6,135); 
              EEPROM.write(1,135);            
            }
  
            if(EEPROM.read(1) == 180)
              reverse_tilt = true;
            if(EEPROM.read(1) == 0)
              reverse_tilt = false;
              
            Serial.print(interval/60000);
            Serial.println(" Minute Passed!");
            EEPROM.commit();
      }
    }
  }
  if(enable_motion_mode){
    digitalWrite(4, HIGH);
     motion_capture();
     delay(500);
     digitalWrite(4, LOW);
     ++image_captured;
     if(is_on_Motion != false || is_on_Timelapse != false){
      writeString(190,String(image_captured));
     }
     enable_motion_mode = false;
  }
  delay(1);
}

const char* param_val = "value";
const char* username = "username";
const char* l_password = "password";
const char* credentials = "credentials";
String inputMessage;
boolean auth_status = true;
void motionMode(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();   
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON")){
         Auto = true; writeString(25, inputMessage.c_str());
      }
      else{
        Auto = false; writeString(25, inputMessage.c_str());
      }
      EEPROM.commit(); delay(1); 
      sprintf(logmessage,"Motion mode set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
   }
   request->send(200 , "text/html", "OK"); //Send web page
}

void autoSave(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();   
      Serial.println(inputMessage);
      writeString(20, inputMessage.c_str());
      EEPROM.commit();  
      sprintf(logmessage,"Autosave set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
   }
   request->send(200 , "text/html", "OK"); //Send web page
   delay(1);
}

boolean saved = false;
void save_to_SDCard(){   
  Serial.printf("Picture file name: %s\n", FILE_PHOTO.c_str());
  char *filename = (char*)malloc(18+ sizeof(FILE_PHOTO));
  sprintf(filename, "/sdcard/Captured%s",FILE_PHOTO.c_str());
  if(Serial_available)
    Serial.println(filename);
  FILE *file = fopen(filename, "w");
  if(!file){
    if(Serial_available)
      Serial.println("Failed to open file in writing mode");
  } 
  else {
    //file.write(captured->buf,captured->len); // payload (image), payload length
    size_t err = fwrite(captured->buf, 1, captured->len, file);
    Serial.printf("Saved file to path: %s\n", filename);
    saved =  true;
  }
  fclose(file);
  free(filename);
  sprintf(logmessage,"Image saved: %s",FILE_PHOTO.c_str());
  if(SDcardAvailable)
    writeLog(SD_MMC, serverlog_file, logmessage);
  else
    writeLog(SPIFFS, spiffs_serverlog_file, logmessage);
}

void setFolder(AsyncWebServerRequest * request){
  if (request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value();   
    Serial.println(inputMessage);
    writeString(100, inputMessage.c_str());
    EEPROM.commit(); delay(1);  
    sprintf(logmessage,"Foldername has been set: %s",inputMessage.c_str());
    if(SDcardAvailable)
        writeLog(SD_MMC, serverlog_file, logmessage);
    else
        writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
  }
}

void timeLapse(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();   
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON")){
         writeString(30, inputMessage.c_str());
      }
      else{
         writeString(30, inputMessage.c_str());
      }
      sprintf(logmessage,"Timelapse set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      EEPROM.commit(); delay(1);  
   }
   request->send(200 , "text/html", "OK"); //Send web page
}
void faceRecognition(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();   
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON"))
         writeString(35, inputMessage.c_str());
      else
         writeString(35, inputMessage.c_str());
      sprintf(logmessage,"Facerecognition set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      EEPROM.commit(); delay(1);  
   }
   request->send(200 , "text/html", "OK"); //Send web page
}
void autosend(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();  
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON"))
         writeString(40, inputMessage.c_str());
      else
         writeString(40, inputMessage.c_str());
      sprintf(logmessage,"Autosend image set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      EEPROM.commit(); delay(1);  
   }
   request->send(200 , "text/html", "OK"); //Send web page
}
void autodelete(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();  
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON"))
         writeString(45, inputMessage.c_str());
      else
         writeString(45, inputMessage.c_str());
      sprintf(logmessage,"Autodelete image set to state: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      EEPROM.commit(); delay(1);  
   }
   request->send(200 , "text/html", "OK"); //Send web page
}

void email_reciepient(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value();  
      Serial.println(inputMessage);
      if(!strcmp(inputMessage.c_str(), "ON"))
         writeString(50, inputMessage.c_str());
      else
         writeString(50, inputMessage.c_str());
      sprintf(logmessage,"Email recipient has been changed! new: %s",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      EEPROM.commit(); delay(1);  
   }
   request->send(200 , "text/html", "OK"); //Send web page
}

void PanUpdate(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      inputMessage = request->getParam(param_val)->value(); 
      Serial.println(inputMessage);  
      ledcAnalogWrite(5,inputMessage.toInt()); 
      EEPROM.write(0, inputMessage.toInt());
      EEPROM.commit(); delay(1);  
      sprintf(logmessage,"Pan Servo turning %s degree",inputMessage.c_str());
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
   }
   request->send(200 , "text/html", "OK"); //Send web page
}

void TiltUpdate(AsyncWebServerRequest * request){
   if (request->hasParam(param_val)) {
      if(!Auto){
        inputMessage = request->getParam(param_val)->value();   
        ledcAnalogWrite(6,inputMessage.toInt()); 
        EEPROM.write(1, inputMessage.toInt());
        EEPROM.commit(); delay(1); 
        sprintf(logmessage,"Tilt Servo turning %s degree",inputMessage.c_str());
        if(SDcardAvailable)
            writeLog(SD_MMC, serverlog_file, logmessage);
        else
            writeLog(SPIFFS, spiffs_serverlog_file, logmessage);  
      }
   }
   request->send(200 , "text/html", "OK"); //Send web page
}

void sendImg(AsyncWebServerRequest * request) {
  sending_in_progress = true;
  Serial.println("Sending email...");
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  Serial.println("############");
  smtpData.setSender("ESP32-CAM", emailSenderAccount);
  smtpData.setPriority("High");
  smtpData.setSubject(emailSubject);
  smtpData.setMessage("<h3 style='color:cyan;'>ESP Cam Photo send successfully in this email.</h3>", true);
  smtpData.addRecipient(emailRecipient);
  if(request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value(); 
    File root = SPIFFS.open(inputMessage); 
    smtpData.addAttachFile(root.name(), "image/jpg");
  }
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  smtpData.setSendCallback(sendCallback);
  if (!MailClient.sendMail(smtpData)){
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    sprintf(logmessage,"Failed to send image, Reason: %s",MailClient.smtpErrorReason().c_str());
    if(SDcardAvailable)
        writeLog(SD_MMC, serverlog_file, logmessage);
    else
        writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
    EEPROM.commit(); delay(1);  
    request->send(501 , "text/html", "Error"); //Send web page
  }
  delay(1);
  smtpData.clearAttachment();
  sprintf(logmessage,"Email successfully sent to: %s",emailRecipient.c_str());
  if(SDcardAvailable)
      writeLog(SD_MMC, serverlog_file, logmessage);
  else
      writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
  request->send(200 , "text/html", "true"); //Send web page
  sending_in_progress = false;
}

void send_all_img(AsyncWebServerRequest * request) {
  sending_in_progress = true;
  Serial.println("Sending email...");
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32-CAM", emailSenderAccount);
  smtpData.setPriority("High");
  smtpData.setSubject(emailSubject);
  smtpData.setMessage("<h3 style='color:cyan;'>All photo stored in ESP Cam has been send.</h3>", true);
  smtpData.addRecipient(emailRecipient);
  File root = SPIFFS.open("/"); 
  File file = root.openNextFile();
  while(file){
    slice_string = file.name();
    if(slice_string.substring(19) != ".txt" && slice_string != "/gmail_account.txt")
      smtpData.addAttachFile(file.name(), "image/jpg");
    file = root.openNextFile();
    delay(10);
  }
  root.close();
  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
  smtpData.setSendCallback(sendCallback);
  if (!MailClient.sendMail(smtpData)){
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
    sprintf(logmessage,"Failed to send all image, Reason: %s",MailClient.smtpErrorReason().c_str());
    if(SDcardAvailable)
        writeLog(SD_MMC, serverlog_file, logmessage);
    else
        writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
    request->send(501 , "text/html", "Error"); //Send web page
    abort_sending = false;
  }
  smtpData.clearAttachment();
  String delete_state = read_String(45);  
  if(delete_state == "ON" && abort_sending == true){
    clear_spiffs_files(SPIFFS); 
    request->send(200 , "text/html", "true_and_deleted");
  }
  else
    request->send(200 , "text/html", "true");
    
  sprintf(logmessage,"Email successfully sent to: %s",emailRecipient.c_str());
  if(SDcardAvailable)
      writeLog(SD_MMC, serverlog_file, logmessage);
  else
      writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
  sending_in_progress = false;
}

void Init_server_Data(){
    server.on("/login", HTTP_GET, [](AsyncWebServerRequest * request) {
      if(Login)  
        request->redirect("/");
      else
        request->send_P(200, "text/html", login_html, res_auth);
    }); 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited main page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else
        request->send_P(200, "text/html", index_html, processor);
      //request->send(SD_MMC, "/Server/index.html", "text/html", false);
    });

    server.on("/record", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited record page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else
        request->send_P(200, "text/html", record_html, processor);
      //request->send(SD_MMC, "/Server/index.html", "text/html", false);
    });
    
    server.on("/motion", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited motion page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else    
        request->send_P(200, "text/html", motion_html, processor);
    });

    server.on("/streaming", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited stream page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else    
        request->send_P(200, "text/html", stream_html, processor);
    });

    server.on("/timelapse", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited timelapse page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else    
        request->send_P(200, "text/html", timelapse_html, processor);
    });
    server.on("/face", HTTP_GET, [](AsyncWebServerRequest * request) {
      sprintf(logmessage,"Visited facerecognition page");
      if(SDcardAvailable)
          writeLog(SD_MMC, serverlog_file, logmessage);
      else
          writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      if(!Login)  
        request->redirect("/login");
      else    
        request->send_P(200, "text/html", face_html);
    });
    server.on("/spiffs_status", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/html", spiffs_status);
    });
    server.on("/camera_status", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/html", camera_status);
    });
    server.on("/delay_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = String(EEPROM.read(2));  //Flash file delay value
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/timer_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = String(EEPROM.read(3));  //Flash file timer value
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/pan_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = String(EEPROM.read(0));  //Flash file timer value
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/tilt_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = String(EEPROM.read(1));  //Flash file timer value
      request->send_P(200, "text/html", convert.c_str());
    });

    server.on("/button_autosave", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = read_String(20);     
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/button_motion", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = read_String(25);    
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/button_timelapse", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = read_String(30);    
      request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/button_facerecognition", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = read_String(35);  ;  
      request->send_P(200, "text/html", convert.c_str());
    });

    server.on("/spiffs_list", HTTP_GET, [](AsyncWebServerRequest * request) {
      String convert = spiffs_array(SPIFFS,"MOTION");  
      request->send_P(200, "text/html", convert.c_str());
    });
    
    server.on("/init_motion_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_motion_data();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_stream_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_stream_data();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_timelapse_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_timelapse_data();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_index_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_index_data();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_spiffsserver_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_SPIFFSServerLog_table();  
       request->send_P(200, "text/html", convert.c_str());
    }); 
    server.on("/init_server_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_ServerLog_table();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_image_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = spiffs_array(SPIFFS);  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_spiffs_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_SPIFFS_table();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_server_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_ServerLog_table();  
       request->send_P(200, "text/html", convert.c_str());
    });
    server.on("/init_spiffsserver_data", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = init_SPIFFSServerLog_table();  
       request->send_P(200, "text/html", convert.c_str());
    });
}
String img_view;
void POST_server_data(){
  server.on("/set_delay_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      const char* DELAY = "value";
      String inputMessage;
      if(request->hasParam(DELAY)) {
        String inputMessage = request->getParam(DELAY)->value();   
        EEPROM.write(2, inputMessage.toInt());
        EEPROM.commit();   
        sprintf(logmessage,"Interval value set to: %s",inputMessage.c_str());
        if(SDcardAvailable)
            writeLog(SD_MMC, serverlog_file, logmessage);
        else
            writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      }
      request->send(200 , "text/html", "OK"); //Send web page
      delay(10); 
    });
    server.on("/set_timer_value", HTTP_GET, [](AsyncWebServerRequest * request) {
      const char* TIMER = "value";
      if(request->hasParam(TIMER)) {
        String inputMessage = request->getParam(TIMER)->value();   
        writeString(120, inputMessage.c_str());
        EEPROM.commit();
        sprintf(logmessage,"Timelapse capture interval value set to: %s",inputMessage.c_str());
        if(SDcardAvailable)
            writeLog(SD_MMC, serverlog_file, logmessage);
        else
            writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
      }
      request->send(200 , "text/html", "OK"); //Send web page
      delay(10); 
    });
    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request) {
        sprintf(logmessage,"Server restarted, Server file logging done!");
        if(SDcardAvailable)
            writeLog(SD_MMC, serverlog_file, logmessage);
        else
            writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
        ESP.restart();
    });

    server.on("/save", HTTP_GET, [](AsyncWebServerRequest * request) {
        save_to_SDCard();
        SPIFFS.remove(FILE_PHOTO);
        if(saved == true){request->send(200 , "text/html", "true"); saved = false;} 
    });
    server.on("/view_img", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* file_img = "value";
        String inputMessage;
        if(request->hasParam(file_img)) {
           inputMessage = request->getParam(file_img)->value();  
           img_view = inputMessage;
           server.on("/display", HTTP_GET, [](AsyncWebServerRequest * request) {
              request->send(SPIFFS, img_view , "image/jpg", false); //Send web page
           });   
           Serial.println(inputMessage);
           request->send(200 , "text/html", inputMessage); //Send web page
        }
        delay(10); 
       // xTaskCreatePinnedToCore(send_mail,"Send Email",10024,NULL,0,NULL,0);
    });
    server.on("/delete_img", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* file_img = "value";
        if(request->hasParam(file_img)) {
           spiffs_photo = request->getParam(file_img)->value();     
           xTaskCreatePinnedToCore(delete_spiffs,"Delete Image",10024,NULL,0,NULL,0);   
           sprintf(logmessage,"Image deleteted: %s",spiffs_photo.c_str());
           if(SDcardAvailable)
              writeLog(SD_MMC, serverlog_file, logmessage);
           else
              writeLog(SPIFFS, spiffs_serverlog_file, logmessage);             
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/timelapse_folder", HTTP_GET, setFolder);
    server.on("/auto_state", HTTP_GET, autosend);
    server.on("/autodelete_state", HTTP_GET, autodelete);
    server.on("/email_address", HTTP_GET, email_reciepient);
    server.on("/send_img", HTTP_GET, sendImg);
    server.on("/send_all", HTTP_GET,send_all_img);

    server.on("/update_capture_counter", HTTP_GET, [](AsyncWebServerRequest * request) {
       String convert = String(image_captured); 
       request->send(200 , "text/html", convert.c_str()); //Send web page
    });
    server.on("/download", HTTP_GET, [](AsyncWebServerRequest * request) {
       request->send(SPIFFS, "/spiffs_serverlog_0.txt", "text/html", true);
    });
    server.on("/panservo", HTTP_GET,PanUpdate);
    server.on("/tiltservo", HTTP_GET,TiltUpdate);
    server.on("/capture", capture);
    server.on("/stream", HTTP_GET, streamJpg);
    server.on("/autosave", HTTP_GET, autoSave);
    server.on("/motion_sensor", HTTP_GET, motionMode);
    server.on("/timelapse_state", HTTP_GET, timeLapse);
    server.on("/facerecognition", HTTP_GET, faceRecognition);
    
    server.on("/SPIFFS_file_download", HTTP_GET, SPIFFS_file_download);
    server.on("/JPG_download", HTTP_GET, JPG_download);
    server.on("/SDServer_download", HTTP_GET, SDServer_download);
    server.on("/SD_SPIFFSServer_download", HTTP_GET, SD_SPIFFSServer_download);

    server.on("/TFolder_Delete", HTTP_GET, TFolder_Delete);
    server.on("/SPIFFS_delete", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* file_name = "value";
        String inputMessage;
        if(request->hasParam(file_name)) {
           inputMessage = request->getParam(file_name)->value();     
           SPIFFS.remove(inputMessage);
           Serial.print("Removed: ");
           Serial.println(inputMessage);
           sprintf(logmessage,"Spiffs txt file deleteted: %s",inputMessage.c_str());
           if(SDcardAvailable)
              writeLog(SD_MMC, serverlog_file, logmessage);
           else
              writeLog(SPIFFS, spiffs_serverlog_file, logmessage);             
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });

    server.on("/SDServer_delete", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* file_name = "value";
        String inputMessage;
        if(request->hasParam(file_name)) {
           inputMessage = request->getParam(file_name)->value();   
           SD_MMC.remove("/ServerLogs/"+inputMessage);
           Serial.print("Removed: ");
           Serial.println(inputMessage);
           sprintf(logmessage,"Serverlogs text file deleteted: %s",inputMessage.c_str());
           if(SDcardAvailable)
              writeLog(SD_MMC, serverlog_file, logmessage);
           else
              writeLog(SPIFFS, spiffs_serverlog_file, logmessage);             
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });

    server.on("/SDSPIFFS_delete", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* file_name = "value";
        String inputMessage;
        if(request->hasParam(file_name)) {
           inputMessage = request->getParam(file_name)->value();   
           SD_MMC.remove("/SPIFFSLogs/"+inputMessage);
           Serial.print("Removed: ");
           Serial.println(inputMessage);
           sprintf(logmessage,"SPIFFSlogs text file deleteted: %s",inputMessage.c_str());
           if(SDcardAvailable)
              writeLog(SD_MMC, serverlog_file, logmessage);
           else
              writeLog(SPIFFS, spiffs_serverlog_file, logmessage);             
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });

    server.on("/auth", HTTP_GET, [](AsyncWebServerRequest * request) {
        String login_user;
        String login_pass;
        String token;
        if (request->hasParam(username)) {
          login_user = request->getParam(username)->value();
          Serial.print(" LOGIN USER: ");
          Serial.println(login_user);
        }   
        if (request->hasParam(l_password)) {
          login_pass = request->getParam(l_password)->value();
          Serial.print(" LOGIN PASS: ");
          Serial.println(login_pass);
        }  
        if(Authentication(login_user,login_pass)){
          Serial.println("Authentication Passed!");
          if (request->hasParam(credentials)) {
            token = request->getParam(credentials)->value();
            Serial.print(" CREDENTIALS: ");
            Serial.println(token);
            EEPROM.write(240,1);
          } 
          else{
            Serial.println(" CREDENTIALS: off");
            EEPROM.write(240,0);
          }
          EEPROM.commit();
          Login = true;
          request->redirect("/");
        }
        else{
          auth_status = false;
          request->redirect("/login");
        }
    });
    server.on("/logout", HTTP_GET, [](AsyncWebServerRequest * request) {
         Login = false;
    });
    server.on("/resetEEPROM", HTTP_GET, [](AsyncWebServerRequest * request) {
      EEPROM_Reset();
      ESP.restart();
    });

    server.on("/resetSPIFFS", HTTP_GET, [](AsyncWebServerRequest * request) {
      wipe_out_spiffs_files(SPIFFS);
      ESP.restart();
    });

    server.on("/attemptTimes", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* attempt = "value";
        String inputMessage;
        if(request->hasParam(attempt)) {
           inputMessage = request->getParam(attempt)->value();   
           Serial.println(inputMessage);
           sprintf(logmessage,"Attempt times changed to %d Times",inputMessage.toInt());
           EEPROM.write(250,inputMessage.toInt());
           EEPROM.commit();
           if(SDcardAvailable)
              writeLog(SD_MMC, serverlog_file, logmessage);
           else
              writeLog(SPIFFS, spiffs_serverlog_file, logmessage);             
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });

    server.on("/ip1", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* ip_add = "value";
        String inputMessage;
        if(request->hasParam(ip_add)) {
          inputMessage = request->getParam(ip_add)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(242,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/ip2", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* ip_add = "value";
        String inputMessage;
        if(request->hasParam(ip_add)) {
          inputMessage = request->getParam(ip_add)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(243,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/ip3", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* ip_add = "value";
        String inputMessage;
        if(request->hasParam(ip_add)) {
          inputMessage = request->getParam(ip_add)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(244,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/ip4", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* ip_add = "value";
        String inputMessage;
        if(request->hasParam(ip_add)) {
          inputMessage = request->getParam(ip_add)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(245,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });

    server.on("/s1", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* subnet_mask = "value";
        String inputMessage;
        if(request->hasParam(subnet_mask)) {
          inputMessage = request->getParam(subnet_mask)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(246,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/s2", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* subnet_mask = "value";
        String inputMessage;
        if(request->hasParam(subnet_mask)) {
          inputMessage = request->getParam(subnet_mask)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(247,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/s3", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* subnet_mask = "value";
        String inputMessage;
        if(request->hasParam(subnet_mask)) {
          inputMessage = request->getParam(subnet_mask)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(248,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/s4", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* subnet_mask = "value";
        String inputMessage;
        if(request->hasParam(subnet_mask)) {
          inputMessage = request->getParam(subnet_mask)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(249,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/g1", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* gateway = "value";
        String inputMessage;
        if(request->hasParam(gateway)) {
          inputMessage = request->getParam(gateway)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(255,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/g2", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* gateway = "value";
        String inputMessage;
        if(request->hasParam(gateway)) {
          inputMessage = request->getParam(gateway)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(256,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/g3", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* gateway = "value";
        String inputMessage;
        if(request->hasParam(gateway)) {
          inputMessage = request->getParam(gateway)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(257,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/g4", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* gateway = "value";
        String inputMessage;
        if(request->hasParam(gateway)) {
          inputMessage = request->getParam(gateway)->value();  
          Serial.println(inputMessage); 
          EEPROM.write(258,inputMessage.toInt());
          EEPROM.commit();
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/networkSTA", HTTP_GET, [](AsyncWebServerRequest * request) {
      const char* networkSTA = "value";
      String inputMessage;
      if (request->hasParam(networkSTA)) {
        inputMessage = request->getParam(networkSTA)->value();  
        Serial.println(inputMessage);
        if(!strcmp(inputMessage.c_str(), "ON"))
           writeString(251, inputMessage.c_str());
        else
           writeString(251, inputMessage.c_str());
        sprintf(logmessage,"NetworkSTA set to state: %s",inputMessage.c_str());
        if(SDcardAvailable)
            writeLog(SD_MMC, serverlog_file, logmessage);
        else
            writeLog(SPIFFS, spiffs_serverlog_file, logmessage); 
        EEPROM.commit(); delay(1);  
      }
      request->send(200 , "text/html", "OK"); //Send web page
    });
    server.on("/sender_account", HTTP_GET, [](AsyncWebServerRequest * request) {
        const char* account_info = "value";
        String inputMessage;
        File file = SPIFFS.open("/gmail_account.txt", FILE_WRITE);
        if(!file)
           Serial.println("Failed to open file");
        else{
          if(request->hasParam(account_info)) {
               inputMessage = request->getParam(account_info)->value();  
               Serial.println(inputMessage); 
               file.print(inputMessage);
               file.close();
          }       
        }
        request->send(200 , "text/html", "OK"); //Send web page
    });
}

String res_auth(const String& var){
  if(var == "Errormsg")
    if(!auth_status)
      return "Invalid username or password";
    else
      return "";
  if(EEPROM.read(240) == 1){
    if(var == "username")
      return read_String(340);
    if(var == "password")
      return read_String(369);
    if(var == "checked")
      return "checked";
  }
  return String();
}

boolean Authentication(String user, String pass){
    String auth_user = read_String(340);
    String auth_pass = read_String(369);
    if((user == auth_user) && (pass == auth_pass)){
      return true;
    }
    else 
      return false;
}

void TFolder_Delete(AsyncWebServerRequest * request) {
  if (request->hasParam(param_val)) {
    inputMessage = "/"+request->getParam(param_val)->value(); 
    Serial.println(inputMessage);
    File root = SD_MMC.open("/Timelapse"+inputMessage);
    File file = root.openNextFile();
    while(file){
      Serial.print(" REMOVING: ");
      Serial.println(file.name());
      SD_MMC.remove(file.name());
      file = root.openNextFile();
    }
    if(SD_MMC.rmdir("/Timelapse"+inputMessage)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
  }
}

void SDServer_download(AsyncWebServerRequest * request) {
  if (request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value(); 
    File download = SD_MMC.open("/ServerLogs/"+inputMessage);
    if(download) {
      Serial.println("SD File Read");
      request->send(SD_MMC, "/ServerLogs/"+inputMessage, "text/html", true);
      download.close();
    }
    else{
     Serial.println("ERROR");
     request->send(200 , "text/html", "error"); //Send web page
    }
  }
}

void SD_SPIFFSServer_download(AsyncWebServerRequest * request) {
  if (request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value(); 
    File download = SD_MMC.open("/SPIFFSLogs/"+inputMessage);
    if(download) {
      Serial.println("SD File Read");
      request->send(SD_MMC, "/SPIFFSLogs/"+inputMessage, "text/html", true);
      download.close();
    }
    else{
     Serial.println("ERROR");
     request->send(200 , "text/html", "error"); //Send web page
    }
  }
}

void JPG_download(AsyncWebServerRequest * request) {
  if (request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value(); 
    File download = SPIFFS.open(inputMessage);
    if(download) {
      Serial.println("OK");
      request->send(SPIFFS, inputMessage, "image/jpeg", true);
      download.close();
    }
    else{
     Serial.println("ERROR");
     request->send(200 , "text/html", "error"); //Send web page
    }
  }
}

void SPIFFS_file_download(AsyncWebServerRequest * request) {
  if (request->hasParam(param_val)) {
    inputMessage = request->getParam(param_val)->value(); 
    File download = SPIFFS.open(inputMessage,"r");
    if(download) {
      Serial.println("OK");
      request->send(SPIFFS, inputMessage, "text/html", true);
      download.close();
    }
    else{
     Serial.println("ERROR");
     request->send(401 , "text/html", "error"); //Send web page
    }
  }
}

void writeString(char address,String data)
{     
  int _size = data.length();
  int i;
  for(i = 0; i<_size; i++){ 
    EEPROM.write(address+i,data[i]);
  }
  EEPROM.write(address+_size,'\0');   //Add termination null character for String Data
  EEPROM.commit();
}

String read_String(char add)
{
  char data[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while(k != '\0' && len < 500){   //Read until null character    
    k = EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

void EEPROM_Reset(){    
    
    for (int i = 0; i < 512; i++) {
     //Serial.print("EEPROM :");
     //Serial.println(EEPROM.read(i));
     EEPROM.write(i,0);
     delay(10);
    }

    // 0 pan value
    // 1 tilt value 
    // 2 delay
    // 3 timelapse captured counter  
    // 40 camera button
    // 20 autosave
    // 25 motion sensor
    // 30 timelapse
    // 35 face recognition
    // 40 autosend
    // 45 autodelete
    // 50-90 target email
    //100-120 foldername
    //120 - 130 capture interval
    //130-total motion captured
    //135-total timelapse captured
    //140 - recent mode
    //150 - end time
    //160 to 190 - log file

    //190-total motion captured
    //195-total timelapse captured
    //200 - recent mode
    //210 - end time
    //220 to 240 - log file

    //240 - credentials
    //241 - log counter
    
    //242 - ip1
    //243 - ip2
    //244 - ip3
    //245 - ip4
    //246 - sub1
    //247 - sub2
    //248 - sub3
    //249 - sub4
    //250 - attempt times
    //251 to 255 STA Status
    //255 - g1
    //256 - g2
    //257 - g3
    //258 - g4
    //259 - previous ip1
    //260 - previous ip2
    //261 - previous ip3
    //262 - previous ip4
    //340 - 369 username
    //369 - 399 password

    EEPROM.write(0,90);
    EEPROM.write(1,90);
    EEPROM.write(2,10);
    EEPROM.write(3,20);
    writeString(20,"OFF"); 
    writeString(25,"OFF"); 
    writeString(30,"OFF"); 
    writeString(35,"OFF"); 
    writeString(40,"OFF"); 
    writeString(45,"OFF");
    
    writeString(50,"deadeyetheory@gmail.com"); 
    writeString(100,"Default");
    writeString(120,"5");
    
    writeString(130,"0"); 
    writeString(135,"0"); 
    writeString(140,"Motion");  
    writeString(150,"100"); 
    writeString(160,"/DefaultLog.txt");

    writeString(190,"0"); 
    writeString(195,"0"); 
    writeString(200,"Motion");  
    writeString(210,"0"); 
    writeString(220,"/DefaultLog.txt");

    //LOGIN INFO
    writeString(340,"RazorScythe");
    writeString(369,"481264");
    
    EEPROM.write(240,0);
    EEPROM.write(241,0);
    //IP ADDRESS
    EEPROM.write(242,192);
    EEPROM.write(243,168);
    EEPROM.write(244,100);
    EEPROM.write(245,10);
    //SUBNET MASK
    EEPROM.write(246,255);
    EEPROM.write(247,255);
    EEPROM.write(248,255);
    EEPROM.write(249,0);
    //GATEWAY
    EEPROM.write(255,192);
    EEPROM.write(256,168);
    EEPROM.write(257,100);
    EEPROM.write(258,1);
    //previous IP Address
    EEPROM.write(259,0);
    EEPROM.write(260,0);
    EEPROM.write(261,0);
    EEPROM.write(262,0);    
    //ATTEMPT RESET
    EEPROM.write(250,7);
    //NETWORK STA STATUS
    writeString(251,"OFF");
    
    EEPROM.commit(); 
    
    
}

String randomName(){
  String Randomized = "abcdefghijklmnopqrstuvwxyz123456789";
  String generatedByte;
  for(int gen = 0; gen <= 7; gen++){
    char randomchar = Randomized[random(0, Randomized.length())];
    generatedByte += randomchar;
  }
  return generatedByte;
}

float spiffs_size(fs::FS &fs){
  float memory_size = 0;
  File root = fs.open("/"); 
  File file = root.openNextFile();
  while(file){
      list_name = file.name();
      memory_size += file.size();
      file = root.openNextFile();
  }
  return memory_size/1024;  
}

String byte_size(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + "B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + "KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + "MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
}

String init_index_data(){
  //150-total motion captured
  //155-total timelapse captured
  //160 - recent mode
  //170 - end time
  //180 - log file
  String timelapse_state = read_String(30);
  String motion_state = read_String(25);
  String runningtime = String(millis()/1000);
  String total_captured = String(image_captured);

  String recent_mode = read_String(140);
  String total_m_captured = read_String(130);
  String total_t_captured = read_String(135);
  String end_time = read_String(150);
  String recent_log_file = read_String(160);
  String N_STA = read_String(251);
  String list_data;
  list_data += timelapse_state; list_data += " ";
  list_data += motion_state; list_data += " ";
  list_data += runningtime; list_data += " ";
  list_data += total_captured; list_data += " ";

  list_data += recent_mode; list_data += " ";
  list_data += total_m_captured; list_data += " ";
  list_data += total_t_captured; list_data += " ";
  list_data += end_time; list_data += " ";
  list_data += recent_log_file; list_data += " ";
  list_data += N_STA; list_data += " ";
  return list_data;
}

//String init_SPIFFS_table(){
//   String returnText = "";
//   File root = SPIFFS.open("/"); 
//   File foundfile = root.openNextFile();
//   while (foundfile) {
//      slice_string = foundfile.name();
//      if(slice_string.substring(19) == ".txt" || slice_string.substring(20) == ".txt"){
//        returnText += "<tr><td class='fn'>"+String(foundfile.name())+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='action'><button class='open'>open</button>|<button class='SPIFFS_download'>download</button>|<button class='SPIFFS_delete'>remove</button></td></tr>"; 
//      }
//      foundfile = root.openNextFile();
//   }
//   root.close();
//   foundfile.close();
//  
//   return returnText;
//}
//

String spiffs_array(fs::FS &fs,String html){ 
   total_image = 0;
   String returnText = "";
   File root = fs.open("/"); 
   File foundfile = root.openNextFile();
   if(html == "MOTION"){
     while (foundfile) {
        slice_string = foundfile.name();
        if(slice_string.substring(19) != ".txt" && slice_string.substring(20) != ".txt" && slice_string != "/captured.jpg" && slice_string != "/gmail_account.txt"){
          returnText += "<tr><td class='fn'>"+String(foundfile.name())+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='action'><button class='view'>View</button>|<button class='download'>Download</button>|<button class='delete'>Delete</button>|<button class='send'>Send</button></td></tr>";
          total_image++;
        } 
        foundfile = root.openNextFile();
     }
   }
   else if(html == "INDEX"){
     while (foundfile) {
        returnText += "<tr><td class='fn'>"+String(foundfile.name())+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='action'><button class='view'>view</button>|<button class='JPG_download'>download</button>|<button class='IMAGE_delete'>remove</button></td></tr>"; 
        foundfile = root.openNextFile();
     }
   }
   root.close();
   foundfile.close();
  
   return returnText;
   delay(1);
}
//
//String init_ServerLog_table(){
//   String returnText = "";
//   File root = SD_MMC.open("/ServerLogs"); 
//   File foundfile = root.openNextFile();
//   for(int i = 0; i <= 45; i++){
//   //while (foundfile) {
//        slice_string = foundfile.name();
//        if(slice_string.substring(23) == ".txt" || slice_string.substring(24) == ".txt"){
//          returnText += "<tr><td class='fn'>"+slice_string.substring(12)+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='action'><button class='open'>open</button>|<button class='SDServer_download'>download</button>|<button class='SDServer_delete'>remove</button></td></tr>"; 
//  //        returnText += slice_string.substring(12); returnText += " ";
//  //        returnText += byte_size(foundfile.size()); returnText += " ";
//        }
//        foundfile = root.openNextFile();
//   }
//   root.close();
//   foundfile.close();
//  
//   return returnText;
//   delay(1);
//}
//
//String init_SPIFFSServerLog_table(){ 
//  String returnText = "";
//  File root = SD_MMC.open("/SPIFFSLogs");
//  File foundfile = root.openNextFile();
//  for(int i = 0; i <= 45; i++){
//  //while (foundfile) {
//      slice_string = foundfile.name();
//      if(slice_string.substring(23) == ".txt" || slice_string.substring(24) == ".txt"){
//        returnText += "<tr><td class='fn'>"+slice_string.substring(12)+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='action'><button class='open'>open</button>|<button class='SD_SPIFFSServer_download'>download</button>|<button class='SDSPIFFS_delete'>remove</button></td></tr>"; 
////        returnText += slice_string.substring(12); returnText += " ";
////        returnText += byte_size(foundfile.size()); returnText += " ";
//      }
//      foundfile = root.openNextFile();
//  }
//  root.close();
//  foundfile.close();
//  
//  return returnText;
//  delay(1);
//}

String spiffs_array(fs::FS &fs){
   File root = fs.open("/"); 
   File file = root.openNextFile();
   String spiffs_data;
   while(file){
      slice_string = file.name();
      if(slice_string.substring(19) != ".txt" && slice_string.substring(20) != ".txt" && slice_string != "/captured.jpg" && slice_string != "/gmail_account.txt"){
        spiffs_data += file.name();
        spiffs_data += " ";
        spiffs_data += byte_size(file.size());
        spiffs_data += " ";
      }
      file = root.openNextFile();
   }
   return spiffs_data;
}

String init_SPIFFS_table(){
   File root = SPIFFS.open("/"); 
   File file = root.openNextFile();
   String spiffs_data;
   while(file){
      slice_string = file.name();
      if(slice_string.substring(19) == ".txt" || slice_string.substring(20) == ".txt"){
        spiffs_data += file.name();
        spiffs_data += " ";
        spiffs_data += byte_size(file.size());
        spiffs_data += " ";
      }
      file = root.openNextFile();
   }
   return spiffs_data;
}

String init_ServerLog_table(){
   File root = SD_MMC.open("/ServerLogs"); 
   File file = root.openNextFile();
   String spiffs_data;
   while(file){
      slice_string = file.name();
      spiffs_data += slice_string.substring(12);
      spiffs_data += " ";
      spiffs_data += byte_size(file.size());
      spiffs_data += " ";
      file = root.openNextFile();
   }
   return spiffs_data;
}

String init_SPIFFSServerLog_table(){
  File root = SD_MMC.open("/SPIFFSLogs"); 
  File file = root.openNextFile();
  String spiffs_data;
  while(file){
    slice_string = file.name();
    spiffs_data += slice_string.substring(12);
    spiffs_data += " ";
    spiffs_data += byte_size(file.size());
    spiffs_data += " ";
    file = root.openNextFile();
  }
  return spiffs_data;
}

String init_FolderTimelapse_table(){ 
  String returnText = "";
  char path[64];
  String items;
  long size = 0;
  File root = SD_MMC.open("/Timelapse"); 
  File foundfile = root.openNextFile();
  for(int i = 0; i <= 45; i++){
  //while (foundfile) {
      if(foundfile.isDirectory()){
          slice_string = foundfile.name();
          Serial.print("  DIR : ");
          Serial.println(foundfile.name());
          sprintf(path,"%s/Items.txt",foundfile.name());
          File file = SD_MMC.open(path);
          while(file.available()){
            items += file.readString();
          }
          Serial.print("  Items : ");
          Serial.println(items);
          returnText += "<tr><td class='fn'>"+slice_string.substring(11)+"</td><td class='fs'>"+byte_size(foundfile.size())+"</td><td class='items'>"+items+"</td><td class='action'><button class='TFolder_Delete'>remove</button></td></tr>"; 
          items = "";
      }
      foundfile = root.openNextFile();
  }
  root.close();
  foundfile.close();
  
  return returnText;
  delay(1);
}

String processor(const String& var){
  String motion_state = read_String(25);  
  String autosend_state = read_String(40);  
  String email_target = read_String(50);  
  String autodelete_state = read_String(45);  
  String timelapse_state = read_String(30);
  String autosave_state = read_String(20);
  
  String recent_mode = read_String(160);
  String total_m_captured = read_String(150);
  String total_t_captured = read_String(155);
  String end_time = read_String(170);
  String recent_log_file = read_String(180);

  if(var == "ADMIN")
    return read_String(340);
  //index
  if(var == "RUNNING_TIME")
    return String(millis()/1000);
  if(var == "RECENT_MODE")
    return recent_mode;
  if(var == "TOTAL_MC")
    return total_m_captured;
  if(var == "TOTAL_TC")
    return total_t_captured;
  if(var == "END_TIME")
    return end_time;
  if(var == "RECENT_LOG")
    return recent_log_file;
   
  if(var == "ATTEMPT")
    return String(EEPROM.read(250));
  if(var == "IP1")
    return String(EEPROM.read(242));
  if(var == "IP2")
    return String(EEPROM.read(243));
  if(var == "IP3")
    return String(EEPROM.read(244));
  if(var == "IP4")
    return String(EEPROM.read(245));
  if(var == "S1")
    return String(EEPROM.read(246));
  if(var == "S2")
    return String(EEPROM.read(247));
  if(var == "S3")
    return String(EEPROM.read(248));
  if(var == "S4")
    return String(EEPROM.read(249));
  if(var == "G1")
    return String(EEPROM.read(255));
  if(var == "G2")
    return String(EEPROM.read(256));
  if(var == "G3")
    return String(EEPROM.read(257));
  if(var == "G4")
    return String(EEPROM.read(258));
  if(var == "SENDER")
    return emailSenderAccount;
  if(var == "RECIPIENT")
    return read_String(50);
    
  //table //cannot handle
//  if (var == "IMAGE_LIST")
//    return spiffs_array(SPIFFS,"INDEX");
//  if (var == "SPIFFS_LIST")
//    return init_SPIFFS_table();
//  if (var == "SERVER_LOG")
//    return init_ServerLog_table();
//  if (var == "SPIFFS_SERVER_LOG")
//    return init_SPIFFSServerLog_table();
  if(var == "TIMELAPSE_FOLDER")
    return init_FolderTimelapse_table();  
    
  //stream
  if(var == "PAN")
    return String(EEPROM.read(0));
  if(var == "TILT")
    return String(EEPROM.read(1));
  if(var == "AUTOSAVE_STATE")
    return autosave_state;
    
  //timelapse 
  if(var == "TOTAL_CAPTURED")
    return String(image_captured);
  if(var == "FOLDER_NAME")
    return folder_name;
    
  //motion
  if (var == "MOTION_IMAGE_LIST")
    return spiffs_array(SPIFFS,"MOTION");
  if(var == "TOTAL_IMAGE"){
    spiffs_array(SPIFFS,"MOTION");
    return String(total_image);
  }
  if(var == "TOTAL_SIZE")
    return String(spiffs_size(SPIFFS));
  if(var == "AUTOSEND_STATE")
    return autosend_state;
  if(var == "AUTODELETE_STATE")
    return autodelete_state;
  if(var == "DELAY_VALUE")
    return String(EEPROM.read(2));
  if(var == "RECIEPIENT")
    return email_target;
  if(var == "NEXT_TILT")
    return String((currentMillis - previousMillis)/1000);
  
  return String();
}

String init_motion_data(){
  // 25 motion sensor
  String motion_state = read_String(25);  
  String timelapse_state = read_String(30);
  String list_data;
  list_data += motion_state; list_data += " ";
  list_data += timelapse_state; list_data += " ";
  list_data += String(EEPROM.read(2)); list_data += " ";
  return list_data;
}

String init_stream_data(){
  String timelapse_state = read_String(30);
  String motion_state = read_String(25);
  String list_data;
  list_data += timelapse_state; list_data += " ";
  list_data += motion_state; list_data += " ";
  return list_data;
}

String init_timelapse_data(){
  String next_captured = String((currentMillis - previousMillis)/1000);
  String capturedtimer = read_String(120);
  String timelapse_state = read_String(30);
  String motion_state = read_String(25);
  String list_data;
  list_data += capturedtimer; list_data += " ";
  list_data += timelapse_state; list_data += " ";
  list_data += motion_state; list_data += " ";
  list_data += next_captured; list_data += " ";
  return list_data;
}
