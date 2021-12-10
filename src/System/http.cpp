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
<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script>
<script>
    var pushButton = function(button){
      var xhr = new XMLHttpRequest();
      xhr.open("GET", button, true);
      xhr.send(null);
    }
    var getMain = function () {
      $.getJSON("api/json", (data) => {
        document.getElementById("clock").innerHTML = data.clock;
        document.getElementById("freeram").innerHTML = data.freeram;
        document.getElementById("uptime").innerHTML = data.uptime;
        document.getElementById("battery").innerHTML = data.battery;
        document.getElementById("ischarge").innerHTML = data.power;
      });
    }
    getMain();
    setInterval(getMain, 1000);
  </script>
<body>
  <h2>Misaki Player</h2>
  <p><table>
      <tr><td>Current Clock</td><td><span id="clock" class="value"></span>
      <tr><td>Battery(%)</td><td><span id="battery" class="value"></span>
      <tr><td>Power Supply</td><td><span id="ischarge" class="value"></span>
      <tr><td>Free RAM</td><td><span id="freeram" class="value"></span>
      <tr><td>Uptime</td><td><span id="uptime" class="value"></span>
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
<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script>
<script>
 isImageLoad=false;
 var newImage = new Image();
    var imageReload = function(){
      if(newImage.complete) {
          newImage = new Image();
          newImage.src = "/screenshot"+"?" + new Date().getTime();
          document.getElementById("screenshot").src = newImage.src;
          isImageLoad=false;
          }
    }
    var pushButton = function(button){
      var xhr = new XMLHttpRequest();
      xhr.open("GET", button, true);
      xhr.send(null);
    }
    
    var getMain = function () {
      if(isImageLoad){
      $.getJSON("/api/json", (data) => {
        document.getElementById("clock").innerHTML = data.clock;
        document.getElementById("freeram").innerHTML = data.freeram;
        document.getElementById("uptime").innerHTML = data.uptime;
        document.getElementById("battery").innerHTML = data.battery;
        document.getElementById("ischarge").innerHTML = data.power;
      });
      }
    }
    getMain();
    setInterval(getMain, 1000);
    
  </script>
<body>
  <h2>Misaki Player</h2>
  
  <p><table>
      <tr><td>Current Clock</td><td><span id="clock" class="value"></span>
      <tr><td>Battery(%)</td><td><span id="battery" class="value"></span>
      <tr><td>Power Supply</td><td><span id="ischarge" class="value"></span>
      <tr><td>Free RAM</td><td><span id="freeram" class="value"></span>
      <tr><td>Uptime</td><td><span id="uptime" class="value"></span>
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
  <img id="screenshot" src="screenshot" onload="isImageLoad=true"><br>
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

  server[0].on("/api/json", HTTP_GET, [](AsyncWebServerRequest *request)
               {
                 char*text=new char[200];
                sprintf(text, "%4d/%02d/%02d %02d:%02d:%02d",SystemAPI::Time_year,SystemAPI::Time_month,SystemAPI::Time_day,
                SystemAPI::Time_currentTime/3600000,SystemAPI::Time_currentTime/60000%60,SystemAPI::Time_currentTime/1000%60);
                String clock=text;
                sprintf(text,"%d",SystemAPI::FreeRAM);
                String freeram=text;
                sprintf(text,"%d",SystemAPI::BatteryLeft);
                String battery=text;
                String isPower;
                if(SystemAPI::BatteryIsFull){
                  isPower="Full";
                }else if(SystemAPI::BatteryIsSupply){
                  isPower="Charging";
                }else{
                  isPower="Discharge";
                }
                sprintf(text,"%d:%02d:%02d",millis()/3600000,millis()/60000%60,millis()/1000%60);
                String uptime=text;
                sprintf(text,"{\"clock\":\"%s\",\"freeram\":\"%s\",\"battery\":\"%s\",\"power\":\"%s\",\"uptime\":\"%s\"}",
                clock.c_str(),freeram.c_str(),battery.c_str(),isPower.c_str(),uptime.c_str());

                request->send(200, "text/plain",text); 
                delete[] text; });


  server[0].begin();
}