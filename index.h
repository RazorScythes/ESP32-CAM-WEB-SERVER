static const char PROGMEM login_html[] = R"rawliteral(
  <!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
  <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
  <title>Login</title>
  <style>
        body,
        html {
      width:95%;
      margin:auto;
            font-family: sans-serif;
      margin-top:2px;
        }
        .loginbox{
            width: 350px;
            height: 420px;
            background-color: rgba(0,0,0,0.8);
            color: #fff;
            top: 55%;
            left: 50%;
            position: absolute;
            transform: translate(-50%,-50%);
            box-sizing: border-box;
            box-shadow: 0 2px 4px rgba(0,0,0,0.6);
            padding: 70px 30px;
            font-family:sans-serif;
        }
        .loginbox label{
            margin: 0px;
            padding: 0px;
        }
        .loginbox input[type="text"], input[type="password"], input[type="email"], input[type="submit"]{
            width:100%;
            margin-bottom: 20px;
        }
        .loginbox input[type="text"], input[type="password"], input[type="email"]{
            border: none;
            border-bottom: 1px solid #fff;
            background: transparent;
            outline: none;
            height: 40px;
            color: #fff;
            font-size: 15px;
        }
        h1{
            margin: 0px;
            padding: 5px 0 10px;
            text-align: center;
            font-size: 30px;
        }
        h5{
            margin: 0px;
            padding: 5px 0 10px;
            text-align: center;
        }
        .loginbox span{
            font-size:15px;
            line-height:30px;
            color: #ff0000;
            text-shadow:1px 1px black;
        }
        input[type="submit"]{
            border: none;
            outline: none;
            height: 40px;
            background: #fb2525;
            color: #fff;
            font-size: 18px;
            border-radius:20px;
            margin-top:10px;
        }
        input[type="submit"]:hover{
            cursor: pointer;
            background: #ffc107;
            color: #000;
        }
        .loginbox a{
            text-decoration: none;
            font-size:15px;
            line-height:30px;
            color: darkgrey;
            margin-left:10px;
        }
        .loginbox a:hover{
            color: #ffc107;
        }
        .Usericon{
            width: 45%;
            height: 40%;
            border-radius: 50%;
            position: absolute;
            top: -100px;
            left: calc(50% - 80px);
        }
        .back{
            text-decoration:none;
            padding:10px;
            float:left;
            color:black;
            font-family:georgia;
        }
        .back:hover{
            text-decoration:underline;
        }
        #label{
            margin:0 5px;
        }
        #remember{
            float:left;
        }
        @media only screen and (max-width: 600px) {
            .loginbox{
                width: 80%;
                height: 63%;
            }
            .loginbox input[type="text"], input[type="password"], input[type="email"], input[type="submit"]{
                margin-bottom: 10px;
            }
            h1{
                font-size: 25px;
            }
            .Usericon{
                width: 45%;
                height: 36%;
                left: calc(55% - 80px);
            }
        }
    </style>
</head>
<body>
    <!-- Login Form -->
    <div class="loginbox">
        <img class="Usericon" src="data:image/png;base64,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">
        <h1> Web Server Login</h1>  
            <span>*Errormsg*</span>
        <form action="/auth">
            <label>Username</label><br>
            <input type="text" name="username" value="*username*" placeholder="Enter Username" maxlength="30" autocomplete="off">
            <label>Password</label>
            <input type="password" name="password" value="*password*" placeholder="Enter Password" maxlength="30" autocomplete="off">
            <input type="checkbox" name="credentials" id="remember" *checked*><label id="label"> Remember credentials</label>
            <input type="submit" value="login">
        </form>
    </div>  
</body>
</html>
)rawliteral";
static const char PROGMEM index_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
  <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
  <title>Main</title>
<style>
    body,
        html {
      width:95%;
      margin:auto;
            font-family: sans-serif;
      margin-top:2px;
        }
    #container{
      width:95%;
      height:auto;
      position: relative;
          margin: auto;       
    }
    #dashboard{
      float:left;
      width:350px;
      height:800px;
      background-color: #191919;
    }
    #dashboard header{
      width:100%;
      height:70px;
      background-color:#03254c; 
      text-align: center;
    }
    #dashboard p{
      color:ivory;
      display:inline-block;
      font-size: 30px;
      margin-top:20px;
    }
    #dashboard header span{
      display:inline-block;
      margin:20px 5px;
      color:ivory;
      font-size:25px;
      display:none;
    }
    #dashboard header span:hover{
      cursor:pointer;
    }
    #content{
      width:1350px;
      height:800px;
      background-color:#F5FFFA;
    }
    #content header{
      width:100%;
      height:70px;
      background-color:#03254c;
      text-align: left;
    }
    #content header span{
      display:inline-block;
      margin:20px 5px;
      color:ivory;
      font-size:25px;
      display:none;
    }
    #content header span:hover{
      cursor:pointer;
    }
    #info{
      width:100%;
      height:20%;
    }
    #info img{
      float:left;
      width:120px;
      height:75%;
      border-radius: 100px;
      margin:20px;
    }
    #info label{
      color:ivory;
      display:inline-block;
      margin-top:60px;
    }
    #info a{
      text-decoration: none;
      margin-top:5px;
      color:yellow;
    }
    #info a:hover{
      color:gold;
    }
    #main{
      margin-top:20px;
      width:100%;
      height:550px;
      background-color: #333;
    }
    ul {
      list-style-type: none;
      margin: 0;
      padding: 0;
      overflow: hidden;
      background-color: #333;
        }
    a{
      text-decoration: none;
      color:ivory;
      display:inline;
    }
    li:hover{
      background-color: #1c2841;
      cursor: pointer;
    }
    li{
      width:90%;
      padding:20px;
      text-decoration: none;
      color:ivory;
    }
    .active{background-color: #1c2841;}
    li label{
      float: right;
    }
    .menutext{
      width:88.5%;
      display:inline-block;
      padding:20px;
      color:gray;
      background-color: #0c0c0c;
    }
    .title{
      display:inline-block;
      font-size:40px;
      margin:30px 10px 0 50px;
      color:  #4169E1;
    } 
    hr{float:left; border:none; width:900px; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:50px;}
    nav{
      float:left;
      width:50%;
      margin-left:50px;
    }
    nav button{
      border:none;
      background:transparent;
      padding:0 10px;
      font-size: 15px;
      outline:none;
    }
    nav button:hover{cursor: pointer;}
    #home{height:650px;}
    #recent,#ongoing{
      float:left;
      width:40%;
      height:35%;
      margin:20px 50px;
      background-color:#b2b2b2;
      box-shadow: 2px 2px #808080;
    }
    #recent header,#ongoing header{
      float:left;
      width:100%;
      height:30%; 
    }
    .headertitle,.Recentheadertitle{
      display:inline-block;
      margin:25px 15px;
      color:#FFF;
      font-size: 20px;
      font-weight: bold;
      text-shadow:1px 1px black;
    }
    #recent header i,#ongoing header i{
      float:right;
      margin:20px;
      color:#FFF;
      font-size: 30px;
    }
    #ongoing label{display:inline-block;}
    .activeMode{
      font-size:20px;
      font-weight: bold;
      color:gold;
      text-shadow:2px 2px black;
      margin:20px 0;
    }
    #statbox{
      color:black;
      font-size: 19px;
      margin:5px 10px;
      font-weight: bold;
    }
    .status{color:#00FF00; text-shadow:1.5px 1px black; font-weight: normal;}
    .runningTime,#timer{
      color:black;
      margin:5px 10px;
      font-size: 19px;
      font-weight: bold;
      display:block;
    }
    .endRunningTime,#endtime{
      color:black;
      margin:0 10px;
      font-size: 19px;
      font-weight: bold;
      display:inline-block;
    }
    #endtime,#current{
      display:none;
    }
    .captureLabel,.totalcaptureLabel{
      color:black;
      margin:5px 10px;
      font-size: 19px;
      font-weight: bold;
    }
    .totalcaptureLabel{
      display:block;
      margin:10px 10px;
    }
    #endtime, .totalcounter, .file{font-weight: normal;}
    .RecentmodeLabel,.modeLabel,.logfile{
      margin:12px 10px;
      color:black;
      font-size: 19px;
      font-weight: bold;
      display:block;
    }
    .logfile{width:100%;}
    #timer,.counter{font-weight: normal;}
    .alert, .alert2 {
      width:90.5%;
      height:25px;
      padding: 10px;
      background-color: #f44336;
      color: white;
      opacity: 1;
      transition: opacity 0.6s;
      float:left;
      font-size:12px;
      text-align:left;
      margin:5px 12px;
      line-height: 15px;
      position:relative;
    }
    .alert {
      width:65%;
      height:20px;
      line-height: 20px;
      margin:10px 0 0 50px;
      float:left;
      display:none;
    }
    .alert.success ,.alert2.success{background-color: #4CAF50;}
    .alert.info ,.alert2.info{background-color: #2196F3;}
    .alert.warning ,.alert2.warning{background-color: #ff9800;}
    .closebtn {
      float:right;
      color: white;
      font-weight: bold;
      float: right;
      font-size: 22px;
      line-height: 20px;
      cursor: pointer;
      transition: 0.3s;
    }
    .closebtn:hover {
      color: black;
    }
    tbody {
      display:inline-block;
      width: 100%;
      height:500px;
      overflow-y: scroll;
        } 
    .fn{width:200px;} .fs{width:150px;} .action{width:350px;}
    th{
      
      text-align:left;
      padding:12px;
      color:#191970;
      background-color:#B0C4DE;
      font-weight: normal;
    }
    td{
      text-align:left;
      padding:12px;
      color:#191970;
      background-color:#F0F8FF;
      font-weight: normal;
      font-size:13px;
    }
    table {
      display:inline-block;
      border-collapse: collapse;
      margin:10px 0 0 50px;
      height:500px;
    }
    .view,.open,.IMAGE_delete, .SPIFFS_delete, .SDServer_delete, .SDSPIFFS_delete,.SPIFFS_download, .JPG_download, .SDServer_download, .SD_SPIFFSServer_download{
      background:transparent;
      color:#191970;
      border:none;
      outline:none;
    }
    .view:hover,.open:hover,.IMAGE_delete:hover, .SPIFFS_delete:hover, .SDServer_delete:hover, .SDSPIFFS_delete:hover,.SPIFFS_download:hover, .JPG_download:hover, .SDServer_download:hover, .SD_SPIFFSServer_download:hover{
      text-decoration: underline;
      cursor: pointer;
    }
    select{
      float:left;
      margin:30px 300px 0 50px;
      padding:2px 20px;
      border:none;
      border-bottom: 2px solid #4169E1;
      outline:none;
    }
    #log{display:none}
    #IMAGE_Table,#SDServer_Table,#SDSPIFFSServer_Table{display:none;}
    #modalWindow {
            position: fixed;
            top: 0;
            right: 0;
            bottom: 0;
            left: 0;
            background: rgba(0, 0, 0, 0.8);
            z-index: 9999;
            opacity:1;
      display:none;
        }
        #modalWindow:target {
            opacity:1;
            pointer-events: auto;
        }
        #modalWindow > div {
            width: 80%;
            height: 30%;
            position: relative;
            margin-top:150px;
            padding: 20px 20px 13px 20px;
            background: transparent;
        }
        #modalimg{
            position:absolute;
            max-width: 80%;
            height: 500px;
            border:1px solid rgba(0,0,0,0.1);
            top:1%;
            left: 60%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
        }
    #settings{
      float:left;
      width:65.5%;
      height:73%;
      margin:15px 50px;
      display:none;
      overflow: hidden;
      overflow-y: scroll;
    }
    .settingsHeader{
      display:inline-block;
      margin:25px 15px;
      color:gold;
      font-size: 23px;
      font-weight: bold;
      text-shadow:1px 1px black;
    }
    #smtp,#mainconfig{
      float:left;
      width:97%;
      height:33%;
      margin:20px 50px;
      background-color:#b2b2b2;
    }
    #network{
      float:left;
      width:97%;
      height:37%;
      margin:20px 50px;
      background-color:#b2b2b2;
    }
    /* Chrome, Safari, Edge, Opera */
    input::-webkit-outer-spin-button,
    input::-webkit-inner-spin-button {
      -webkit-appearance: none;
      margin: 0;
    }
    #network,#smtp,#mainconfig{
      margin:10px;
    }
    #network .settingwrap,#mainconfig .settingwrap{
      margin:10px 20px;
    }
    #mainconfig .settingwrap{
      margin:10px 20px;
    }
    #mainconfig button{cursor:pointer;}
    #network .settingwrap label{
      display: inline-block; width: 100px;
      margin-right:10px;
      text-align: right;
    }
    #network .settingwrap input,#mainconfig .settingwrap input{
      width:30px;
    }
    #mainconfig input{
      outline:none;
      background:transparent;
      border:none;
      border-bottom: 1px solid red;
      color:red;
    }
    #mainconfig div label{
      display: inline-block;
      width:220px;
      text-align: right;
    }
    #network .settingwrap button{
      margin-left:20px;
      cursor:pointer;
    }
    #resetEEPROM{margin:0 20px;}
    #resetSPIFFS{margin:0 20px;}
    #smtp .settingwrap{
      margin:10px 20px;
    }
    #smtp .settingwrap label{
      display: inline-block; width: 135px;
      margin-right:20px;
      text-align: right;
    }
    #smtp .settingwrap input{
      width:200px;
    }
    #smtp .settingwrap button{
      margin-left:20px;
      cursor:pointer;
    }
    #networkSTA{
      float:right;
      margin:19px 10px;
      padding:8px;
      background:red;
      border:none;
      color:white;
      font-weight: bold;
      cursor:pointer;
      outline:none;
    }
    input::-webkit-outer-spin-button,
    input::-webkit-inner-spin-button {
      -webkit-appearance: none;
      margin: 0;
    }
    @media only screen and (max-width: 600px) {
      body,
      html {
        width:100%;
      }
      #dashboard{
        float:left;
        width:350px;
        height:600px;
        background-color: #191919;
        position:absolute;
        right:750px;
        z-index:1;
      }
      #dashboard header{
        width:100%;
        height:70px;
        background-color:#03254c; 
        text-align: left;
      }
      #dashboard p{
        color:ivory;
        display:inline-block;
        font-size: 30px;
        margin-top:20px;
        margin-left:10px;
      }
      #dashboard header span{
        float:right;
        display:block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #content{
        width:100%;
        background-color:#F5FFFA;
      }
      #content header{
        width:100%;
        height:70px;
        background-color:#03254c;
        text-align: left;
      }
      #content header span{
        display:inline-block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #info{
        width:100%;
        height:20%;
      }
      #info img{
        float:left;
        width:90px;
        height:75%;
        border-radius: 100px;
        margin:20px;
      }
      #info label{
        color:ivory;
        display:inline-block;
        margin-top:45px;
      }
      #info a{
        text-decoration: none;
        margin-top:5px;
        color:yellow;
      }
      #info a:hover{
        color:gold;
      }
      #main{
        margin-top:30px;
        width:100%;
        height:inherit;
        background-color: #333;
      }
      .menutext{
        width:88.5%;
        display:inline-block;
        padding:20px;
        color:gray;
        background-color: #0c0c0c;
      }
      .title{
        display:inline-block;
        font-size:40px;
        margin:30px 10px 0 10px;
        color:  #4169E1;
      }
      hr{float:left; border:none; width:95%; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:10px;}
      nav{
        float:left;
        width:100%;
        margin-left:10px;
      }
      #recent,#ongoing{
        width:96%;
        height:35%;
        margin:20px 10px;
      }
      #recent header,#ongoing header{
        width:100%;
        height:30%; 
      }
      #network,#smtp,#mainconfig{
        width:96%;
        margin:10px 0;
      }
      #settings{
        float:left;
        width:100%;
        height:73%;
        margin:15px 10px;
      }
      .headertitle,.Recentheadertitle{
        display:inline-block;
        margin:25px 15px;
        color:#FFF;
        font-size: 20px;
        font-weight: bold;
        text-shadow:1px 1px black;
      }
      .settingsHeader{
        font-size: 20px;
      }
      #recent header i,#ongoing header i{
        float:right;
        margin:20px;
        color:#FFF;
        font-size: 30px;
      }
      #ongoing label{display:inline-block;}
      .status{color:#00FF00; text-shadow:1.5px 1px black; font-weight: normal;}
      .activeMode,#statbox,.runningTime,.endRunningTime,.captureLabel,.totalcaptureLabel,#endtime,#timer,.modeLabel,.logfile{
        font-size:17px;
      }
      #timer,.counter{font-weight: normal;}
      .alert, .alert2 {
        width:inherit;
                margin:0;
      }
      .alert {
        width:93.9%;
        height:20px;
        line-height: 20px;
        margin:10px 0 0 0;
      }
      tbody {
        width:99.5%;
      } 
      .view,.open,.IMAGE_delete, .SPIFFS_delete, .SDServer_delete, .SDSPIFFS_delete,.SPIFFS_download, .JPG_download, .SDServer_download, .SD_SPIFFSServer_download,.Folder_Download,.Folder_Delete,.TFolder_Download,.TFolder_Delete{
        font-size:12px;
      }
      td{
        font-size:12px;
      }
      table {
        display:inline-block;
        border-collapse: collapse;
        margin:10px 0 0 0;
        height:100%;
        width:98%;
      }
      select{
        margin:30px 300px 0 0;
        background:transparent;
      }
      th.fn{width:45%;} th.fs{width:15%;} th.action{width:100%;}
            td.fn{width:45%;} td.fs{width:15%;}td.action{width:100%;}
      #modalWindow {
                position: fixed;
                top: 0;
                right: 0;
                bottom: 0;
                left: 0;
                background: rgba(0, 0, 0, 0.8);
                z-index: 9999;
                opacity:1;
            }
            #modalWindow:target {
                opacity:1;
                pointer-events: auto;
            }
            #modalWindow > div {
                width: 80%;
                height: 30%;
                position: relative;
                margin-top:150px;
                padding: 20px 20px 13px 20px;
                background: transparent;
            }
            #modalimg{
                position:absolute;
        width: 100%;
                max-width: 100%;
                height: 120%;
                border:1px solid rgba(0,0,0,0.1);
                left: 55%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
            }
      #mainconfig span,#network span{
        text-align: left;
        font-size: 14px;
        margin: 2px 0px;
      }
      #mainconfig input{
        height:10px;
        font-size: 13px;
        outline:none;
        background:transparent;
        border:none;
        border-bottom: 1px solid red;
        color:red;
      }
      #mainconfig div label{
        width:190px;
        font-size:13px;
        font-weight: bold;
      }
      #resetEEPROM{margin:0 12px;}
      #resetSPIFFS{margin:0 12px;}
      #network div label{
        font-size:13px;
        font-weight: bold;
        margin:-20px;
        margin-right:0px;
      }
      #smtp div label{
        float:left;
        font-size:13px;
        font-weight: bold;
        margin-left:-30px;
      }
      #smtp .settingwrap input{
        display:inline-block;
        height:12px;
        font-size: 12px;
        width:30%;
        margin-left:-2px;
      }
      #network .settingwrap input{
        display:inline;
        height:12px;
        font-size: 12px;
        width:8.2%;
        margin-left:-2px;
      }
      #network button{
        float:right;
      }
    }
  </style>
