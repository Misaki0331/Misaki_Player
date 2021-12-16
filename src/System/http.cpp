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
<!DOCTYPE HTML><meta name="viewport" content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no"><title>Misaki Player</title><body><script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script><script>isImageLoad=!0;var newImage=new Image,imageReload=function(){newImage.complete&&(newImage=new Image,newImage.src="/screenshot?"+(new Date).getTime(),document.getElementById("screenshot").src=newImage.src,isImageLoad=!1)},pushButton=function(n){var t=new XMLHttpRequest;t.open("GET",n,!0);t.send(null)},getMain=function(){isImageLoad&&$.getJSON("/api/json",n=>{document.getElementById("clock").innerHTML=n.clock,document.getElementById("freeram").innerHTML=n.freeram,document.getElementById("uptime").innerHTML=n.uptime,document.getElementById("battery").innerHTML=n.battery,document.getElementById("ischarge").innerHTML=n.power,document.getElementById("cpu").innerHTML=n.cpu})};getMain();setInterval(getMain,1e3)</script><style>.api_list table td:nth-of-type(2){background-color:#afffff;width:210px}.api_list table td:nth-of-type(1){background-color:#dfffff;width:130px}.api_list table tr,td{border:solid 1px}.box{width:300px;height:200px;margin:10px 50px 10px 50px;float:left;text-align:center}.boxContainer{overflow:hidden;height:0;position:relative;float:left;text-align:center;margin-left:auto;margin-right:auto}.buttons{width:70px;height:40px;margin-left:10px;margin-right:10px}</style><h2>Misaki Player</h2><div class="boxContain"><div class="box"><div class="api_list"><p><table style="border-collapse:collapse;text-align:left;border:solid 2px"><tbody><tr><td>Current Clock<td><span id="clock" class="value"></span><tr><td>Battery(%)<td><span id="battery" class="value"></span><tr><td>Power Supply<td><span id="ischarge" class="value"></span><tr><td>CPU Usage(%)<td><span id="cpu" class="value"></span><tr><td>Free RAM<td><span id="freeram" class="value"></span><tr><td>Uptime<td><span id="uptime" class="value"></span></table><p></div></div><div class="box"><div class="Screen"><div style="text-align:center;float:left;width:320px;height:50px"><input type="button" value="Enable ScreenCapture" onclick="window.location.href = '/enabled_screenshot';" style="height:40px;width:120px;margin-right:35px;white-space:normal"><input type="button" class="b_reload" value="Reload Screen" style="height:40px;width:90px;margin-left:65px;white-space:normal"></div><br><div style="width:320px;height:240px;background-color:#000;margin-block-start:24px"><p style="color:#fff">Disabled Screen capture</div><div class="button_contain" style="text-align:center;width:320px;height:40px;margin-top:5px"><input type="button" class="buttons" onclick="pushButton('/BA');"><input type="button" class="buttons" onclick="pushButton('/BB');"><input type="button" class="buttons" onclick="pushButton('/BC');"><br></div></div></div></div>
)rawliteral";
const String Main::index_html_sc = R"rawliteral(
<!DOCTYPE HTML><meta name="viewport" content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no"><title>Misaki Player</title><body><script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.4/jquery.min.js"></script><script>isImageLoad=!1;var newImage=new Image,imageReload=function(){newImage.complete&&(newImage=new Image,newImage.src="/screenshot?"+(new Date).getTime(),document.getElementById("screenshot").src=newImage.src,isImageLoad=!1)},pushButton=function(n){var t=new XMLHttpRequest;t.open("GET",n,!0);t.send(null)},getMain=function(){isImageLoad&&$.getJSON("/api/json",n=>{document.getElementById("clock").innerHTML=n.clock,document.getElementById("freeram").innerHTML=n.freeram,document.getElementById("uptime").innerHTML=n.uptime,document.getElementById("battery").innerHTML=n.battery,document.getElementById("ischarge").innerHTML=n.power,document.getElementById("cpu").innerHTML=n.cpu})};getMain();setInterval(getMain,1e3)</script><style>.api_list table td:nth-of-type(2){background-color:#afffff;width:210px}.api_list table td:nth-of-type(1){background-color:#dfffff;width:130px}.api_list table tr,td{border:solid 1px}.box{width:300px;height:200px;margin:10px 50px 10px 50px;float:left;text-align:center}.boxContainer{overflow:hidden;height:0;position:relative;float:left;text-align:center;margin-left:auto;margin-right:auto}.buttons{width:70px;height:40px;margin-left:10px;margin-right:10px}</style><h2>Misaki Player</h2><div class="boxContain"><div class="box"><div class="api_list"><p><table style="border-collapse:collapse;text-align:left;border:solid 2px"><tr><td>Current Clock<td><span id="clock" class="value"></span><tr><td>Battery(%)<td><span id="battery" class="value"></span><tr><td>Power Supply<td><span id="ischarge" class="value"></span><tr><td>CPU Usage(%)<td><span id="cpu" class="value"></span><tr><td>Free RAM<td><span id="freeram" class="value"></span><tr><td>Uptime<td><span id="uptime" class="value"></span></table></div></div><div class="box"><div class="Screen"><div style="text-align:center;float:left;width:320px;height:50px"><input type="button" value="Disable ScreenCapture" onclick="window.location.href = '/';" style="height:40px;width:120px;margin-right:35px;white-space:normal"><input type="button" class="b_reload" value="Reload Screen" onclick="imageReload();" style="height:40px;width:90px;margin-left:65px;white-space:normal"><br></div><div style="width:320px;height:240px"><img id="screenshot" src="screenshot" onload="isImageLoad=true" style="width:320px;height:240px;background-color:#000"><br></div><br><div class="button_contain" style="text-align:center;width:320px;height:40px;margin-top:30px"><input type="button" class="buttons" onclick="pushButton('/BA');"><input type="button" class="buttons" onclick="pushButton('/BB');"><input type="button" class="buttons" onclick="pushButton('/BC');"><br></div></div></div></div>
)rawliteral";
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
                sprintf(text,"{\"clock\":\"%s\",\"cpu\":%.1f,\"freeram\":%d,\"battery\":%d,\"power\":\"%s\",\"uptime\":\"%s\"}",
                clock.c_str(), (100.0-SystemAPI::LPS/10.0),SystemAPI::FreeRAM,SystemAPI::BatteryLeft,isPower.c_str(),uptime.c_str());

                request->send(200, "text/plain",text); 
                delete[] text; });


  server[0].begin();
}