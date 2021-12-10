#include "main.h"
#include "Fonts/FastFont.h"
#include <WiFi.h>
#include <Server.h>
#include "Config.h"
using namespace Core;
using namespace Core::Draw;
AsyncWebServer *Main::server;
const uint8_t Main::BMP_Header[] = {
    0x42, 0x4D, 0x36, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
    0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const String Main::index_html = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<script>
    var pushButton = function(button){
      var xhr = new XMLHttpRequest();
      xhr.open("GET", button, true);
      xhr.send(null);
    }
    var getMain = function () {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("clock").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/clock", true);
      xhr.send(null);
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("freeram").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/freeram", true);
      xhr.send(null);
      
    }
    
    var getSub = function () {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("battery").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/battery", true);
      xhr.send(null);
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ischarge").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/power", true);
      xhr.send(null);
      
    }
    getMain();
    getSub();
    setInterval(getMain, 1000);
    setInterval(getSub, 10000);
  </script>
<body>
  <h2>Misaki Player</h2>
  <p><table>
      <tr><td>Current Clock</td><td><span id="clock" class="value"></span>
      <tr><td>Battery(%)</td><td><span id="battery" class="value"></span>
      <tr><td>Power Supply</td><td><span id="ischarge" class="value"></span>
      <tr><td>Free RAM</td><td><span id="freeram" class="value"></span>
      </td></tr>
  </table></p>
  <div style="text-align:right;">
  <input type="button"  value="Reload" onclick="location.reload();"><br>
  </div>
  <div style="text-align:center;">
  <input type="button"  value="Button A" onclick="pushButton('/BA');">
  <input type="button"  value="Button B" onclick="pushButton('/BB');">
  <input type="button"  value="Button C" onclick="pushButton('/BC');"><br>
  </div>
  <div style="text-align:center;">
  <input type="button"  value="Enable Screenshot" onclick="window.location.href = '/enabled_screenshot';">
  </div>
  
</body>  
</html>)rawliteral";
const String Main::index_html_sc = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<script>
 var newImage = new Image();
    var imageReload = function(){
      if(newImage.complete) {
          newImage = new Image();
          newImage.src = '/screenshot';
          var temp = newImage.src;
          newImage = new Image();
          document.getElementById("screenshot").src = temp;
          }
    }
    var pushButton = function(button){
      var xhr = new XMLHttpRequest();
      xhr.open("GET", button, true);
      xhr.send(null);
    }
    var getMain = function () {
      if(1){
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("clock").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/clock", true);
      xhr.send(null);
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("freeram").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/freeram", true);
      xhr.send(null);
      }
    }
    
    var getSub = function () {
      if(1){
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("battery").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/battery", true);
      xhr.send(null);
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ischarge").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", "/api/power", true);
      xhr.send(null);
      }    
    }
    getMain();
    getSub();
    setInterval(getMain, 1000);
    setInterval(getSub, 10000);
  </script>
<body>
  <h2>Misaki Player</h2>
  
  <p><table>
      <tr><td>Current Clock</td><td><span id="clock" class="value"></span>
      <tr><td>Battery(%)</td><td><span id="battery" class="value"></span>
      <tr><td>Power Supply</td><td><span id="ischarge" class="value"></span>
      <tr><td>Free RAM</td><td><span id="freeram" class="value"></span>
      </td></tr>
  </table></p>
  <div style="text-align:right;">
  
  <input type="button"  value="Reload" onclick="imageReload();
"><br>
  </div>
  <div style="text-align:center;">
  <input type="button"  value="Button A" onclick="pushButton('/BA');">
  <input type="button"  value="Button B" onclick="pushButton('/BB');">
  <input type="button"  value="Button C" onclick="pushButton('/BC');"><br>
  </div>
  <div style="text-align:center;">
  <input type="button"  value="Disable Screenshot" onclick="window.location.href = '/';"><br>
  <img id="screenshot" src="screenshot"><br>
  </div>
  
</body>  
</html>)rawliteral";
int Main::ScreenshotRequest = 0;
void Main::HTTPInit()
{
  server = new AsyncWebServer(80);
  server[0].on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send_P(200, "text/html", index_html.c_str()); });
  server[0].on("/enabled_screenshot", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send_P(200, "text/html", index_html_sc.c_str()); });
  server[0].on("/screenshot", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 if (SystemData::IsCaptureAvailable)
                 {
                   ScreenshotRequest = 1;
                   while (ScreenshotRequest > 0)
                     vTaskDelay(1);
                   Serial.println("screenshot Sending...");
                   request->send(SD, "/scrnshot.bmp", "image/bmp");
                 }
                 else
                 {
                   request->send(503, "text/plain", "Couldn't captured because SD Card is busy.\nReason : Sound Player is active! You can't capture while M5stack is playing sound.");
                 } });

  server[0].on("/BA", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressA=true;
      request->redirect("/"); });

  server[0].on("/BB", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressB=true;
      request->redirect("/"); });

  server[0].on("/BC", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressC=true;
      request->redirect("/"); });

  server[0].on("/BA_s", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressA=true;
      request->redirect("/enabled_screenshot"); });

  server[0].on("/BB_s", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressB=true;
      request->redirect("/enabled_screenshot"); });

  server[0].on("/BC_s", HTTP_GET, [](AsyncWebServerRequest *request)
               {
      SystemData::IsHttpPressC=true;
      request->redirect("/enabled_screenshot"); });
  server[0].on("/api/clock", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 char*text=new char[30];
                sprintf(text, "%4d/%02d/%02d %02d:%02d:%02d",SystemAPI::Time_year,SystemAPI::Time_month,SystemAPI::Time_day,
                SystemAPI::Time_currentTime/3600000,SystemAPI::Time_currentTime/60000%60,SystemAPI::Time_currentTime/1000%60);
                request->send(200, "text/plain",text); 
                delete[] text; });

  server[0].on("/api/freeram", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 char*text=new char[30];
                sprintf(text,"%d",SystemAPI::FreeRAM);
                request->send(200, "text/plain",text); 
                delete[] text; });
  server[0].on("/api/battery", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 char*text=new char[30];
                sprintf(text,"%d",SystemAPI::BatteryLeft);
                request->send(200, "text/plain",text); 
                delete[] text; });
  server[0].on("/api/power", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 if(SystemAPI::BatteryIsFull||SystemAPI::BatteryIsSupply){
                  request->send(200, "text/plain","true"); 
                 }else{
                  request->send(200, "text/plain","false"); 
                 } });

  server[0].begin();
}