</head>
<body>
  <div id="container">
  <div id="dashboard">
    <header>
      <p><b>Camera</b> Server</p>
      <span id="closeslider"><i class="fa fa-bars" aria-hidden="true"></i></span>
    </header>
    <div id="info">
      <img src="data:image/png;base64,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">
      <label><b>Admin:</b> *ADMIN*</label><br>
      <a href="#" id="logout"><i class="fa fa-cogs" aria-hidden="true"></i> Logout Account</a>
    </div>
    <div id="main">
      <label class="menutext">Dashboard</label>
      <ul>
        <a href="/"><li class="active"><i class="fa fa-tachometer" aria-hidden="true"></i> Overview <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/streaming"><li><i class="fa fa-video-camera" aria-hidden="true"></i> Stream <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/motion"><li><i class="fa fa-dot-circle-o" aria-hidden="true"></i> Motion Sensor <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/timelapse"><li><i class="fa fa-clock-o" aria-hidden="true"></i> Timelapse <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/record"><li><i class="fa fa-book" aria-hidden="true"></i> Records <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
      </ul>
    </div>
  </div>
  <div id="content">
    <header>
      <span id="slider"><i class="fa fa-bars" aria-hidden="true"></i></span>
    </header>
    <label class="title">Overview</label>
    <hr>
    <nav>
      <button id="h">Home</button>
      <button id="s">Settings</button>
    </nav>
    <div id="home">
    <div id="ongoing">
      <header>
        <label class="headertitle">Ongoing: </label><label class="activeMode"></label> <a id="redirectToActive" href="#"><i class="fa fa-angle-double-right" aria-hidden="true"></i></a>
      </header>
      <div>
        <div id="current">
        <div class="alert2" id="disable">
          <strong></strong> 
        </div>
        <div id="timer">
          <label id="statbox">Status: <label class="status">Active</label></label><br>
          <label class="runningTime">Running Time: </label>
          <span id="hour">00</span> :
          <span id="min">00</span> :
          <span id="sec">00</span> <br>
          <label class="captureLabel">Captured Image: <label class="counter">0</label></label>
        </div>
        </div>
      </div>
    </div>
    <div id="recent">
      <header>
        <label class="Recentheadertitle">Recent Activity</label> <a href="#"><i class="fa fa-angle-double-right" aria-hidden="true"></i></a>
      </header>
      <div>
        <div id="endtime">
          <label class="RecentmodeLabel"></label>
          <label class="endRunningTime">End Time: </label>
                    <span id="endhour">00</span> :
                    <span id="endmin">00</span> :
          <span id="endsec">00</span>
          <label class="totalcaptureLabel">Total Captured: <label class="totalcounter">0</label></label>
          <label class="logfile">Log filename: <label class="file">/logfile.txt</label></label>
                </div>
      </div>
    </div>
    </div>
    <div id="settings">
      <div id="mainconfig">
        <header>
          <label class="settingsHeader">Main Configuration</label>
        </header>     
        <div class="settingwrap"><span style="font-weight:bold; color:red;">Restore Default EEPROM configuration when failed to start server </span><input type="number" id="attemptTimes" value="*ATTEMPT*" style="text-align:center; font-weight:bold;" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==2) return false;"><span style="font-weight:bold; color:red;"> times</span></div>
        <div class="settingwrap"><label>Reset EEPROM Configuration:</label><button id="resetEEPROM">Confirm</button></div>
        <div class="settingwrap"><label>Clear SPIFFS Configuration:</label><button id="resetSPIFFS">Confirm</button></div>
      </div>
      <div id="network">
        <header>
          <label class="settingsHeader">Internet Connection</label> <button id="networkSTA">Disabled</button>
        </header> 
        <div class="settingwrap"><span style="font-weight:bold; color:red;">Automatically send an email when an new IP Address has been set</span></div>
        <div class="settingwrap"><label>IP Address:</label><input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="IP1" placeholder="*IP1*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="IP2" placeholder="*IP2*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="IP3" placeholder="*IP3*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="IP4" placeholder="*IP4*"> <button id="IpConfig">Save</button></div>
        <div class="settingwrap"><label>Subnet Mask:</label><input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="S1" placeholder="*S1*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="S2" placeholder="*S2*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="S3" placeholder="*S3*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="S4" placeholder="*S4*"> <button id="SubnetConfig">Save</button></div>
        <div class="settingwrap"><label>Gateway:</label><input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="G1" placeholder="*G1*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="G2" placeholder="*G2*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="G3" placeholder="*G3*"> <input type="number" pattern="/^-?\d+\.?\d$/" onKeyPress="if(this.value.length==3) return false;" id="G4" placeholder="*G4*"> <button id="GatewayConfig">Save</button></div>
      </div>
      <div id="smtp">
        <header>
          <label class="settingsHeader">SMTP Account</label>
        </header> 
        <div class="settingwrap"><label>Sender Account:</label><input id="input_sender" type="text" maxlength="30" placeholder="*SENDER*"></div>
        <div class="settingwrap"><label>Sender Password:</label><input id="input_password" type="password" maxlength="30"> <button id="gmail_account">Save</button></div>
        <div class="settingwrap"><label>Email Recipient:</label><input id="input_recipient" type="text" maxlength="30" placeholder="*RECIPIENT*"> <button id="recipient">Save</button></div>
      </div>
    </div>
    <div id="modalWindow">
      <div><img src="display" id="modalimg"></div>
    </div>
  </div>
  </div>
</body>
</html>
<script>
    var running_hrcnt = 0; var running_mcnt = 0; var running_scnt; //running timer
    var running_sec; var running_min; var running_hour;

    var running_value = 0;
  var running_milisec = 0;
    var end_hrcnt = 0; var end_mcnt = 0; var end_scnt; //end time
    var end_value = 0;

  $(window).on("load", function() { 
    $("#modalWindow").css("display", "none");
    var init_data = [];
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
            init_data.push(x);
            x = "";
          }
        }
        console.log(init_data);
      
        running_value = parseInt(init_data[2])
        if(init_data[1] == "ON "){
          $(".activeMode").html("Motion Sensor")
          $("#disable").html("<strong>Motion is currently running, SD card function won't be available until motion sensor turn off.</strong>")
          $(".status").html("Active")
          $("#redirectToActive").attr("href", "/motion");
          $(".counter").html(parseInt(init_data[3]))
          running_setTime();
          setInterval(running_time, 10);
          $("#current").css("display","inline-block")
        }
        else if(init_data[0] == "ON "){
          $(".activeMode").html("Timelapse")
          $("#disable").html("<strong>Timelapse is currently running, Limit use of video streaming to reduce system crash.</strong>")
          $(".status").html("Running")
          $("#redirectToActive").attr("href", "/timelapse");
          $(".counter").html(parseInt(init_data[3]))
          running_setTime();
          setInterval(running_time, 10);
          $("#current").css("display","inline-block")
        }
        else{
          $(".headertitle").html("No Ongoing Activity")
        }

        if(init_data[4] == "Motion "){
          $(".RecentmodeLabel").html("Started: <label class='activeMode'>Motion Sensor</label>")
          $(".totalcounter").html(init_data[5])
          $(".file").html(init_data[8])
          end_value = parseInt(init_data[7])
          end_setTime();
          $("#endtime").css("display","inline-block")
        }
        else if(init_data[4] == "Timelapse "){
          $(".RecentmodeLabel").html("Started: <label class='activeMode'>Timelapse</label>")
          $(".totalcounter").html(init_data[6])
          $(".file").html(init_data[8])
          end_value = parseInt(init_data[7])
          end_setTime();
          $("#endtime").css("display","inline-block")
        }
        else{
          $(".Recentheadertitle").html("No Recent Activity")
        }

        if(init_data[9] == "ON "){
          $("#networkSTA").html("Enabled");
          $("#networkSTA").css("background","green");
        }
        else{
          $("#networkSTA").html("Disabled");
          $("#networkSTA").css("background","red");
        }
      }
    }; 
    xhttp.open("GET", "/init_index_data", true);    
    xhttp.send();
  });
    function running_time(){
        running_milisec = ++running_milisec;
        if(running_milisec === 100) {running_milisec = 0; running_sec = ++running_sec;}
        if(running_sec == 60) {running_min = ++running_min; running_sec = 0;}
        if(running_min == 60) {running_min = 0; running_hour = ++running_hour;}
        $("#sec").html(checkTime(running_sec));
        $("#min").html(checkTime(running_min));
        $("#hour").html(checkTime(running_hour));
    }
    function running_setTime(){
        while(running_value > 3600){running_hrcnt++; running_value -= 3600;}
        while(running_value > 60){running_mcnt++; running_value -= 60;}
        running_scnt = running_value;
        running_sec = running_scnt; running_min = running_mcnt; running_hour = running_hrcnt;
    }

    function end_setTime(){
        while(end_value > 3600){end_hrcnt++; end_value -= 3600;}
        while(end_value > 60){end_mcnt++; end_value -= 60;}
        end_scnt = end_value;
        end_sec = end_scnt; end_min = end_mcnt; end_hour = end_hrcnt;
        $("#endsec").html(checkTime(end_sec));
        $("#endmin").html(checkTime(end_min));
        $("#endhour").html(checkTime(end_hour));
    }
    function checkTime(i) {
        if (i < 10){i = "0" + i;}
        return i;
    }
  $("#h").on('click',function(e){
    $("#log").css("display","none"); $("#settings").css("display","none"); $("#home").css("display","block"); $("#h").css("text-decoration","underline"); $("#l").css("text-decoration","none"); $("#s").css("text-decoration","none");
    $(".title").html("Overview")
  });
  $("#s").on('click',function(e){
    $("#log").css("display","none"); $("#home").css("display","none"); $("#settings").css("display","block"); $("#s").css("text-decoration","underline"); $("#l").css("text-decoration","none"); $("#h").css("text-decoration","none");
    $(".title").html("Settings")
  });

  $('#modalWindow').on('click',function(e){
      $(this).css("display", "none");
    });

  function display(res) {
    var val = $("#modalimg").attr("src")
    $("#modalimg").attr("src", val + '?_=' + new Date().getTime());
    }
  $("#logout").click(function () {
    window.location.href = "/login";
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/logout", true);    
    xhttp.send();
  });
  $("#slider").click(function () {
    $("#dashboard").animate({ "right": 0 }, "slow");
  });
  $("#closeslider").click(function () {
    $("#dashboard").animate({ "right": 750 }, "slow");
  });
  $("#resetEEPROM").click(function () {
    if(confirm("Do you want to return EEPROM default configuration?")){
      alert("Reseting EEPROM config, Server is restarting... Please wait");
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("GET", "/resetEEPROM", true);    
      xhttp.send();
    }
  });
  $("#resetSPIFFS").click(function () {
    if(confirm("Do you want to delete all SPIFFS Data?")){
      alert("Deleting all data, Server is restarting... Please wait");
      var xhttp = new XMLHttpRequest(); 
      xhttp.open("GET", "/resetSPIFFS", true);    
      xhttp.send();
    }
  });
  $("#gmail_account").click(function () {
    if($("#input_sender").val() != ""){
      if(isValidEmailAddress($("#input_sender").val()) && $("#input_password").val() != ''){
        alert("Input Sender Saved, Restart to take changes.");
        var xhttp = new XMLHttpRequest();    
        xhttp.open("GET", "/sender_account?value="+$("#input_sender").val()+" "+$("#input_password").val()+" ", true);    
        xhttp.send();
      }
      else if($("#input_sender").val() == ''){
        alert("Error: Email sender is empty!");
      }
      else if($("#input_password").val() == ''){
        alert("Error: Password is empty!");
      }
      else{
        alert("Invalid Sender Email Address!");
      }
    }
  });
  $("#recipient").click(function () {
    if($("#input_recipient").val() != ""){
      if(isValidEmailAddress($("#input_recipient").val())){
        alert("Input Recipient Saved!");
        var xhttp = new XMLHttpRequest();    
        xhttp.open("GET", "/email_address?value="+$("#input_recipient").val(), true);    
        xhttp.send();
      }
      else{
        alert("Invalid Recipient Email Address!");
      }
    }
  });
  $("#IpConfig").click(function () {
    if($("#IP1").val() != "" && $("#IP2").val() != "" && $("#IP3").val() != "" && $("#IP4").val() != ""){
      if(confirm("Are you sure you want to change the IP Address?")){
        alert("Restart the system to take changes.");
        var xhttp = new XMLHttpRequest();    
        xhttp.open("GET", "/ip1?value="+$("#IP1").val(), true);    
        xhttp.send();
        var xhttp2 = new XMLHttpRequest();    
        xhttp2.open("GET", "/ip2?value="+$("#IP2").val(), true);    
        xhttp2.send();
        var xhttp3 = new XMLHttpRequest();    
        xhttp3.open("GET", "/ip3?value="+$("#IP3").val(), true);    
        xhttp3.send();
        var xhttp4 = new XMLHttpRequest();    
        xhttp4.open("GET", "/ip4?value="+$("#IP4").val(), true);    
        xhttp4.send();
      }
    }
    else{
      alert("Error: Input field is empty or invalid");
    }
  });
  $("#SubnetConfig").click(function () {
    if($("#S1").val() != "" && $("#S2").val() != "" && $("#S3").val() != "" && $("#S4").val() != ""){
      if(confirm("Are you sure you want to change the subnetmask?")){
        alert("Restart the system to take changes.");
        var xhttp = new XMLHttpRequest();    
        xhttp.open("GET", "/s1?value="+$("#S1").val(), true);    
        xhttp.send();
        var xhttp2 = new XMLHttpRequest();    
        xhttp2.open("GET", "/s2?value="+$("#S2").val(), true);    
        xhttp2.send();
        var xhttp3 = new XMLHttpRequest();    
        xhttp3.open("GET", "/s3?value="+$("#S3").val(), true);    
        xhttp3.send();
        var xhttp4 = new XMLHttpRequest();    
        xhttp4.open("GET", "/s4?value="+$("#S4").val(), true);    
        xhttp4.send();
      }
    }
    else{
      alert("Error: Input field is empty or invalid");
    }
  });
  $("#GatewayConfig").click(function () {
    if($("#G1").val() != "" && $("#G2").val() != "" && $("#G3").val() != "" && $("#G4").val() != ""){
      if(confirm("Are you sure you want to change the default gateway?")){
        alert("Restart the system to take changes.");
        var xhttp = new XMLHttpRequest();    
        xhttp.open("GET", "/g1?value="+$("#G1").val(), true);    
        xhttp.send();
        var xhttp2 = new XMLHttpRequest();    
        xhttp2.open("GET", "/g2?value="+$("#G2").val(), true);    
        xhttp2.send();
        var xhttp3 = new XMLHttpRequest();    
        xhttp3.open("GET", "/g3?value="+$("#G3").val(), true);    
        xhttp3.send();
        var xhttp4 = new XMLHttpRequest();    
        xhttp4.open("GET", "/g4?value="+$("#G4").val(), true);    
        xhttp4.send();
      }
    }
    else{
      alert("Error: Input field is empty or invalid");
    }
  });
  $("#attemptTimes").change(function () {
    var xhttp = new XMLHttpRequest(); 
    xhttp.open("GET", "/attemptTimes?value="+$(this).val(), true);    
    xhttp.send();
  });
  $("#networkSTA").click(function () {
    var xhttp = new XMLHttpRequest(); 
    if($(this).text() == "Disabled"){
      $(this).html("Enabled");
      $(this).css("background","green");
      xhttp.open("GET", "/networkSTA?value=ON", true);   
    }
    else if($(this).text() == "Enabled"){
      $(this).html("Disabled");
      $(this).css("background","red");
      xhttp.open("GET", "/networkSTA?value=OFF", true);   
    }
    xhttp.send();
  });
  $('body').on('click', '.closebtn', function() {
        var div = $(this).closest('div');
        $(this).closest('div').animate({opacity:0},700, () => {
            $(this).closest('div').css("display","none");
            $(this).closest('div').css("opacity","1");
        });
    });
  function isValidEmailAddress(emailAddress) {
    var pattern = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}$/;
    return pattern.test(emailAddress);
  };
