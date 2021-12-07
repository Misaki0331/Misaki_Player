#include "main.h"
#include "Fonts/FastFont.h"
#include <WiFi.h>
#include <Server.h>
#include"Config.h"
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
<body>
  <h2>Misaki Player</h2>
  <div style="text-align:right;">
  <input type="button"  value="Reload" onclick="location.reload();"><br>
  </div>
  <div style="text-align:center;">
  <input type="button"  value="Button A" onclick="window.location.href = '/BA';">
  <input type="button"  value="Button B" onclick="window.location.href = '/BB';">
  <input type="button"  value="Button C" onclick="window.location.href = '/BC';"><br>
  </div>
  <div style="text-align:center;">
  <img src="screenshot"><br>
  </div>
  
</body>  
</html>)rawliteral";
int Main::ScreenshotRequest = 0;
void Main::HTTPInit()
{
  server = new AsyncWebServer(80);
  server[0].on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send_P(200, "text/html", index_html.c_str()); });
    server[0].on("/screenshot", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    ScreenshotRequest = 1;
    Serial.println("screenshot requesting...");
    while (ScreenshotRequest > 0)
      vTaskDelay(1);
    Serial.println("screenshot Sending...");
    request->send(SD, "/scrnshot.bmp", "image/bmp");});
        
    server[0].on("/BA", HTTP_GET, [](AsyncWebServerRequest *request) {
      SystemData::IsHttpPressA=true;
      request->redirect("/");
    });

    server[0].on("/BB", HTTP_GET, [](AsyncWebServerRequest *request) {
      SystemData::IsHttpPressB=true;
      request->redirect("/");
    });

    server[0].on("/BC", HTTP_GET, [](AsyncWebServerRequest *request) {
      SystemData::IsHttpPressC=true;
      request->redirect("/");
    });

    server[0].begin();
}