</script>
)rawliteral";

static const char PROGMEM motion_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,height=device-height initial-scale=1">
    <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
    <title>Motion Capture</title>
    <style>
          body,
            html {
          width:95%;
          margin:auto;
                font-family: sans-serif;
          margin-top:2px;
            }
        #container{
          width:95%;
          height:auto;
          position: relative;
              margin: auto;        
        }
        #dashboard{
          float:left;
          width:350px;
          height:800px;
          background-color: #191919;
        }
        #dashboard header{
          width:100%;
          height:70px;
          background-color:#03254c; 
          text-align: center;
        }
        #dashboard p{
          color:ivory;
          display:inline-block;
          font-size: 30px;
          margin-top:20px;
        }
        #dashboard header span{
          display:inline-block;
          margin:20px 5px;
          color:ivory;
          font-size:25px;
          display:none;
        }
        #dashboard header span:hover{
          cursor:pointer;
        }
        #content{
          width:1350px;
          height:800px;
          background-color:#F5FFFA;
        }
        #content header{
          width:100%;
          height:70px;
          background-color:#03254c;
          text-align: left;
        }
        #slider,#reset{
          display:inline-block;
          margin:20px 5px;
          color:ivory;
          font-size:25px;
          display:none;
        }
        #slider:hover,#reset:hover{
          cursor:pointer;
        }
        #reset{
          margin:25px;
          display:block;
          float:right;
        }
        #infos{
          width:100%;
          height:20%;
        }
        #infos img{
          float:left;
          width:120px;
          height:75%;
          border-radius: 100px;
          margin:20px;
        }
        #infos label{
          color:ivory;
          display:inline-block;
          margin-top:60px;
        }
        #infos a{
          text-decoration: none;
          margin-top:5px;
          color:yellow;
        }
        #infos a:hover{
          color:gold;
        }
        #main{
          margin-top:20px;
          width:100%;
          height:550px;
          background-color: #333;
        }
        ul {
          list-style-type: none;
          margin: 0;
          padding: 0;
          overflow: hidden;
          background-color: #333;
            }
        a{
          text-decoration: none;
          color:ivory;
          display:inline;
        }
        li:hover{
          background-color: #1c2841;
          cursor: pointer;
        }
        li{
          width:90%;
          padding:20px;
          text-decoration: none;
          color:ivory;
        }
        .active{background-color: #1c2841;}
        li label{
          float: right;
        }
        .menutext{
          width:88.5%;
          display:inline-block;
          padding:20px;
          color:gray;
          background-color: #0c0c0c;
        }
        .title{
          display:inline-block;
          font-size:40px;
          margin:30px 10px 0 50px;
          color:  #4169E1;
        }
        hr{float:left; border:none; width:900px; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:50px;}
        nav{
          float:left;
          width:50%;
          margin-left:50px;
        }
        nav button{
          border:none;
          background:transparent;
          padding:0 10px;
          font-size: 15px;
          outline:none;
        }
        nav button:hover{cursor: pointer;}
        .alert{
          width:65%;
          height:25px;
          line-height: 25px;
          padding: 10px;
          background-color: #f44336;
          color: white;
          opacity: 1;
          transition: opacity 0.6s;
          float:left;
          font-size:16px;
          text-align:left;
          margin:5px 50px;
          display:none;
        }
        .alert.success{background-color: #4CAF50;}
        .alert.info{background-color: #2196F3;}
        .alert.warning{background-color: #ff9800;}
        .closebtn {
          float:right;
          color: white;
          font-weight: bold;
          float: right;
          font-size: 22px;
          line-height: 25px;
          cursor: pointer;
          transition: 0.3s;
        }
        .closebtn:hover {
          color: black;
        }
        #statbox{
          color:black;
          font-weight: bold;
          font-size: 17px;
          padding-top:8px;
          margin-right:50px;
          float:right;
        }
        #status{
            color:#00FF00;
        }
        .text {
          color: white;
          font-size:12px;
          float: right;
          margin-right: 5px;
          margin-top: 3px;
        }
        #state{
          padding: 10px 15px;
          margin:0 50px;
          float:left;
          color: #FFFFF0;
          background: #00FF00;
          border:none;
          font-weight:bold;
          text-shadow:1.2px 1px black;
          outline:none;
        }
        #state:hover{
          background:green;
          cursor: pointer;
        }
        tbody {
          display:inline-block;
          width: 100%;
          height:200px;
          overflow-y: scroll;
        } 
        .fn{width:300px;} .fs{width:150px;} .action{width:375px;}
        th{       
          text-align:left;
          padding:12px;
          color:#191970;
          background-color:#B0C4DE;
          font-weight: normal;
        }
        td{
          text-align:left;
          padding:12px;
          color:#191970;
          background-color:#F0F8FF;
          font-weight: normal;
          font-size:13px;
        }
        table {
          display:inline-block;
          border-collapse: collapse;
          margin:10px 0 0 50px;
          height:245px;
        }
        .view,.delete,.download,.send{
          background:transparent;
          color:#191970;
          border:none;
          outline:none;
        }
        .view:hover,.delete:hover,.download:hover,.send:hover{
          text-decoration: underline;
          cursor: pointer;
        }
        #modalWindow {
            position: fixed;
            top: 0;
            right: 0;
            bottom: 0;
            left: 0;
            background: rgba(0, 0, 0, 0.8);
            z-index: 9999;
            opacity:1;
            display:none;
        }
        #modalWindow:target {
            opacity:1;
            pointer-events: auto;
        }
        #modalWindow > div {
            width: 80%;
            height: 30%;
            position: relative;
            margin-top:150px;
            padding: 20px 20px 13px 20px;
            background: transparent;
        }
        #modalimg{
            position:absolute;
            max-width: 80%;
            height: 500px;
            border:1px solid rgba(0,0,0,0.1);
            top:1%;
            left: 60%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
        }
        .details{
          width:66.5%;
          height:20px;
          float:left;
          margin:20px 0 0 50px;
          font-weight: bold;
        }
        #timer{float:right;}
        #totalkb{
          color:black;
          float:left;
          margin:10px 0 0 50px;
          padding:8px;
          font-weight: bold;
        }
        #sendall{
          float:right;
          margin:10px 50px 0 0;
          padding:3px 5px;
        }
        #settings{
          float:left;
          width:66.5%;
          height:22%;
          border:1px solid transparent;
          margin:20px 0 0 50px;
          font-weight: bold;
        }
        .con{
          position:relative;
          margin:0;
          display:block;
          width:100%;
          height:30px;
          border:1px solid transparent;
        }
        #autosend,#autodelete{
          float:right;
          margin:5px;
          padding: 1px 6px;
          font-weight:bold;
        }
        .conlabel,#currentdelay{
          float:left;
          padding:5px 0 5px 5px;
          color:black;
        }
        #delay_value{
          width:12%;
          margin:5px;
          float:right;
        }
        #currentdelay{
          float:left;
          font-size:11px;
          color:#191919;
          line-height:1;
        }
        #email{
          width:55%;
          float:right;
          margin:5px;
        }
        #save,#restart{
          float:right;
          padding: 3px 15px;
          margin:-25px 50px 0 50px;
          border: 0;
          line-height: 28px;
          cursor: pointer;
          color: #fff;
          background: red;
          border-radius: 2px;
          font-size: 12px;
          outline: 0;
          text-shadow: 1px 1px black;
        }
        #validate{
          font-size:12px;
          text-shadow:1px .5px black;
          float:right;
          left:83%;
        }
        @media only screen and (max-width: 600px) {
          body,
          html {
            width:100%;
          }
          #dashboard{
            float:left;
            width:350px;
            height:600px;
            background-color: #191919;
            position:absolute;
            right:750px;
            z-index:1;
          }
          #dashboard header{
            width:100%;
            height:70px;
            background-color:#03254c; 
            text-align: left;
          }
          #dashboard p{
            color:ivory;
            display:inline-block;
            font-size: 30px;
            margin-top:20px;
            margin-left:10px;
          }
          #dashboard header span{
            float:right;
            display:block;
            margin:20px 20px;
            color:ivory;
            font-size:25px;
          }
          #content{
            width:100%;
            background-color:#F5FFFA;
          }
          #content header{
            width:100%;
            height:70px;
            background-color:#03254c;
            text-align: left;
          }
          #content header span{
            display:inline-block;
            margin:20px 20px;
            color:ivory;
            font-size:25px;
          }
          #infos{
            width:100%;
            height:20%;
          }
          #infos img{
            float:left;
            width:90px;
            height:75%;
            border-radius: 100px;
            margin:20px;
          }
          #infos label{
            color:ivory;
            display:inline-block;
            margin-top:45px;
          }
          #infos a{
            text-decoration: none;
            margin-top:5px;
            color:yellow;
          }
          #info a:hover{
            color:gold;
          }
          #main{
            margin-top:30px;
            width:100%;
            height:inherit;
            background-color: #333;
          }
          .menutext{
            width:88.5%;
            display:inline-block;
            padding:20px;
            color:gray;
            background-color: #0c0c0c;
          }
          .title{
            display:inline-block;
            font-size:40px;
            margin:30px 10px 0 10px;
            color:  #4169E1;
          }
          hr{float:left; border:none; width:95%; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:10px;}
          .alert{
            float:left;
            margin:2px 5px;
            height:10px;
            width:92%;
            padding: 10px;
            font-size:12px;
            line-height: 12px;
          }
          .closebtn {
            line-height: 10px;
          }
          #statbox{
            color:black;
            font-size: 15px;
            margin:0 10px;
          }
          #state{
            margin:0 0 5px 10px;
          }
          .details{
            width:100%;
            height:20px;
            float:left;
            margin:10px 0 20px 10px;
            font-weight: bold;
          }
          .label{display:inline-block; width:54%; padding-right: 50px;}
          #timer{margin-top: 7px; float:left;}
          tbody {
            width:99.5%;
            height:90%;
          }   
          .view,.delete,.send,.download{
            font-size:12px;
          }
          td{
            font-size:12px;
          }
          table {
            display:inline-block;
            border-collapse: collapse;
            margin:10px 0 0 5px;
            width:98%;
          }
          th.fn{width:45%;} th.fs{width:15%;} th.action{width:100%;}
            td.fn{width:45%;} td.fs{width:15%;}td.action{width:100%;}
          #totalkb{
            float:left;
            color:black;
            margin:25px 0 0 0;
            padding:8px;
            
          }
          #sendall{
            margin:30px 0 0 0;
            padding:3px 5px;
            float:right;
          }
          #settings{
            width:98%;
            height:20%;
            margin:0;
            border:1px solid transparent;
            float:left;
            margin-top:20px;
          }
          .conlabel,#currentdelay{
            padding:5px 0 5px 0px;
          }
          #save,#restart{
            margin:0 10px;
          }
          #validate{
            font-size:12px;
            position: relative;
            float:right;
          }
          #modalWindow {
              position: fixed;
              top: 0;
              right: 0;
              bottom: 0;
              left: 0;
              background: rgba(0, 0, 0, 0.8);
              z-index: 9999;
              opacity:1;
              display:none;
          }
          #modalWindow:target {
              opacity:1;
              pointer-events: auto;
          }
          #modalWindow > div {
              width: 80%;
              height: 30%;
              position: relative;
              margin-top:150px;
              padding: 20px 20px 13px 20px;
              background: transparent;
          }
          #modalimg{
              position:absolute;
              width: 100%;
              max-width: 100%;
              height: 120%;
              border:1px solid rgba(0,0,0,0.1);
              left: 55%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
          }
          #save{margin-top:-10px;}
        } 
    </style>
</head>
<body>
  <div id="container">
    <div id="dashboard">
      <header>
        <p><b>Camera</b> Server</p>
        <span id="closeslider"><i class="fa fa-bars" aria-hidden="true"></i></span>
      </header>
      <div id="infos">
        <img src="data:image/png;base64,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">
        <label><b>Admin:</b> *ADMIN*</label><br>
        <a href="#" id="logout"><i class="fa fa-cogs" aria-hidden="true"></i> Logout Account</a>
      </div>
      <div id="main">
        <label class="menutext">Dashboard</label>
        <ul>
          <a href="/"><li><i class="fa fa-tachometer" aria-hidden="true"></i> Overview <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/streaming"><li><i class="fa fa-video-camera" aria-hidden="true"></i> Stream <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/motion"><li class="active"><i class="fa fa-dot-circle-o" aria-hidden="true"></i> Motion Sensor <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/timelapse"><li><i class="fa fa-clock-o" aria-hidden="true"></i> Timelapse <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/record"><li><i class="fa fa-book" aria-hidden="true"></i> Records <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        </ul>
      </div>
    </div>
    <div id="content">
      <header>
        <span id="slider"><i class="fa fa-bars" aria-hidden="true"></i></span>
        <span id="reset"><i class="fa fa-power-off" aria-hidden="true"></i></span>
      </header>
      <label class="title">Motion Capture</label>
      <hr>
      <button id="state">Enable</button>
        <div id="statbox"><label>Status: <label id="status">Disable</label></label></div>
      <div class="alert" id="disable">
        <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Warning: </strong> 
      </div>
        <div class="alert" id="error">
          <span class="closebtn">&times;</span>  
          <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
        </div>
        <div class="alert info" id="info">
          <span class="closebtn">&times;</span>  
          Motion off, Enable it first before you can use this.
        </div>
        <div class="alert success" id="success">
          <span class="closebtn">&times;</span>  
          <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
        </div>
        <div class="alert warning" id="warning">
          <span class="closebtn">&times;</span>  
          <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
        </div>
        <div class="details">
        <label class="label">Total captured: *TOTAL_IMAGE*</label>
        <div id="timer">
          <label>Next Tilt in: </label>
          <span id="hour">00</span> :
          <span id="min">00</span> :
          <span id="sec">00</span>
        </div>
        </div>
        <table>
          <thead>
            <tr>
            <th class="fn">Image Name</th>
            <th class="fs">Size</th>
            <th class="action">Actions</th>
            </tr>
          </thead>
          *MOTION_IMAGE_LIST*
        </table>
        <label id="totalkb">Total KB used: *TOTAL_SIZE*KB</label>
        <button id="sendall">Send All Image</button>
        <div id="settings">
          <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Delete Image (when sent)</label><button id="autodelete">*AUTODELETE_STATE*</button></div>
          <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Auto Send (when started)</label><button id="autosend">*AUTOSEND_STATE*</button></div>
          <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Set Tilt Interval: </label><input type="number" id="delay_value" value="*DELAY_VALUE*"><label class="conlabel"><label id="currentdelay">Current interval: *DELAY_VALUE*secs</label></div>
          <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Email Reciever </label><input type="email" id="email" value="*RECIEPIENT*"></div> <label class="conlabel"><label id="validate"></label></div>
          <div class="con"><button id="save">Save Email</button></div>
        </div>
        <div id="modalWindow">
          <div><img src="display" id="modalimg"></div>
        </div>
    </div>
  </div>
</body>
</html>
<script>
  var hrcnt = 0; var mcnt = 0; var scnt; //capture timer var
  var sec; var min; var hour;
  var milisec = 100;
  var value = 0;
  $(window).on("load", function() { 
    $("#modalWindow").css("display", "none");$("#info").css("display", "none");$("#success").css("display", "none");$("#warning").css("display", "none");$("#error").css("display", "none"); $("#disable").css("display","none");
    var init_data = [];

    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
            init_data.push(x);
            x = "";
          }
        }
        console.log(init_data);
        value = init_data[2];
        if(init_data[0] == "ON "){
          $("#state").text("Disable");
          $("#status").css("color","#00ff00");$("#status").html("Active");
          setTimer();
          setInterval(timer, 10);
        }
        else{
          $("#state").text("Enable");
          $("#status").html("Disable");$("#status").css("color","#ff3034");
        }
        if($("#state").text() == "Enable"){
          $("#info").css("display", "block"); $("#blocked").css("display", "block"); 
        }else{
          $("#blocked").css("display", "none"); 
        }
      }
      if(init_data[1] == "ON "){
        $("#state").prop("disabled",true);
        $("#state").css("background-color","#808080");
        $("#info").css("display","none"); $("#disable").css("display","block");$("#disable").html("<i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Warning:</strong>Timelapse is enabled, it may cause malfunction");
      }
      else{
        $("#state").prop("disabled",false);
        if($("#state").text() == "Disable")
            $("#state").css("background-color","red");
        else
            $("#state").css("background-color","green");
        $("#disable").css("display","none");
      }
    }; 
    xhttp.open("GET", "/init_motion_data", true);    
    xhttp.send();
    });

    $('#modalWindow').on('click',function(e){
      $(this).css("display", "none");
    });
    
    function table(list,fsize) {
      $("tbody").append("<tr><td class='fn'>"+list+"</td><td class='fs'>"+fsize+"KB</td><td class='action'><button class='view'>View</button>|<button class='download'>Download</button>|<button class='delete'>Delete</button>|<button class='send'>Send</button></td></tr>");
    }

    $('#sendall').on('click',function(e){
    $("#info").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Preparing to send all image, Please wait!");
    $("#warning").css("display", "none"); $("#info").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      if(res == "true"){
        $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>All images are sent!");
        $("#success").css("display", "block"); $("#error").css("display", "none"); $("#warning").css("display", "none"); $("#info").css("display", "none");
        if($("#autodelete").text() == "ON")
          $("#tbody").empty();
      }
      else{
        $("#error").html("<span class='closebtn'>&times;</span> <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Error: </strong> Failed to send image, Try again!");
        $("#error").css("display", "block"); $("#success").css("display", "none"); $("#warning").css("display", "none"); $("#info").css("display", "none");
      }
      }
    }; 
    xhttp.open("GET", "/send_all", true);    
    xhttp.send();
    });
    $('#state').on('click',function(e){
    var xhttp = new XMLHttpRequest();    
    if($(this).text() == "Enable"){
      $(this).text("Disable");
      $(this).css("background-color","#ff3034");
      $("#warning").html("<span class='closebtn'>&times;</span>  <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong> Warning: </strong> Restart ESP to take changes.");
      $("#warning").css("display", "block");$("#info").css("display", "none");$("#success").css("display", "none");$("#error").css("display", "none");
      xhttp.open("GET", "/motion_sensor?value=ON", true);   
    }
    else{
      $(this).text("Enable");
      $(this).css("background-color","green");
      $("#warning").html("<span class='closebtn'>&times;</span>  <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong> Warning: </strong> Restart ESP to take changes.");
      $("#warning").css("display", "block");$("#info").css("display", "none");$("#success").css("display", "none");$("#error").css("display", "none");
      xhttp.open("GET", "/motion_sensor?value=OFF", true);   
    }
        xhttp.send();
    });
  $('#autodelete').on('click',function(e){
    var xhttp = new XMLHttpRequest();    
    if($(this).text() == "ON"){
      $(this).text("OFF");
      xhttp.open("GET", "/autodelete_state?value="+$(this).text(), true);    
    }
    else{
      $(this).text("ON");
      xhttp.open("GET", "/autodelete_state?value="+$(this).text(), true);    
    }
    xhttp.send();
    });
  $("#logout").click(function () {
    window.location.href = "/login";
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/logout", true);    
    xhttp.send();
  });
  $('table').on('click', '.download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/JPG_download?value="+n, false);    
    xhttp.send();
    
    window.location = '/JPG_download?value='+n;
  });

  $('#autosend').on('click',function(e){
    var xhttp = new XMLHttpRequest();    
    if($(this).text() == "ON"){
      $(this).text("OFF");
      xhttp.open("GET", "/auto_state?value="+$(this).text(), true);    
    }
    else{
      $(this).text("ON");
      xhttp.open("GET", "/auto_state?value="+$(this).text(), true);    
    }
    xhttp.send();
    });
  $('table').on('click', '.view', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#modalWindow").css("display", "block");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      display(res);
      }
    }; 
    xhttp.open("GET", "/view_img?value="+n, true);    
    xhttp.send();
  });
  function display(res) {
    var val = $("#modalimg").attr("src")
    $("#modalimg").attr("src", val + '?_=' + new Date().getTime());
    }
  $('table').on('click', '.delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block"); $("#info").css("display", "none"); $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/delete_img?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
  });
  $('#reset').on('click',function(e){
    $("#info").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Server restarting, Please wait!");
    $("#warning").css("display", "none"); $("#info").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");

    var xhttp = new XMLHttpRequest(); 
    xhttp.open("GET", "/restart", true);    
    xhttp.send();
  });
  $('#save').on('click',function(e){
    if(isValidEmailAddress($("#email").val())){
      $("#warning").html("<span class='closebtn'>&times;</span>  <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong> Warning: </strong> Email saved! Restart ESP to take changes.");
      $("#warning").css("display", "block");$("#info").css("display", "none");$("#success").css("display", "none");$("#error").css("display", "none");
      var xhttp = new XMLHttpRequest();    
      xhttp.open("GET", "/email_address?value="+$("#email").val(), true);    
      xhttp.send();
    }
  });
  $('table').on('click', '.send', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#info").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Sending "+n+", Please wait!");
    $("#warning").css("display", "none"); $("#info").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      if(res == "true"){
        $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" sent!");
        $("#success").css("display", "block"); $("#error").css("display", "none"); $("#warning").css("display", "none"); $("#info").css("display", "none");
        $(this).closest('tr').remove();
      }
      else{
        $("#error").html("<span class='closebtn'>&times;</span> <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Error: </strong> Failed to send image, Try again!");
        $("#error").css("display", "block"); $("#success").css("display", "none"); $("#warning").css("display", "none"); $("#info").css("display", "none");
      }
      }
    }; 
    xhttp.open("GET", "/send_img?value="+n, true);    
    xhttp.send();
  });
  $("#delay_value").bind("change paste keyup", function() {   
    $("#currentdelay").html("Current interval: "+$(this).val()+" secs");
    var xhttp = new XMLHttpRequest(); 
    xhttp.open("GET", "/set_delay_value?value="+$("#delay_value").val(), true);   
    xhttp.send();
  });
  $("#email").bind("change paste keyup", function() {   
    if(isValidEmailAddress($(this).val())){
      $("#validate").css("color","#00ff00");
      $("#validate").html("Valid email address.");
    }
    else{
      $("#validate").css("color","red");
      $("#validate").html("Invalid email address format!");
    }   
  });
  function isValidEmailAddress(emailAddress) {
    var pattern = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}$/;
    return pattern.test(emailAddress);
  };
  function setTimer(){
        while(value > 3600){hrcnt++; value -= 3600;}
        while(value > 60){mcnt++; value -= 60;}
        scnt = value; sec = scnt; min = mcnt; hour = hrcnt;
    }
    function timer(){
        milisec = --milisec;
        if (milisec === 0){milisec = 99; sec = --sec;}
        if (sec == 0) {
            if(parseInt(checkTime(min))!= 00){min = --min; sec = 60;}
            else {
                setTimer();
                var xhttp = new XMLHttpRequest(); 
                xhttp.onreadystatechange = function() {
                    if(xhttp.readyState === 4) { 
                        var res = this.responseText;
                        $("#totalcaptured").html(res);
                    }
                }
                xhttp.open("GET", "/update_capture_counter", true);    
                xhttp.send();
            }
        }
        if(min == 0) {
            if(parseInt(checkTime(hour)) != 00){min = 60; hour = --hour;}
        }
        $("#sec").html(checkTime(sec)); $("#min").html(checkTime(min)); $("#hour").html(checkTime(hour));
    }
    function checkTime(i) {
        if (i < 10){i = "0" + i;}
        return i;
    }
    $("#slider").click(function () {
      $("#dashboard").animate({ "right": 0 }, "slow");
    });
    $("#closeslider").click(function () {
          $("#dashboard").animate({ "right": 750 }, "slow");
    });
  $('body').on('click', '.closebtn', function() {
        var div = $(this).closest('div');
        $(this).closest('div').animate({opacity:0},700, () => {
            $(this).closest('div').css("display","none");
            $(this).closest('div').css("opacity","1");
        });
  });
</script>
)rawliteral";


static const char PROGMEM stream_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
  <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
  <title>Stream</title>
  <style>
    body,
        html {
      width:95%;
      margin:auto;
            font-family: sans-serif;
      margin-top:2px;
        }
    #container{
      width:95%;
      height:auto;
      position: relative;
          margin: auto;       
    }
    #dashboard{
      float:left;
      width:350px;
      height:800px;
      background-color: #191919;
    }
    #dashboard header{
      width:100%;
      height:70px;
      background-color:#03254c; 
      text-align: center;
    }
    #dashboard p{
      color:ivory;
      display:inline-block;
      font-size: 30px;
      margin-top:20px;
    }
    #dashboard header span{
      display:inline-block;
      margin:20px 5px;
      color:ivory;
      font-size:25px;
      display:none;
    }
    #dashboard header span:hover{
      cursor:pointer;
    }
    #content{
      width:1350px;
      height:800px;
      background-color:#F5FFFA;
    }
    #content header{
      width:100%;
      height:70px;
      background-color:#03254c;
      text-align: left;
    }
    #slider,#reset{
          display:inline-block;
          margin:20px 5px;
          color:ivory;
          font-size:25px;
          display:none;
        }
        #slider:hover,#reset:hover{
          cursor:pointer;
        }
        #reset{
          margin:25px;
          display:block;
          float:right;
        }
    #info{
      width:100%;
      height:20%;
    }
    #info img{
      float:left;
      width:120px;
      height:75%;
      border-radius: 100px;
      margin:20px;
    }
    #info label{
      color:ivory;
      display:inline-block;
      margin-top:60px;
    }
    #info a{
      text-decoration: none;
      margin-top:5px;
      color:yellow;
    }
    #info a:hover{
      color:gold;
    }
    #main{
      margin-top:20px;
      width:100%;
      height:550px;
      background-color: #333;
    }
    ul {
      list-style-type: none;
      margin: 0;
      padding: 0;
      overflow: hidden;
      background-color: #333;
        }
    a{
      text-decoration: none;
      color:ivory;
      display:inline;
    }
    li:hover{
      background-color: #1c2841;
      cursor: pointer;
    }
    li{
      width:90%;
      padding:20px;
      text-decoration: none;
      color:ivory;
    }
    .active{background-color: #1c2841;}
    li label{
      float: right;
    }
    .menutext{
      width:88.5%;
      display:inline-block;
      padding:20px;
      color:gray;
      background-color: #0c0c0c;
    }
    .title{
      display:inline-block;
      font-size:40px;
      margin:30px 10px 0 50px;
      color:  #4169E1;
    }
    hr{float:left; border:none; width:900px; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:50px;}
    nav{
      float:left;
      width:50%;
      margin-left:50px;
    }
    nav button{
      border:none;
      background:transparent;
      padding:0 10px;
      font-size: 15px;
      outline:none;
    }
    nav button:hover{cursor: pointer;}
    #home{height:650px;}
    #recent,#ongoing{
      float:left;
      width:40%;
      height:35%;
      margin:20px 50px;
      background-color:#b2b2b2;
      box-shadow: 2px 2px #808080;
    }
    #recent header,#ongoing header{
      float:left;
      width:100%;
      height:30%; 
    }
    .headertitle{
      display:inline-block;
      margin:25px 15px;
      color:#FFF;
      font-size: 20px;
      font-weight: bold;
      text-shadow:1px 1px black;
    }
    #recent header i,#ongoing header i{
      float:right;
      margin:20px;
      color:#FFF;
      font-size: 30px;
    }
    #ongoing label{display:inline-block;}
    .activeMode{
      font-size:20px;
      font-weight: bold;
      color:gold;
      text-shadow:2px 2px black;
      margin:20px 0;
    }
    #statbox{
      color:black;
      font-size: 19px;
      margin:5px 10px;
      font-weight: bold;
    }
    .status{color:#00FF00; text-shadow:1.5px 1px black; font-weight: normal;}
    .runningTime,#timer{
      color:black;
      margin:5px 10px;
      font-size: 19px;
      font-weight: bold;
      display:block;
    }
    .endRunningTime,#endtime{
      color:black;
      margin:0 10px;
      font-size: 19px;
      font-weight: bold;
      display:inline-block;
    }
    .captureLabel,.totalcaptureLabel{
      color:black;
      margin:5px 10px;
      font-size: 19px;
      font-weight: bold;
    }
    .totalcaptureLabel{
      display:block;
      margin:10px 10px;
    }
    #endtime, .totalcounter, .file{font-weight: normal;}
    .modeLabel,.logfile{
      margin:12px 10px;
      color:black;
      font-size: 19px;
      font-weight: bold;
      display:block;
    }
    .logfile{width:100%;}
    #timer,.counter{font-weight: normal;}
    .alert{
      width:65%;
      height:25px;
            line-height: 25px;
      padding: 10px;
      background-color: #f44336;
      color: white;
      opacity: 1;
      transition: opacity 0.6s;
      float:left;
      font-size:16px;
      text-align:left;
      margin:5px 50px;
            display:none;
    }
        #infobox{display:inline-block;}
    .alert.success{background-color: #4CAF50;}
    .alert.info{background-color: #2196F3;}
    .alert.warning{background-color: #ff9800;}
    .closebtn {
      float:right;
      color: white;
      font-weight: bold;
      float: right;
      font-size: 22px;
      line-height: 25px;
      cursor: pointer;
      transition: 0.3s;
    }
    .closebtn:hover {
      color: black;
    }
        #streambtn,#capture{
            float:left;
            padding: 0 10px;
            margin:10px 200px ;
            border: 0;
            line-height: 28px;
            cursor: pointer;
            color: #fff;
            background: #ff3034;
            border-radius: 2px;
            font-size: 12px;
            outline: 0
        }
        #capture{
            float:left;
            margin-left:50px;
        }
        #stream{
            -webkit-user-select: none;
            max-width: 100%;
            max-height: 100%;
            margin: auto;
        }
        #wrap{
            float:left;
            width:43%;
            height:54.5%;
            border:1px solid black;
            margin-left:50px;
        }
        #option{
            float:left;
            width:21%;
            height:54.5%;
            border:1px solid black;
            margin-left:5px;
            background:#E6E6FA;
        }
        #mobile{display:none}
        .servo{
            width:100%;
            height:35px;
        }
        .servo input[type=range] {
            -webkit-appearance: none;
            width: 38%;
            height: 22px;
            background: transparent;
            cursor: pointer;
            right: 58%;
            margin:7px 5px;
        }         
        #PanservoValue,
        #TiltservoValue {
            float:left;
            line-height: 40px;
            color: black;
            font-size: 16px;
            font-weight: bold;
            float: left;
        }
        #pan,#tilt {
            float:left;
            width: 30px;
            margin:5px 0 50px 50px;
            padding: 0 10px;
            line-height: 28px;
            color: #fff;
            background: #ff3034;
            font-size: 14px;
            float: left;
        }
        #TiltservoValue,#tilt,#tiltservo{
            display:none;
        }
        input[type=range] {
            float:left;
            -webkit-appearance: none;
            width: 5px;
            height: 22px;
            background: #4c4c4c;
            cursor: pointer;
            margin: 0
        }
        input[type=range]:focus {
            outline: 0
        }

        input[type=range]::-webkit-slider-runnable-track {
            width: 15%;
            height: 2px;
            cursor: pointer;
            background: black;
            border-radius: 0;
            border: 0 solid  black;
        }

        input[type=range]::-webkit-slider-thumb {
            border: 1px solid rgba(0, 0, 30, 0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #ff3034;
            cursor: pointer;
            -webkit-appearance: none;
            margin-top: -11.5px
        }

        input[type=range]:focus::-webkit-slider-runnable-track {
            background:  black;
        }

        input[type=range]::-moz-range-track {
            width: 30%;
            height: 2px;
            cursor: pointer;
            background: #EFEFEF;
            border-radius: 0;
            border: 0 solid #EFEFEF
        }

        input[type=range]::-moz-range-thumb {
            border: 1px solid rgba(0, 0, 30, 0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #ff3034;
            cursor: pointer
        }

        input[type=range]::-ms-track {
            width: 30%;
            height: 2px;
            cursor: pointer;
            background: 0 0;
            border-color: transparent;
            color: transparent;
        }

        input[type=range]::-ms-fill-lower {
            background: #EFEFEF;
            border: 0 solid #EFEFEF;
            border-radius: 0
        }

        input[type=range]::-ms-fill-upper {
            background: #EFEFEF;
            border: 0 solid #EFEFEF;
            border-radius: 0
        }

        input[type=range]::-ms-thumb {
            border: 1px solid rgba(0, 0, 30, 0);
            height: 22px;
            width: 22px;
            border-radius: 50px;
            background: #ff3034;
            cursor: pointer;
            height: 2px
        }

        input[type=range]:focus::-ms-fill-lower {
            background: #EFEFEF
        }
        #settingsHeader{
            width:100%;
            height:7%;
            background-color: #191970;
        }
        #settingsHeader label{
            display:inline-block;
            font-size:15px;
            margin:7px;
            color:ivory;
        }
        .note{
            float:left;
            color:black;
            font-size:13px;
            text-align: left;
            margin:0 5px;
            margin-top:15px;
            margin-bottom:25px;
            width:100%;
        }
        table {
            display:inline-block;
            width: 100%;
            border-collapse: collapse;
            border:none;
        }
        thead {
            color: #fff;
            background-color:#191970;
            text-shadow: 2px 2px black;
        }
        
        th{
            padding: 8px 10px;
            width:150px;
        }
        td {
            width:160px;
            text-align: center;
            font-size:15px;
        }
        tbody {
            color:black;
            height: 136.5px;
            overflow-y: scroll;
            border:1px solid transparent;
            font-weight: bold;
        }
        .view,.save,.send{
            background:transparent;
            color:black;
            border:none;
            padding:4px 6px;
            outline:none;
        }
        .view:hover,.save:hover,.send:hover{  
            text-decoration:underline;
            cursor: pointer;
        }
        #modalWindow {
            position: fixed;
            top: 0;
            right: 0;
            bottom: 0;
            left: 0;
            background: rgba(0, 0, 0, 0.8);
            z-index: 9999;
            opacity:1;
            display:none;
        }
        #modalWindow:target {
            opacity:1;
            pointer-events: auto;
        }
        #modalWindow > div {
            width: 80%;
            height: 30%;
            position: relative;
            margin-top:150px;
            padding: 20px 20px 13px 20px;
            background: transparent;
        }
        #modalimg{
            position:absolute;
            max-width: 80%;
            height: 500px;
            border:1px solid rgba(0,0,0,0.1);
            top:1%;
            left: 60%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
        }
        #settings{
            width:100%;
            height:7%;
            border:1px solid transparent;
            margin-top:30px;
        }
        .con{
            position:relative;
            margin:0;
            display:block;
            width:100%;
            height:30px;
            border:1px solid transparent;
            color:black;
        }
        .autosave,.autodelete{
            float:right;
            margin:5px;
            padding: 1px 6px;
            font-weight:bold;
        }
        .conlabel{
            float:left;
            font-size:13px;
            padding:5px 0 5px 5px;
            color:black;
            font-weight: bold;
        }
    @media only screen and (max-width: 600px) {
      body,
      html {
        width:100%;
      }
      #dashboard{
        float:left;
        width:350px;
        height:600px;
        background-color: #191919;
        position:absolute;
        right:750px;
        z-index:1;
      }
      #dashboard header{
        width:100%;
        height:70px;
        background-color:#03254c; 
        text-align: left;
      }
      #dashboard p{
        color:ivory;
        display:inline-block;
        font-size: 30px;
        margin-top:20px;
        margin-left:10px;
      }
      #dashboard header span{
        float:right;
        display:block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #content{
        width:100%;
        background-color:#F5FFFA;
      }
      #content header{
        width:100%;
        height:70px;
        background-color:#03254c;
        text-align: left;
      }
      #content header span{
        display:inline-block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #info{
        width:100%;
        height:20%;
      }
      #info img{
        float:left;
        width:90px;
        height:75%;
        border-radius: 100px;
        margin:20px;
      }
      #info label{
        color:ivory;
        display:inline-block;
        margin-top:45px;
      }
      #info a{
        text-decoration: none;
        margin-top:5px;
        color:yellow;
      }
      #info a:hover{
        color:gold;
      }
      #main{
        margin-top:30px;
        width:100%;
        height:inherit;
        background-color: #333;
      }
      .menutext{
        width:88.5%;
        display:inline-block;
        padding:20px;
        color:gray;
        background-color: #0c0c0c;
      }
      .title{
        display:inline-block;
        font-size:40px;
        margin:30px 10px 0 10px;
        color:  #4169E1;
      }
      hr{float:left; border:none; width:95%; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:10px;}
      .alert {
              float:left;
              margin:2px 0;
              height:10px;
        width:94%;
              padding: 10px;
              font-size:12px;
              line-height: 12px;
      }
            .closebtn {
        line-height: 12px;
      }
            #option{display:none;}
            #mobile{display:block}
            #streambtn,#capture{
                float:right;
                padding: 0 5px;
                margin:5px 0;
                border: 0;
                line-height: 28px;
                cursor: pointer;
                color: #fff;
                background: #ff3034;
                border-radius: 2px;
                font-size: 12px;
                outline: 0
            }
            #capture{
                float:left;
            }
            #stream{
                -webkit-user-select: none;
                max-width: 100%;
                margin: auto;
            }
            #wrap{
                float:left;
                width:100%;
                height:32.1%;
                border:1px solid black;
                margin:auto;
            }
            .servo{
                width:100%;
                height:35px;
            }
            .servo input[type=range] {
                -webkit-appearance: none;
                width: 72%;
                height: 22px;
                background: transparent;
                cursor: pointer;
                right: 58%;
                margin-top: 7px;
            }         
            #PanservoValue,
            #TiltservoValue {
                float:left;
                line-height: 40px;
                margin:0 0;
                color: black;
                font-size: 16px;
                font-weight: bold;
                float: left;
            }
            #pan,#tilt {
                float:left;
                width: 30px;
                margin: 5px 1px;
                padding: 0 7px;
                line-height: 28px;
                color: #fff;
                background: #ff3034;
                font-size: 14px;
                float: left;
            }
            input[type=range] {
                float:left;
                -webkit-appearance: none;
                width: 100px;
                height: 22px;
                background: #4c4c4c;
                cursor: pointer;
                margin: 0
            }
            .note{
                color:black;
                font-size:12px;
                margin:0 5px;
                margin-top:15px;
                margin-bottom:5px;
            } 
            th{
                padding: 8px 10px;
                width:150px;
            }
            td {
                font-weight: bold;
            }
            thead{
                background-color:red;
            }
            tbody {
                color:black;
            }
            .view,.save,.send{
                color:black;
                text-shadow: 0px 0px black;
                font-weight: bold;
            }
            #modalWindow {
                position: fixed;
                top: 0;
                right: 0;
                bottom: 0;
                left: 0;
                background: rgba(0, 0, 0, 0.8);
                z-index: 9999;
                opacity:1;
            }
            #modalWindow:target {
                opacity:1;
                pointer-events: auto;
            }
            #modalWindow > div {
                width: 80%;
                height: 30%;
                position: relative;
                margin-top:150px;
                padding: 20px 20px 13px 20px;
                background: transparent;
            }
            #modalimg{
                position:absolute;
                width: 100%;
                max-width: 100%;
                height: 120%;
                border:1px solid rgba(0,0,0,0.1);
                left: 55%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
            }
            .con{
                color:black;
            }
            .conlabel{
                color:black;
                font-weight: bold;
            }
            #reset{
                margin:20px 5px;
            }
    }
  </style>
</head>
<body>
  <div id="container">
  <div id="dashboard">
    <header>
      <p><b>Camera</b> Server</p>
      <span id="closeslider"><i class="fa fa-bars" aria-hidden="true"></i></span>
    </header>
    <div id="info">
      <img src="data:image/png;base64,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">
      <label><b>Admin:</b> *ADMIN*</label><br>
      <a href="#" id="logout"><i class="fa fa-cogs" aria-hidden="true"></i> Logout Account</a>
    </div>
    <div id="main">
      <label class="menutext">Dashboard</label>
      <ul>
        <a href="/"><li><i class="fa fa-tachometer" aria-hidden="true"></i> Overview <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/streaming"><li class="active"><i class="fa fa-video-camera" aria-hidden="true"></i> Stream <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/motion"><li><i class="fa fa-dot-circle-o" aria-hidden="true"></i> Motion Sensor <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/timelapse"><li><i class="fa fa-clock-o" aria-hidden="true"></i> Timelapse <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        <a href="/record"><li><i class="fa fa-book" aria-hidden="true"></i> Records <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
      </ul>
    </div>
  </div>
  <div id="content">
    <header>
            <span id="slider"><i class="fa fa-bars" aria-hidden="true"></i></span>
            <span id="reset"><i class="fa fa-power-off" aria-hidden="true"></i></span>
    </header>
    <label class="title">Stream</label>
        <hr>
          <div class="alert" id="disable">
            <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Warning: </strong> Motion is enabled, SD card unavailable
          </div>
          <div class="alert" id="error">
            <span class="closebtn">&times;</span>  
            <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
          </div>
          <div class="alert info" id="infobox">
            <span class="closebtn">&times;</span>  
            <i class="fa fa-info-circle" aria-hidden="true"></i><strong> Info:</strong> Camera ready! Use stream to view.
          </div>
          <div class="alert success" id="success">
            <span class="closebtn">&times;</span>  
            <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
          </div>
          <div class="alert warning" id="warning">
            <span class="closebtn">&times;</span>  
            <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
           </div>
        <button id="capture"><i class="fa fa-camera" aria-hidden="true"></i> Capture</button>
        <button id="streambtn"><i class="fa fa-video-camera" aria-hidden="true"></i> Start Stream</button>
        <div id="wrap">
            <div id="stream-container" class="image-container hidden"></div>
                <img src="" id="stream">
             </div>
             <div id="option">
                 <div id="settingsHeader"><label><i class="fa fa-cog" aria-hidden="true"></i> Settings</label></div>
                <label class="note"><b>Note:</b> [Save] is for SD card and [Send] is for gmail recipient</label>
                <table>
                    <thead>
                        <tr>
                            <th>Image</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody id="tbody">
                        <tr id="captured">
                            <td class="filename">/captured.jpg</td>
                            </td><td><button class='view'>View</button><span class="y">|</span><button class='save'>Save</button><span class='x'>|</span><button class='send'>Send</button></td>
                        </tr>
                    </tbody>
                </table>
                <div id="settings">
                    <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Delete Image (when sent or save)</label><button class="autodelete">*AUTODELETE_STATE*</button></div>
                    <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Auto Save (when captured)</label><button class="autosave">*AUTOSAVE_STATE*</button></div>
                </div>
            </div>
            <div class="servo">
                <label id="pan">Pan</label>
                <input type="range" id="panservo" min="45" max="135" value="*PAN*" class="default-action">
                <label id="PanservoValue">*PAN*&deg</label>
                <label id="tilt">Tilt</label>
                <input type="range" id="tiltservo" min="0" max="180" value="*TILT*" class="default-action">
                <label id="TiltservoValue">*TILT*&deg</label>
            </div>
            <div id="mobile">
            <label class="note"><b>Note:</b> [Save] is for SD card and [Send] is for gmail recipient</label>
            <table>
                <thead>
                    <tr>
                        <th>Image</th>
                        <th>Actions</th>
                    </tr>
                </thead>
                <tbody id="tbody">
                    <tr id="captured">
                        <td class="filename">/captured.jpg</td>
                        </td><td><button class='view'>View</button><span class="y">|</span><button class='save'>Save</button><span class='x'>|</span><button class='send'>Send</button></td>
                    </tr>
                </tbody>
            </table>
            <div id="settings">
                <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Delete Image (when sent or save)</label><button class="autodelete">*AUTODELETE_STATE*</button></div>
                <div class="con"><label class="conlabel"><i class="fa fa-chevron-right" aria-hidden="true"></i> Auto Save (when captured)</label><button class="autosave">*AUTOSAVE_STATE*</button></div>
            </div>
            </div>
            <div id="modalWindow">
                <div><img src="display" id="modalimg"></div>
            </div>
        </div>
    </div>
    </div>
</body>
</html>
<script>
    var c = document.location.origin;
    var init_data = [];
    var motion_state;
    $(window).on("load", function() { 
    $("#info").css("display", "block");$("#success").css("display", "none");$("#warning").css("display", "none");$("#error").css("display", "none");
        $("#tilt").css("display","none"); $("#tiltservo").css("display","none"); $("#TiltservoValue").css("display","none");

        $("#captured").css("opacity", "1");

        if($(".autosave").text() == "ON" && $('#autosave').text() == "ON"){
            $(".y").css("display","none"); $(".send").prop("disabled",true); $(".send").css("display","none");
        }
        if($('.autosave').text() == "ON"){
            $(".x").css("display","none"); $(".save").prop("disabled",true); $(".save").css("display","none");
        }
        var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      var x = "";
      for(var i = 0; i < res.length; i++){
        x += res.charAt(i);
        if(res.charAt(i) == " "){
          init_data.push(x);
          x = "";
        }
      }
      console.log(init_data);
      if(init_data[0] == "ON "){
                $("#disable").html("<i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Warning: </strong> Timelapse is enabled, it may cause malfunction");
                $(".x").css("display","none"); $(".send").css("display","none");
      }
      else if(init_data[1] == "ON "){
                $("#disable").html("<i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Warning: </strong> Motion is enabled, SD card unavailable");
                $(".y").css("display","none"); $(".save").css("display","none");
                motion_state = true;
            }
            else
                $("#disable").css("display","none")
          } 
        }
    xhttp.open("GET", "/init_stream_data", true);    
    xhttp.send();
    });
    $('#capture').on('click',function(e){
    $("#infobox").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Capturing image, Please wait!");
        $("#warning").css("display", "none"); $("#infobox").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");
        var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      if(res == "true"){
        $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: Image captured!");
                $("#success").css("display", "block"); $("#error").css("display", "none"); $("#warning").css("display", "none"); $("#infobox").css("display", "none");
                $("#captured").css("opacity", "1");
      }
      else{
        $("#error").html("<span class='closebtn'>&times;</span> <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Error: </strong> Failed to capture image on time!");
                $("#error").css("display", "block"); $("#success").css("display", "none"); $("#warning").css("display", "none"); $("#infobox").css("display", "none");
      }
      }
    }; 
    xhttp.open("GET", "/capture", true);      
    xhttp.send();
   });
     $('table').on('click', '.view', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".filename").text();
    $("#modalWindow").css("display", "block");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      display(res);
      }
    }; 
    xhttp.open("GET", "/view_img?value="+n, true);    
    xhttp.send();
    });
  function display(res) {
    var val = $("#modalimg").attr("src")
    $("#modalimg").attr("src", val + '?_=' + new Date().getTime());
  }
  $("#logout").click(function () {
    window.location.href = "/login";
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/logout", true);    
    xhttp.send();
  });
    $('table').on('click', '.save', function() {
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      if(res == "true"){
        $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>Image save to SD Card");
        $("#success").css("display", "block"); $("#error").css("display", "none"); $("#warning").css("display", "none"); 
      }
      else{
        $("#error").html("<span class='closebtn'>&times;</span> <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Error: </strong> There was a problem saving Image to SD Card!");
        $("#error").css("display", "block"); $("#success").css("display", "none"); $("#warning").css("display", "none"); $("#info").css("display", "none");
      }
      }
    }; 
        xhttp.open("GET", "/save", true);   
        xhttp.send();
    });
    $('table').on('click', '.send', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".filename").text();
    $("#infobox").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Sending "+n+", Please wait!");
    $("#warning").css("display", "none"); $("#infobox").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      if(res == "true"){
        $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" sent!");
        $("#success").css("display", "block"); $("#error").css("display", "none"); $("#warning").css("display", "none"); $("#infobox").css("display", "none");
      }
      else{
        $("#error").html("<span class='closebtn'>&times;</span> <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong>Error: </strong> Failed to send image, Try again!");
        $("#error").css("display", "block"); $("#success").css("display", "none"); $("#warning").css("display", "none"); $("#infobox").css("display", "none");
      }
      }
    }; 
    xhttp.open("GET", "/send_img?value="+n, true);    
    xhttp.send();
  });
    $("#streambtn").on('click',function(e){ 
        if($(this).text() == " Start Stream"){
            $(this).html("<i class='fa fa-ban' aria-hidden='true'></i> Stop Stream");
            $("#stream").attr("src",`${c}/stream`); 
        }
        else{
            $(this).html("<i class='fa fa-video-camera' aria-hidden='true'></i> Start Stream");
            $("#stream").attr("src",``); 
        }
    });
    $('#modalWindow').on('click',function(e){
    $(this).css("display", "none");
    });
    $("#pan").on('click',function(e){
        $("#pan").css("display","none"); $("#panservo").css("display","none"); $("#PanservoValue").css("display","none");
        $("#tilt").css("display","block"); $("#tiltservo").css("display","block"); $("#TiltservoValue").css("display","block");
    });
    $("#tilt").on('click',function(e){
        $("#tilt").css("display","none"); $("#tiltservo").css("display","none"); $("#TiltservoValue").css("display","none");
        $("#pan").css("display","block"); $("#panservo").css("display","block"); $("#PanservoValue").css("display","block");
    });
    $("#panservo").on('change',function(e){
        $("#PanservoValue").html($(this).val()+"&deg");
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/panservo?value="+$(this).val(), true);   
    xhttp.send();
    });
    $("#tiltservo").on('change',function(e){
        $("#TiltservoValue").html($(this).val()+"&deg");
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/tiltservo?value="+$(this).val(), true);   
    xhttp.send();
    });
    $('.autodelete').on('click',function(e){
    var xhttp = new XMLHttpRequest();    
    if($(this).text() == "ON"){
      $(".autodelete").text("OFF");
      xhttp.open("GET", "/autodelete_state?value="+$(this).text(), true);    
    }
    else{
            $(".autodelete").text("ON");
      xhttp.open("GET", "/autodelete_state?value="+$(this).text(), true);    
        }
        if($(this).text() == "ON" && $('.autosave').text() == "ON"){
            $(".x").css("display","none"); $(".send").prop("disabled",true); $(".send").css("display","none");
        }
        else{
            $(".x").css("display","inline"); $(".send").prop("disabled",false); $(".send").css("display","inline");
        }
    xhttp.send();
      });
    $('#reset').on('click',function(e){
        $("#infobox").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Server restarting, Please wait!");
        $("#warning").css("display", "none"); $("#infobox").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");

        var xhttp = new XMLHttpRequest(); 
        xhttp.open("GET", "/restart", true);    
        xhttp.send();
    });
    $('.autosave').on('click',function(e){
    var xhttp = new XMLHttpRequest();    
    if($(this).text() == "ON" ){
          $(".autosave").text("OFF"); 
          if(!motion_state){
            $(".y").css("display","inline"); $(".save").prop("disabled",false); $(".save").css("display","inline");
          }
    }
    else{
            $(".autosave").text("ON"); 
          if(!motion_state){
            $(".y").css("display","none"); $(".save").prop("disabled",true); $(".save").css("display","none");
          }
        }  
        if($(this).text() == "ON" && $('.autodelete').text() == "ON"){
            $(".x").css("display","none"); $(".send").prop("disabled",true); $(".send").css("display","none");
        }
        else{
            $(".x").css("display","inline"); $(".send").prop("disabled",false); $(".send").css("display","inline");
        }
    xhttp.open("GET", "/autosave?value="+$(this).text(), true); 
    xhttp.send();
    });
      $("#slider").click(function () {
    $("#dashboard").animate({ "right": 0 }, "slow");
      });
      $("#closeslider").click(function () {
            $("#dashboard").animate({ "right": 750 }, "slow");
      });
      $('body').on('click', '.closebtn', function() {
        var div = $(this).closest('div');
        $(this).closest('div').animate({opacity:0},700, () => {
            $(this).closest('div').css("display","none");
            $(this).closest('div').css("opacity","1");
        });
    });
</script>
)rawliteral";


static const char PROGMEM timelapse_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
    <title>Timelapse</title>
    <style>
       body,
        html {
      width:95%;
      margin:auto;
            font-family: sans-serif;
      margin-top:2px;
        }
    #container{
      width:95%;
      height:auto;
      position: relative;
          margin: auto;       
    }
    #dashboard{
      float:left;
      width:350px;
      height:800px;
      background-color: #191919;
    }
    #dashboard header{
      width:100%;
      height:70px;
      background-color:#03254c; 
      text-align: center;
    }
    #dashboard p{
      color:ivory;
      display:inline-block;
      font-size: 30px;
      margin-top:20px;
    }
    #dashboard header span{
      display:inline-block;
      margin:20px 5px;
      color:ivory;
      font-size:25px;
      display:none;
    }
    #dashboard header span:hover{
      cursor:pointer;
    }
    #content{
      width:1350px;
      height:800px;
      background-color:#F5FFFA;
    }
    #content header{
      width:100%;
      height:70px;
      background-color:#03254c;
      text-align: left;
    }
    #slider,#reset{
          display:inline-block;
          margin:20px 5px;
          color:ivory;
          font-size:25px;
          display:none;
        }
        #slider:hover,#reset:hover{
          cursor:pointer;
        }
        #reset{
          margin:25px;
          display:block;
          float:right;
        }
    #info{
      width:100%;
      height:20%;
    }
    #info img{
      float:left;
      width:120px;
      height:75%;
      border-radius: 100px;
      margin:20px;
    }
    #info label{
      color:ivory;
      display:inline-block;
      margin-top:60px;
    }
    #info a{
      text-decoration: none;
      margin-top:5px;
      color:yellow;
    }
    #info a:hover{
      color:gold;
    }
    #main{
      margin-top:20px;
      width:100%;
      height:550px;
      background-color: #333;
    }
    ul {
      list-style-type: none;
      margin: 0;
      padding: 0;
      overflow: hidden;
      background-color: #333;
        }
    a{
      text-decoration: none;
      color:ivory;
      display:inline;
    }
    li:hover{
      background-color: #1c2841;
      cursor: pointer;
    }
    li{
      width:90%;
      padding:20px;
      text-decoration: none;
      color:ivory;
    }
    .active{background-color: #1c2841;}
    li label{
      float: right;
    }
    .menutext{
      width:88.5%;
      display:inline-block;
      padding:20px;
      color:gray;
      background-color: #0c0c0c;
    }
    .title{
      display:inline-block;
      font-size:40px;
      margin:30px 10px 0 50px;
      color:  #4169E1;
    }
    hr{float:left; border:none; width:900px; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:50px;}
        .alert{
      width:65%;
      height:25px;
            line-height: 25px;
      padding: 10px;
      background-color: #f44336;
      color: white;
      opacity: 1;
      transition: opacity 0.6s;
      float:left;
      font-size:16px;
      text-align:left;
      margin:5px 50px;
            display:none;
    }
    .alert.success{background-color: #4CAF50;}
    .alert.info{background-color: #2196F3;}
    .alert.warning{background-color: #ff9800;}
    .closebtn {
      float:right;
      color: white;
      font-weight: bold;
      float: right;
      font-size: 22px;
      line-height: 25px;
      cursor: pointer;
      transition: 0.3s;
    }
    .closebtn:hover {
      color: black;
    }
        .note{
            float:left;
            margin:5px 50px;
            color:black;
            font-size:15px;
            width:65.5%;
        }
        #timelapse{
            float:left;
            padding: 5px 10px;
            margin:10px 0 10px 50px;
            border: 0;
            line-height: 28px;
            cursor: pointer;
            color: #fff;
            background: #ff3034;
            border-radius: 2px;
            font-size: 14px;
            outline: 0;
            text-shadow: 1px 1px black;
        }
        #statbox{
            color:black;
            font-size: 15px;
            padding-top:8px;
            float:right;
            margin:5px 50px;
            font-weight: bold;
        }
        #status{
            color:#ff3034;
        }
        #timer{width:65.5%; display:inline; float:left; margin: 30px 0 0 50px; color:black;}
        #timer span{
            font-weight: bold;               
        }
        #timer label{
            color:black;
            font-size:15px;
            line-height: 15px;
        }
        .totalimg{
            float:left;
            margin:10px 5px;
            color:black;
            font-weight: bold;
            margin:15px 50px;
        }
        #settings{width:67.5%; margin:20px 0 0 50px; float:left;
            display:block;}
        .con{
            float:left;
            display:block;
            width:100%;
            height:30px;
            border:1px solid transparent;
        }
        .conlabel{
            float:left;
            color:black;
            margin:5px;
            line-height: 20px;
            font-weight:bold;
        }
        #interval{
            float:right;
            margin:5px;
            width:100px;
            font-weight:bold;
        }
        #folder{float:right; margin:5px; width:250px; font-weight:bold;}
        #currentInterval{
            float:left; display:inline; line-height: 32px; color:#191919; font-size: 12px;
        }
        #save{
            float:right;
            padding: 0 10px;
            margin:5px;
            margin-top:20px;
            border: 0;
            line-height: 28px;
            cursor: pointer;
            color: #fff;
            background: #ff3034;
            border-radius: 2px;
            font-size: 12px;
            outline: 0;
            text-shadow: 1px 1px black;
        }
        @media only screen and (max-width: 600px) {
      body,
      html {
        width:100%;
      }
      #dashboard{
        float:left;
        width:350px;
        height:600px;
        background-color: #191919;
        position:absolute;
        right:750px;
        z-index:1;
      }
      #dashboard header{
        width:100%;
        height:70px;
        background-color:#03254c; 
        text-align: left;
      }
      #dashboard p{
        color:ivory;
        display:inline-block;
        font-size: 30px;
        margin-top:20px;
        margin-left:10px;
      }
      #dashboard header span{
        float:right;
        display:block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #content{
        width:100%;
        background-color:#F5FFFA;
      }
      #content header{
        width:100%;
        height:70px;
        background-color:#03254c;
        text-align: left;
      }
      #content header span{
        display:inline-block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #info{
        width:100%;
        height:20%;
      }
      #info img{
        float:left;
        width:90px;
        height:75%;
        border-radius: 100px;
        margin:20px;
      }
      #info label{
        color:ivory;
        display:inline-block;
        margin-top:45px;
      }
      #info a{
        text-decoration: none;
        margin-top:5px;
        color:yellow;
      }
      #info a:hover{
        color:gold;
      }
      #main{
        margin-top:30px;
        width:100%;
        height:inherit;
        background-color: #333;
      }
      .menutext{
        width:88.5%;
        display:inline-block;
        padding:20px;
        color:gray;
        background-color: #0c0c0c;
      }
      .title{
        display:inline-block;
        font-size:40px;
        margin:30px 10px 0 10px;
        color:  #4169E1;
      }
      hr{float:left; border:none; width:95%; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:10px;}
      .alert {
              float:left;
              margin:2px 0;
              height:10px;
        width:94%;
              padding: 10px;
              font-size:12px;
              line-height: 12px;
      }
            .closebtn {
        line-height: 12px;
      }
            #timelapse{
                float:left;
                padding: 3px 10px;
                margin:10px 0 0 10px;
            }
            #statbox{
        float:right;
                margin:5px 10px 0 0;
      }
            .note{
                width:100%;
                margin:5px 10px;
                font-size:13px;
            }
            #timer{width:100%; margin:35px 0 0 10px;}
            .totalimg{
                margin:10px 10px;
            }
            #settings{width:100%; margin:20px 0;float:left;
        display:block;}
            .con{
        float:left;
        display:block;
        width:100%;
        height:30px;
        border:1px solid transparent;
      }
            .conlabel{
        float:left;
                margin:5px;
                line-height: 20px;
      }
            #interval{
                float:right;
                margin:5px;
                width:100px;
                font-weight:bold;
            }
            #folder{float:right; margin:5px; width:150px; font-weight:bold;}
            #currentInterval{
                float:left; display:inline; line-height: 32px; font-size: 12px;
            }
            #save{
                float:right;
                padding: 0 10px;
                margin:5px;
                margin-top:20px;
                border: 0;
                line-height: 28px;
                cursor: pointer;
                background: #ff3034;
                border-radius: 2px;
                font-size: 12px;
                outline: 0;
                text-shadow: 1px 1px black;
            }
        }
    </style>
</head>
<body>
    <div id="container">
        <div id="dashboard">
            <header>
                <p><b>Camera</b> Server</p>
                <span id="closeslider"><i class="fa fa-bars" aria-hidden="true"></i></span>
            </header>
            <div id="info">
                <img src="data:image/png;base64,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">
                <label><b>Admin:</b> *ADMIN*</label><br>
                <a href="#" id="logout"><i class="fa fa-cogs" aria-hidden="true"></i> Logout Account</a>
            </div>
            <div id="main">
                <label class="menutext">Dashboard</label>
                <ul>
                    <a href="/"><li><i class="fa fa-tachometer" aria-hidden="true"></i> Overview <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
                    <a href="/streaming"><li><i class="fa fa-video-camera" aria-hidden="true"></i> Stream <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
                    <a href="/motion"><li><i class="fa fa-dot-circle-o" aria-hidden="true"></i> Motion Sensor <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
                    <a href="/timelapse"><li class="active"><i class="fa fa-clock-o" aria-hidden="true"></i> Timelapse <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
                    <a href="/record"><li><i class="fa fa-book" aria-hidden="true"></i> Records <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
                </ul>
            </div>
        </div>
        <div id="content">
            <header>
                <span id="slider"><i class="fa fa-bars" aria-hidden="true"></i></span>
                <span id="reset"><i class="fa fa-power-off" aria-hidden="true"></i></span>
            </header>
            <label class="title">Timelapse</label>
            <hr>
             <label class="note"><b>Note: </b>Timelapse will run upon device has started</label>
              <div class="alert" id="disable">
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Warning: </strong> Motion is enabled, SD card unavailable
              </div>
              <div class="alert" id="error">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
              </div>
              <div class="alert info" id="infobox">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-info-circle" aria-hidden="true"></i><strong> Info:</strong> Camera ready! Use stream to view.
              </div>
              <div class="alert success" id="success">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
              </div>
              <div class="alert warning" id="warning">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
               </div>
            <div id="statbox"><label>Status: <label id="status">Inactive</label></label></div>
            <button id="timelapse">Start Timelapse</button>
            <div id="timer">
                <label>Next capture in: </label>
                <span id="hour">00</span> :
                <span id="min">00</span> :
                <span id="sec">00</span>
            </div>
            <label class="totalimg">Total image captured: <label id="totalcaptured">*TOTAL_CAPTURED*</label></label>
            <div id="settings">
                <div class="con"><label class="conlabel">Capture interval: </label>
                    <div id="currentInterval">
                        <span id="currenthour">00</span> :
                        <span id="currentmin">00</span> :
                        <span id="currentsec">00</span>
                    </div>
                    <input type="number" id="interval"></div>
                <div class="con"><label class="conlabel">Folder name: </label><input type="text" id="folder" placeholder="*FOLDER_NAME*"></div>
                <div class="con"><button id="save">Save changes</button></div>
            </div>
        </div>
    </div>
</body>
</html>
<script>
    var hrcnt = 0; var mcnt = 0; var scnt; //capture timer var
    var sec; var min; var hour;
    var currentInterval = 0;
    var value = 0;
     $(window).on("load", function() { 
    $("#infobox").css("display", "none");$("#success").css("display", "none");$("#warning").css("display", "none");$("#error").css("display", "none");
        var init_data = []; //0 runningtime 1capturedtimer 2total image 3timelapse state 4motionstate
        var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
      if(xhttp.readyState === 4) { 
      var res = this.responseText;
      var x = "";
      for(var i = 0; i < res.length; i++){
        x += res.charAt(i);
        if(res.charAt(i) == " "){
          init_data.push(x);
          x = "";
        }
      }
      console.log(init_data);
            currentInterval = parseInt(init_data[0]);
            current_interval(currentInterval);
            value = parseInt(init_data[3]);
            if(init_data[1] == "ON "){
                $("#timelapse").html("Stop Timelapse"); 
                $("#timelapse").css("background-color","#ff3034"); 
                $("#status").html("Running"); $("#status").css("color","#00FF00");
                setTimer();
                setInterval(timer, 10);
            }
            else {
                $("#timelapse").html("Start Timelapse");
                $("#timelapse").css("background-color","green"); 
                $("#status").html("Inactive"); $("#status").css("color","#FF0000");
            }
            if(init_data[2] == "ON "){
                $("#disable").css("display","block");
                $("#timelapse").css("background-color","#808080"); 
                $("#timelapse").prop('disabled', true);
            }
            else {$("#disable").css("display","none"); $('#timelapse').prop("disabled",false);}
            if(currentInterval == 0){$('#save').prop("disabled",true); $('#save').css("background-color","#808080"); $("#timelapse").prop("disabled",true); $("#timelapse").css("background-color","#808080");}
            else{$('#save').prop("disabled",false); $('#save').css("background-color","#ff3034");}
            value = parseInt(init_data[0]);
          } 
        }
    xhttp.open("GET", "/init_timelapse_data", true);    
    xhttp.send();
     });
     $('body').on('click', '.closebtn', function() {
        var div = $(this).closest('div');
        $(this).closest('div').animate({opacity:0},700, () => {
            $(this).closest('div').css("display","none");
            $(this).closest('div').css("opacity","1");
        });
    });
    $('#timelapse').on('click',function(e){
        var xhttp = new XMLHttpRequest();  
    if($(this).text() == "Start Timelapse"){
            $(this).html("Stop Timelapse")
            $(this).css("background-color","#ff3034")
            xhttp.open("GET", "/timelapse_state?value=ON", true);   
        }
        else{
            $(this).html("Start Timelapse")
            $(this).css("background-color","green")
            xhttp.open("GET", "/timelapse_state?value=OFF", true);   
        }
        $("#warning").html("<span class='closebtn'>&times;</span>  <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong> Warning: </strong> Restart ESP to take changes.");
    $("#warning").css("display", "block");$("#infobox").css("display", "none");$("#success").css("display", "none");$("#error").css("display", "none");   
    xhttp.send();
    });
    $('#save').on('click',function(e){
        $("#warning").html("<span class='closebtn'>&times;</span>  <i class='fa fa-exclamation-triangle' aria-hidden='true'></i> <strong> Warning: </strong> Saved! Restart ESP to take changes.");
        $("#warning").css("display", "block");$("#infobox").css ("display", "none");$("#success").css("display", "none");$("#error").css("display", "none");
        if($("#interval").val() != ""){
            var xhttp = new XMLHttpRequest(); 
            xhttp.open("GET", "/set_timer_value?value="+$("#interval").val(), true);   
            xhttp.send();
            $("#timelapse").prop("disabled",false);
        }
        if($("#folder").val() != ""){
            var xhttp = new XMLHttpRequest(); 
            xhttp.open("GET", "/timelapse_folder?value="+$("#folder").val(), true);   
            xhttp.send();
        }
    });
    $("#logout").click(function () {
      window.location.href = "/login";
      var xhttp = new XMLHttpRequest();    
      xhttp.open("GET", "/logout", true);    
      xhttp.send();
    });
    $("#interval").bind("change paste keyup", function() {   
        current_interval($(this).val())
        if($(this).val() != ""){
            $('#save').prop("disabled",false); $('#save').css("background-color","#ff3034");
        }
        else{
            $('#save').prop("disabled",true); $('#save').css("background-color","#808080");
        }
  });
    $('#reset').on('click',function(e){
    $("#infobox").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Server restarting, Please wait!");
    $("#warning").css("display", "none"); $("#infobox").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");

    var xhttp = new XMLHttpRequest(); 
    xhttp.open("GET", "/restart", true);    
    xhttp.send();
  });
    var milisec = 100;
    function current_interval(e){
        var interval = e; var currentsec; var currentmin = 0; var currenthour = 0;
        while(interval > 3599){currenthour++; interval -= 3600;}
        while(interval > 59){currentmin++; interval -= 60;}
        currentsec = interval;
        $("#currenthour").html(checkTime(currenthour)); $("#currentmin").html(checkTime(currentmin));  $("#currentsec").html(checkTime(currentsec)) 
    }
    function setTimer(){
        while(value > 3600){hrcnt++; value -= 3600;}
        while(value > 60){mcnt++; value -= 60;}
        scnt = value; sec = scnt; min = mcnt; hour = hrcnt;
    }
    function timer(){
        milisec = --milisec;
        if (milisec === 0){milisec = 99; sec = --sec;}
        if (sec == 0) {
            if(parseInt(checkTime(min))!= 00){min = --min; sec = 60;}
            else {
                setTimer();
                var xhttp = new XMLHttpRequest(); 
                xhttp.onreadystatechange = function() {
                    if(xhttp.readyState === 4) { 
                        var res = this.responseText;
                        $("#totalcaptured").html(res);
                    }
                }
                xhttp.open("GET", "/update_capture_counter", true);    
                xhttp.send();
            }
        }
        if(min == 0) {
            if(parseInt(checkTime(hour)) != 00){min = 60; hour = --hour;}
        }
        $("#sec").html(checkTime(sec)); $("#min").html(checkTime(min)); $("#hour").html(checkTime(hour));
    }
    function checkTime(i) {
        if (i < 10){i = "0" + i;}
        return i;
    }
    $("#slider").click(function () {
    $("#dashboard").animate({ "right": 0 }, "slow");
      });
      $("#closeslider").click(function () {
            $("#dashboard").animate({ "right": 750 }, "slow");
      });
</script>
)rawliteral";


static const char PROGMEM face_html[] = R"rawliteral(

)rawliteral";

static const char PROGMEM record_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" type="text/css" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
  <script src="https://code.jquery.com/jquery-3.2.0.min.js"></script>
  <title>Records</title>
  <style>
    body,
        html {
      width:95%;
      margin:auto;
            font-family: sans-serif;
      margin-top:2px;
        }
    #container{
      width:95%;
      height:auto;
      position: relative;
          margin: auto;       
    }
    #dashboard{
      float:left;
      width:350px;
      height:800px;
      background-color: #191919;
    }
    #dashboard header{
      width:100%;
      height:70px;
      background-color:#03254c; 
      text-align: center;
    }
    #dashboard p{
      color:ivory;
      display:inline-block;
      font-size: 30px;
      margin-top:20px;
    }
    #dashboard header span{
      display:inline-block;
      margin:20px 5px;
      color:ivory;
      font-size:25px;
      display:none;
    }
    #dashboard header span:hover{
      cursor:pointer;
    }
    #content{
      width:1350px;
      height:800px;
      background-color:#F5FFFA;
    }
    #content header{
      width:100%;
      height:70px;
      background-color:#03254c;
      text-align: left;
    }
    #slider,#reset{
          display:inline-block;
          margin:20px 5px;
          color:ivory;
          font-size:25px;
          display:none;
        }
        #slider:hover,#reset:hover{
          cursor:pointer;
        }
        #reset{
          margin:25px;
          display:block;
          float:right;
        }
    #info{
      width:100%;
      height:20%;
    }
    #info img{
      float:left;
      width:120px;
      height:75%;
      border-radius: 100px;
      margin:20px;
    }
    #info label{
      color:ivory;
      display:inline-block;
      margin-top:60px;
    }
    #info a{
      text-decoration: none;
      margin-top:5px;
      color:yellow;
    }
    #info a:hover{
      color:gold;
    }
    #main{
      margin-top:20px;
      width:100%;
      height:550px;
      background-color: #333;
    }
    ul {
      list-style-type: none;
      margin: 0;
      padding: 0;
      overflow: hidden;
      background-color: #333;
        }
    a{
      text-decoration: none;
      color:ivory;
      display:inline;
    }
    li:hover{
      background-color: #1c2841;
      cursor: pointer;
    }
    li{
      width:90%;
      padding:20px;
      text-decoration: none;
      color:ivory;
    }
    .active{background-color: #1c2841;}
    li label{
      float: right;
    }
    .menutext{
      width:88.5%;
      display:inline-block;
      padding:20px;
      color:gray;
      background-color: #0c0c0c;
    }
    .title{
      display:inline-block;
      font-size:40px;
      margin:30px 10px 0 50px;
      color:  #4169E1;
    }
    hr{float:left; border:none; width:900px; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:50px;}
    .alert{
          width:65%;
          height:25px;
          line-height: 25px;
          padding: 10px;
          background-color: #f44336;
          color: white;
          opacity: 1;
          transition: opacity 0.6s;
          float:left;
          font-size:16px;
          text-align:left;
          margin:5px 50px;
      display:none;
        }
        .alert.success{background-color: #4CAF50;}
        .alert.info{background-color: #2196F3;}
        .alert.warning{background-color: #ff9800;}
        .closebtn {
          float:right;
          color: white;
          font-weight: bold;
          float: right;
          font-size: 22px;
          line-height: 25px;
          cursor: pointer;
          transition: 0.3s;
        }
        .closebtn:hover {
          color: black;
        }
        nav{
      float:left;
      width:50%;
      margin-left:50px;
            margin-bottom:15px;
    }
    nav button{
      border:none;
      background:transparent;
      padding:0 10px;
      font-size: 15px;
      outline:none;
    }
    nav button:hover{cursor: pointer;}
        tbody {
      display:inline-block;
      width: 100%;
      height:500px;
      overflow-y: scroll;
        } 
    .fn{width:200px;} .fs{width:150px;} .action{width:350px;}
        th.action{padding-left:15px;}
    th{
      
      text-align:left;
      padding:12px;
      color:#191970;
      background-color:#B0C4DE;
      font-weight: normal;
    }
    td{
      text-align:left;
      padding:12px;
      color:#191970;
      background-color:#F0F8FF;
      font-weight: normal;
      font-size:13px;
    }
    table {
      display:inline-block;
      border-collapse: collapse;
      margin:10px 0 0 50px;
      height:500px;
    }
    .view,.open,.SPIFFS_delete,.IMAGE_delete,.JPG_download, .SDServer_delete, .SDSPIFFS_delete,.SPIFFS_download,.SDServer_download, .SD_SPIFFSServer_download,.Folder_Download,.Folder_Delete,.TFolder_Download,.TFolder_Delete{
      background:transparent;
      color:#191970;
      border:none;
      outline:none;
    }
    .view:hover,.open:hover, .SPIFFS_delete:hover,.IMAGE_delete:hover, .SDServer_delete:hover, .JPG_download:hover,.SDSPIFFS_delete:hover,.SPIFFS_download:hover, .SDServer_download:hover, .SD_SPIFFSServer_download:hover ,.Folder_Download:hover,.Folder_Delete:hover,.TFolder_Download:hover,.TFolder_Delete:hover{
      text-decoration: underline;
      cursor: pointer;
    }
    select{
      float:left;
      margin:15px 300px 0 50px;
      padding:2px 20px;
      border:none;
      border-bottom: 2px solid #4169E1;
      outline:none;
    }
    #log{display:none}
    #IMAGE_Table,#SDServer_Table,#SDSPIFFSServer_Table{display:none;}
        #fd,#Select_All{display:none;}
        #fd .fn{width:200px;} #fd .fs{width:150px;} #fd .action{width:250px;} #fd .items{width:150px;}
    #modalWindow {
            position: fixed;
            top: 0;
            right: 0;
            bottom: 0;
            left: 0;
            background: rgba(0, 0, 0, 0.8);
            z-index: 9999;
            opacity:1;
      display:none;
        }
        #modalWindow:target {
            opacity:1;
            pointer-events: auto;
        }
        #modalWindow > div {
            width: 80%;
            height: 30%;
            position: relative;
            margin-top:150px;
            padding: 20px 20px 13px 20px;
            background: transparent;
        }
        #modalimg{
            position:absolute;
            max-width: 80%;
            height: 500px;
            border:1px solid rgba(0,0,0,0.1);
            top:1%;
            left: 60%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
        }
    @media only screen and (max-width: 600px) {
      body,
      html {
        width:100%;
      }
      #dashboard{
        float:left;
        width:350px;
        height:600px;
        background-color: #191919;
        position:absolute;
        right:750px;
        z-index:1;
      }
      #dashboard header{
        width:100%;
        height:70px;
        background-color:#03254c; 
        text-align: left;
      }
      #dashboard p{
        color:ivory;
        display:inline-block;
        font-size: 30px;
        margin-top:20px;
        margin-left:10px;
      }
      #dashboard header span{
        float:right;
        display:block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #content{
        width:100%;
        background-color:#F5FFFA;
      }
      #content header{
        width:100%;
        height:70px;
        background-color:#03254c;
        text-align: left;
      }
      #content header span{
        display:inline-block;
        margin:20px 20px;
        color:ivory;
        font-size:25px;
      }
      #info{
        width:100%;
        height:20%;
      }
      #info img{
        float:left;
        width:90px;
        height:75%;
        border-radius: 100px;
        margin:20px;
      }
      #info label{
        color:ivory;
        display:inline-block;
        margin-top:45px;
      }
      #info a{
        text-decoration: none;
        margin-top:5px;
        color:yellow;
      }
      #info a:hover{
        color:gold;
      }
      #main{
        margin-top:30px;
        width:100%;
        height:inherit;
        background-color: #333;
      }
      .menutext{
        width:88.5%;
        display:inline-block;
        padding:20px;
        color:gray;
        background-color: #0c0c0c;
      }
      .title{
        display:inline-block;
        font-size:40px;
        margin:30px 10px 0 10px;
        color:  #4169E1;
      }
      hr{float:left; border:none; width:95%; height:5px; background-color:#4169E1; border-radius: 10px; margin-left:10px;}
      nav{
        float:left;
        width:100%;
        margin-left:10px;
      }
      .alert{
        float:left;
        margin:2px 5px;
        height:10px;
        width:92%;
        padding: 10px;
        font-size:12px;
        line-height: 12px;
      }
      .closebtn {
        line-height: 10px;
      }
      tbody {
        width:99.5%;
      } 
      .view,.open,.IMAGE_delete, .SPIFFS_delete, .SDServer_delete, .SDSPIFFS_delete,.SPIFFS_download, .JPG_download, .SDServer_download, .SD_SPIFFSServer_download,.Folder_Download,.Folder_Delete,.TFolder_Download,.TFolder_Delete{
        font-size:12px;
      }
      td{
        font-size:12px;
      }
      table {
        display:inline-block;
        border-collapse: collapse;
        margin:10px 0 0 0;
        height:100%;
        width:98%;
      }
      select{
        margin:30px 300px 0 0;
        background:transparent;
      }
      th.fn{width:45%;} th.fs{width:15%;} th.action{width:100%;}
            td.fn{width:45%;} td.fs{width:15%;}td.action{width:100%;}
      #modalWindow {
                position: fixed;
                top: 0;
                right: 0;
                bottom: 0;
                left: 0;
                background: rgba(0, 0, 0, 0.8);
                z-index: 9999;
                opacity:1;
            }
            #modalWindow:target {
                opacity:1;
                pointer-events: auto;
            }
            #modalWindow > div {
                width: 80%;
                height: 30%;
                position: relative;
                margin-top:150px;
                padding: 20px 20px 13px 20px;
                background: transparent;
            }
            #modalimg{
                position:absolute;
        width: 100%;
                max-width: 100%;
                height: 120%;
                border:1px solid rgba(0,0,0,0.1);
                left: 55%; right:50%;transform: translate(-50%,0); position: absolute; z-index: 1
            }
    }
  </style>
</head>
<body>
  <div id="container">
    <div id="dashboard">
      <header>
        <p><b>Camera</b> Server</p>
        <span id="closeslider"><i class="fa fa-bars" aria-hidden="true"></i></span>
      </header>
      <div id="info">
        <img src="data:image/png;base64,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">
        <label><b>Admin:</b> *ADMIN*</label><br>
        <a href="#" id="logout"><i class="fa fa-cogs" aria-hidden="true"></i> Logout Account</a>
      </div>
      <div id="main">
        <label class="menutext">Dashboard</label>
        <ul>
          <a href="/"><li><i class="fa fa-tachometer" aria-hidden="true"></i> Overview <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/streaming"><li><i class="fa fa-video-camera" aria-hidden="true"></i> Stream <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/motion"><li><i class="fa fa-dot-circle-o" aria-hidden="true"></i> Motion Sensor <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/timelapse"><li><i class="fa fa-clock-o" aria-hidden="true"></i> Timelapse <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
          <a href="/record"><li class="active"><i class="fa fa-book" aria-hidden="true"></i> Records <label><i id="arrowright" class="fa fa-caret-right" aria-hidden="true"></label></i></li></a>
        </ul>
      </div>
    </div>
    <div id="content">
      <header>
        <span id="slider"><i class="fa fa-bars" aria-hidden="true"></i></span>
        <span id="reset"><i class="fa fa-power-off" aria-hidden="true"></i></span>
      </header>
      <label class="title">Records</label>
      <hr>
            <nav>
                <button id="file">Files</button>
                <button id="folder">Folder</button>
                <button id="custom">Custom</button>
            </nav>
            <div class="alert" id="disable">
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Warning: </strong> Motion is enabled, SD card unavailable
              </div>
              <div class="alert" id="error">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
              </div>
              <div class="alert info" id="infobox">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-info-circle" aria-hidden="true"></i><strong> Info:</strong> Camera ready! Use stream to view.
              </div>
              <div class="alert success" id="success">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
              </div>
              <div class="alert warning" id="warning">
                <span class="closebtn">&times;</span>  
                <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
              </div>
            <div id="c">

            </div>
            <div id="fd">
                <select id="folder_selection">
                    <option value = "Timelapse_Folder"> Timelapse </option>
                    <option value = "All"> ALL </option>
                </select>
                <div class="alert warning" id="warning">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
                </div>
                <div class="alert" id="error">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
                </div>
                <div class="alert success" id="success">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
                </div>
                <table id="Select_All">
                    <thead>
                      <tr>
                        <th class="fn">File Name</th>
                        <th class="fs">Size</th>
                        <th class="items">Items</th>
                        <th class="action">Action</th>
                      </tr>
                    </thead>
                    <tbody>
                        <tr><td class='fn'>serverlog_0.txt</td><td class='fs'>200B</td><td class='items'>100</td><td class='action'><button class='Folder_Download'>download</button>|<button class='Folder_Delete'>remove</button></td></tr>
                        <tr><td class='fn'>serverlog_0.txt</td><td class='fs'>200B</td><td class='items'>3100</td><td class='action'><button class='Folder_Download'>download</button>|<button class='Folder_Delete'>remove</button></td></tr>
                    </tbody>
                </table>
                <table id="Folder_Timelapse">
                    <thead>
                      <tr>
                        <th class="fn">File Name</th>
                        <th class="fs">Size</th>
                        <th class="items">Items</th>
                        <th class="action">Actions</th>
                      </tr>
                    </thead>
                    <tbody>
                        *TIMELAPSE_FOLDER*
                    </tbody>
                </table>
            </div>
            <div id="r">
                <select id="selection">
                    <option value = "SPIFFS"> SPIFFS Log </option>
                    <option value = "SD_CARD"> Server Logs </option>
          <option value = "SPIFFS_SD_CARD"> SPIFFS Server Log</option>
          <option value = "IMAGES"> Images</option>
                </select>
                <div class="alert warning" id="warning">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong> Warning: </strong> Indicates a warning that might need attention.
                </div>
                <div class="alert" id="error">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-exclamation-triangle" aria-hidden="true"></i> <strong>Error: </strong> Error 404
                </div>
                <div class="alert success" id="success">
                    <span class="closebtn">&times;</span>  
                    <i class="fa fa-check-circle" aria-hidden="true"></i> <strong> Success: </strong> Indicates a successful or positive action.
                </div>
                <table id="SPIFFS_Table">
                    <thead>
                      <tr>
                        <th class="fn">File Name</th>
                        <th class="fs">Size</th>
                        <th class="action">Actions</th>
                      </tr>
                    </thead>
                    <tbody id="SPIFFS_tbody">
          
        </tbody>
        </table>
        <table id="IMAGE_Table">
          <thead>
            <tr>
            <th class="fn">Image Name</th>
            <th class="fs">Size</th>
            <th class="action">Actions</th>
            </tr>
          </thead>
          <tbody id="IMAGE_tbody">
          
        </tbody>
        </table>
                <table id="SDServer_Table">
                    <thead>
                      <tr>
                        <th class="fn">File Name</th>
                        <th class="fs">Size</th>
                        <th class="action">Actions</th>
                      </tr>
                    </thead>
                    <tbody id="SDServer_tbody">
          
        </tbody>
                </table>
                <table id="SDSPIFFSServer_Table">
                    <thead>
                      <tr>
                        <th class="fn">File Name</th>
                        <th class="fs">Size</th>
                        <th class="action">Actions</th>
                      </tr>
                    </thead>                 
        <tbody id="SDSPIFFSServer_tbody">
          
        </tbody>
                </table>
            </div>
            <div id="modalWindow">
        <div><img src="display" id="modalimg"></div>
      </div>
    </div>  
  </div>
</body>
</html>
<script>
   $(window).on("load", function() { 
    var image_list = [];
    var spiffs_list = [];
    var serverlog_list = [];
    var spiffsserverlog_list = [];
    var xhttp2 = new XMLHttpRequest();  
    xhttp2.onreadystatechange = function() {
      if(xhttp2.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
          image_list.push(x);
          x = "";
          }
        }
        for(var i = 0; i < image_list.length; i+=2){
          $("#IMAGE_tbody").append("<tr><td class='fn'>"+image_list[i]+"</td><td class='fs'>"+image_list[i+1]+"</td><td class='action'><button class='view'>view</button>|<button class='JPG_download'>download</button>|<button class='IMAGE_delete'>remove</button></td></tr>");          
        }
      }
    }; 
    xhttp2.open("GET", "/init_image_data", true);    
    xhttp2.send();
  
    var xhttp3 = new XMLHttpRequest();  
    xhttp3.onreadystatechange = function() {
      if(xhttp3.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
            spiffs_list.push(x);
            x = "";
          }
        }
        for(var i = 0; i < spiffs_list.length; i+=2){
          $("#SPIFFS_tbody").append("<tr><td class='fn'>"+spiffs_list[i]+"</td><td class='fs'>"+spiffs_list[i+1]+"</td><td class='action'><button class='open'>open</button>|<button class='SPIFFS_download'>download</button>|<button class='SPIFFS_delete'>remove</button></td></tr>");
        }
      }
    }; 
    xhttp3.open("GET", "/init_spiffs_data", true);    
    xhttp3.send();

    var xhttp4 = new XMLHttpRequest();  
    xhttp4.onreadystatechange = function() {
      if(xhttp4.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
            serverlog_list.push(x);
            x = "";
          }
        }
        for(var i = 0; i < serverlog_list.length; i+=2){
          $("#SDServer_tbody").append("<tr><td class='fn'>"+serverlog_list[i]+"</td><td class='fs'>"+serverlog_list[i+1]+"</td><td class='action'><button class='open'>open</button>|<button class='SDServer_download'>download</button>|<button class='SDServer_delete'>remove</button></td></tr>");
        }
      }
    }; 
    xhttp4.open("GET", "/init_server_data", true);    
    xhttp4.send();

    var xhttp5 = new XMLHttpRequest();  
    xhttp5.onreadystatechange = function() {
      if(xhttp5.readyState === 4) { 
        var res = this.responseText;
        var x = "";
        for(var i = 0; i < res.length; i++){
          x += res.charAt(i);
          if(res.charAt(i) == " "){
            spiffsserverlog_list.push(x);
            x = "";
          }
        }
        for(var i = 0; i < spiffsserverlog_list.length; i+=2){
          $("#SDSPIFFSServer_tbody").append("<tr><td class='fn'>"+spiffsserverlog_list[i]+"</td><td class='fs'>"+spiffsserverlog_list[i+1]+"</td><td class='action'><button class='open'>open</button>|<button class='SD_SPIFFSServer_download'>download</button>|<button class='SDSPIFFS_delete'>remove</button></td></tr>");
        }
      }
    }; 
    xhttp5.open("GET", "/init_spiffsserver_data", true);    
    xhttp5.send();
//    var serverlog_list = [];
//    var spiffsserverlog_list = [];
//
//    var xhttp = new XMLHttpRequest();  
//    xhttp.onreadystatechange = function() {
//      if(xhttp.readyState === 4) { 
//        var res = this.responseText;
//        var x = "";
//        for(var i = 0; i < res.length; i++){
//          x += res.charAt(i);
//          if(res.charAt(i) == " "){
//            spiffsserverlog_list.push(x);
//            x = "";
//          }
//        }
//        for(var i = 0; i < spiffsserverlog_list.length; i+=2){
//          $("#SDSPIFFSServer_tbody").append("<tr><td class='fn'>"+spiffsserverlog_list[i]+"</td><td class='fs'>"+spiffsserverlog_list[i+1]+"KB</td><td class='action'><button class='open'>open</button>|<button class='SD_SPIFFSServer_download'>download</button>|<button class='SDSPIFFS_delete'>remove</button></td></tr>");
//        }
//      }
//    }; 
//    xhttp.open("GET", "/init_spiffsserver_data", true);    
//    xhttp.send();
//
//    var xhttp2 = new XMLHttpRequest();  
//    xhttp2.onreadystatechange = function() {
//      if(xhttp2.readyState === 4) { 
//        var res = this.responseText;
//        var x = "";
//        for(var i = 0; i < res.length; i++){
//          x += res.charAt(i);
//          if(res.charAt(i) == " "){
//            serverlog_list.push(x);
//            x = "";
//          }
//        }
//        for(var i = 0; i < serverlog_list.length; i+=2){
//          $("#SDServer_tbody").append("<tr><td class='fn'>"+serverlog_list[i]+"</td><td class='fs'>"+serverlog_list[i+1]+"KB</td><td class='action'><button class='open'>open</button>|<button class='SDServer_download'>download</button>|<button class='SDServer_delete'>remove</button></td></tr>");
//        }
//      }
//    }; 
//    xhttp2.open("GET", "/init_server_data", true);    
//    xhttp2.send();
  });
  $('#reset').on('click',function(e){
        $("#infobox").html("<span class='closebtn'>&times;</span> <i class='fa fa-info-circle' aria-hidden='true'></i><strong> Info:</strong> Server restarting, Please wait!");
        $("#warning").css("display", "none"); $("#infobox").css("display", "block"); $("#success").css("display", "none"); $("#error").css("display", "none");

        var xhttp = new XMLHttpRequest(); 
        xhttp.open("GET", "/restart", true);    
        xhttp.send();
    });
  $("#slider").click(function () {
    $("#dashboard").animate({ "right": 0 }, "slow");
    });
    $("#closeslider").click(function () {
        $("#dashboard").animate({ "right": 750 }, "slow");
    });
    $("#file").on('click',function(e){
    $("#fd").css("display","none"); $("#c").css("display","none"); $("#r").css("display","block"); $("#file").css("text-decoration","underline"); $("#custom").css("text-decoration","none"); $("#folder").css("text-decoration","none");
    $(".title").html("Records")
  });
  $("#folder").on('click',function(e){
    $("#r").css("display","none"); $("#c").css("display","none"); $("#fd").css("display","block"); $("#folder").css("text-decoration","underline"); $("#custom").css("text-decoration","none"); $("#file").css("text-decoration","none");
    $(".title").html("Dir Folders")
  });
  $("#custom").on('click',function(e){
    $("#r").css("display","none"); $("#fd").css("display","none"); $("#c").css("display","block"); $("#custom").css("text-decoration","underline"); $("#folder").css("text-decoration","none"); $("#file").css("text-decoration","none");
    $(".title").html("Custom")
  });
    $("#selection").on('change',function(e){    
    if($(this).val() == "SPIFFS"){
      $("#SDSPIFFSServer_Table").css("display","none"); $("#SDServer_Table").css("display","none"); $("#IMAGE_Table").css("display","none"); $("#SPIFFS_Table").css("display","inline-block");
    }
    else if($(this).val() == "SD_CARD"){
      $("#SDSPIFFSServer_Table").css("display","none"); $("#SPIFFS_Table").css("display","none"); $("#IMAGE_Table").css("display","none"); $("#SDServer_Table").css("display","inline-block");
    }
    else if($(this).val() ==  "SPIFFS_SD_CARD"){
      $("#SPIFFS_Table").css("display","none"); $("#SDServer_Table").css("display","none"); $("#IMAGE_Table").css("display","none"); $("#SDSPIFFSServer_Table").css("display","inline-block");
    }
    else if($(this).val() == "IMAGES"){
      $("#SDSPIFFSServer_Table").css("display","none"); $("#SPIFFS_Table").css("display","none"); $("#SDServer_Table").css("display","none"); $("#IMAGE_Table").css("display","inline-block");
    }
  });

    $('table').on('click', '.SPIFFS_download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SPIFFS_file_download?value="+n, false);    
    xhttp.send();
    
    window.location = '/SPIFFS_file_download?value='+n;
  });

  $('table').on('click', '.SDServer_download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SDServer_download?value="+n, false);    
    xhttp.send();
    
    window.location = '/SDServer_download?value='+n;
  });
  $("#logout").click(function () {
    window.location.href = "/login";
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/logout", true);    
    xhttp.send();
  });
  $('table').on('click', '.SD_SPIFFSServer_download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SD_SPIFFSServer_download?value="+n, false);    
    xhttp.send();
    
    window.location = '/SD_SPIFFSServer_download?value='+n;
  });

  $('table').on('click', '.JPG_download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/JPG_download?value="+n, false);    
    xhttp.send();
    
    window.location = '/JPG_download?value='+n;
  });

  $('table').on('click', '.IMAGE_delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block");  $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/delete_img?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
    });

  $('table').on('click', '.view', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#modalWindow").css("display", "block");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.onreadystatechange = function() {
    if(xhttp.readyState === 4) { 
    var res = this.responseText;
    display(res);
    }
    }; 
    xhttp.open("GET", "/view_img?value="+n, true);    
    xhttp.send();
    });

  $('table').on('click', '.SPIFFS_delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block"); $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SPIFFS_delete?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
    });
  
  $('table').on('click', '.SDServer_delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block"); $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SDServer_delete?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
    });

  $('table').on('click', '.SDSPIFFS_delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block"); $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/SDSPIFFS_delete?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
    });
      
    $('table').on('click', '.TFolder_Download', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/TFolder_Download?value="+n, true);    
    xhttp.send();

        window.location = "/TFolder_Download?value="+n;
    });

    $('table').on('click', '.TFolder_Delete', function() {
    var tr = $(this).closest('tr');
    var n = $(tr).find(".fn").text();
    $("#success").html("<span class='closebtn'>&times;</span>  <i class='fa fa-check-circle' aria-hidden='true'></i> <strong> Success: </strong>"+n+" deleted!");
    $("#success").css("display", "block"); $("#warning").css("display", "none"); $("#error").css("display", "none");
    
    var xhttp = new XMLHttpRequest();    
    xhttp.open("GET", "/TFolder_Delete?value="+n, true);    
    xhttp.send();
    $(this).closest('tr').remove();
    });

    $('#modalWindow').on('click',function(e){
      $(this).css("display", "none");
    });

    function display(res) {
      var val = $("#modalimg").attr("src")
      $("#modalimg").attr("src", val + '?_=' + new Date().getTime());
    }
    
    $('body').on('click', '.closebtn', function() {
        var div = $(this).closest('div');
        $(this).closest('div').animate({opacity:0},700, () => {
            $(this).closest('div').css("display","none");
            $(this).closest('div').css("opacity","1");
        });
    });
</script>
)rawliteral";
