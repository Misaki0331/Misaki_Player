#include "main.h"
#include "ui/ui.h"
#include "config.h"
#include "Fonts/FastFont.h"
#include "System/Debug/Logger.h"
#include "WiFi.h"
using namespace Core;
using namespace Core::Draw;
using namespace Core::Debug;
uint16_t Main::bitFlip16(uint16_t x)
{
    uint16_t y = 0;
    for (int i = 0; i < 16; i++)
        if (x & (1 << i))
            y |= (1 << 15 - i);

    return y;
}
Main::Main()
{
}
void Main::Begin()
{               // M5Stackのバッテリ初期化
    M5.begin(); // M5Stackを初期化
    SPIFFS.begin(1);
    SPIFFS.end();
    M5.Power.begin();
    M5.Power.setPowerVin(true); // USBが抜かれても動作し続けるように
    setCpuFrequencyMhz(240);
    M5.Lcd.clear(BLACK); //表示リセット
    wavePlayer.Begin();
    Serial.begin(115200);
    xTaskCreatePinnedToCore(ControlThread, "ControlThread", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(SoundThread, "SoundThread", 8192, NULL, 1, NULL, 1);
    appSelecter.Begin();
    appSelecter.Update();
    Logger::Log("System Initialized");

    FastFont::begin();

    Logger::Log("Font is ok.");
    bool IsInited = FirstWiFiConnect();
    Logger::Log("Wi-Fi Initialized");
    if (IsInited)
        HTTPInit();
    Logger::Log("Server Initialized");
}
bool Main::FirstWiFiConnect()
{
    systemData.UpdateSignalUI = true;
    String ssid = "";
    String password = "";
    if (!SPIFFS.begin(0))
    {
        Logger::Log("SPIFFS Formatting...");
        SPIFFS.begin(1);
        Logger::Log("SPIFFS Formated");
    }
    fs::FS fs = SPIFFS;
    char *txt = new char[60];
    sprintf(txt, "/config/Wi-Fi_%02d.ini", SystemAPI::WiFiCurrentProfile);
    Serial.printf("Profile:%02d ", SystemAPI::WiFiCurrentProfile);
    File config = fs.open(txt, FILE_READ);
    if (!config)
    {
        Serial.printf("Does not exist.\n");
        SystemAPI::WiFiCurrentProfile = 99;
        SPIFFS.end();
        delete[] txt;
        return 0;
    }
    delete[] txt;
    ssid = config.readStringUntil('\n');
    password = config.readStringUntil('\n');
    ssid.remove(ssid.length() - 1, 1);
    password.remove(password.length() - 1, 1);
    config.close();
    SPIFFS.end();

    Serial.printf("SSID:%s PASS:%s\n", ssid.c_str(), password.c_str());
    if (ssid == "")
    {
        SystemAPI::WiFiCurrentProfile = 99;
        SPIFFS.end();
        return 0;
    }
    WiFi.begin(ssid.c_str(), password.c_str());
    ssid.clear();
    password.clear();
    LatestConnection = millis();
    return 1;
}
int Main::UpdateUI = 0;
int Main::TempMs = 0;
void Main::Draw()
{
    if (systemData.UpdateBatteryUI)
    {
        systemData.UpdateBatteryUI = false;
        drawUI.Battery(systemConfig.BatteryPosX, systemConfig.BatteryPosY,
                       BatteryPercent, systemConfig.EnableALLUpdate);
    }
    if (systemData.UpdateSignalUI)
    {
        systemData.UpdateSignalUI = false;
        drawUI.RSSI(275, 0, SystemAPI::WiFiLevel, SystemAPI::WiFiCurrentProfile, SystemAPI::WiFiIsConnected, 1);
    }
}
int Main::ButtonACount = 0;
int Main::ButtonBCount = 0;
int Main::ButtonCCount = 0;
int Main::LatestConnection = 0;
bool Main::WiFiError = false;
void Main::Loop()
{
    //主な処理
    M5.update();
    unsigned int Buttons = 0;
    if (M5.BtnA.isPressed())
    {
        ButtonACount++;
        Buttons |= (1 << 3);
    }
    else if (ButtonACount > 0)
    {
        ButtonACount = 0;
    }
    if (M5.BtnB.isPressed())
    {
        ButtonBCount++;
        Buttons |= (1 << 4);
    }
    else if (ButtonBCount > 0)
    {
        ButtonBCount = 0;
    }
    if (M5.BtnC.isPressed())
    {
        ButtonCCount++;
        Buttons |= (1 << 5);
    }
    else if (ButtonCCount > 0)
    {
        ButtonCCount = 0;
    }
    if (ButtonACount == CHATTERING_DELAY)
        Buttons |= (1 << 0);
    if (ButtonBCount == CHATTERING_DELAY)
        Buttons |= (1 << 1);
    if (ButtonCCount == CHATTERING_DELAY)
        Buttons |= (1 << 2);
    appSelecter.SetButtonStatus(Buttons);
    appSelecter.Loop();
    appSelecter.Draw();

    int MilliSecounds = millis();
    if (WiFi.status() != WL_CONNECTED)
    {
        if (LatestConnection + 15000 < MilliSecounds)
        {

            if (WiFiError)
            {
                SystemAPI::WiFiCurrentProfile++;
                SystemAPI::WiFiCurrentProfile %= 100;
            }
            LatestConnection = MilliSecounds;
            FirstWiFiConnect();
            WiFiError = true;
        }
    }
    else
    {
        WiFiError = false;
        LatestConnection = MilliSecounds;
    }
    char text[100];
    if (MilliSecounds / 1000 > UpdateUI)
    {
        UpdateUI = MilliSecounds / 1000;
        FastFont::setPos(0, 0);
        // M5.Lcd.fillRect(0,0,200,10,BLACK);
        FastFont::setColor(WHITE, BLACK);
        int MaxLPS = -1;
        switch (getCpuFrequencyMhz())
        {
        case 240:
            MaxLPS = 851000;
            break;
        case 160:
            MaxLPS = 581800;
            break;
        case 80:
            MaxLPS = 294200;
            break;
        case 40:
            MaxLPS = 140700;
            break;
        case 20:
            MaxLPS = 62500;
            break;
        case 10:
            MaxLPS = 23550;
            break;
        default:
            MaxLPS = 842000;
            break;
        }
        int CPULoad = (int)(((sqrt(MaxLPS) - sqrt(MainLPS)) / sqrt(MaxLPS)) * 10000);
        float per = (float)(RAMSIZE - FreeHeapMemory) / RAMSIZE * 100.0;
        // int per2 = (float)per * 100;
        if (CPULoad < 0)
            CPULoad = 0;
        if (CPULoad > 99999)
            CPULoad = 99999;
        float val = FreeHeapMemory / 1024.0 * 100.0;
        // int val2 = val;

        sprintf(text, "CPU:%3d.%02d%%(%4d) RAM:%6dB", CPULoad / 100, CPULoad % 100,
                SystemAPI::LPS, FreeHeapMemory);
        FastFont::printRom(text, 0, 0, systemConfig.UIUsageCPU_TextColor, 1, systemConfig.UIUsageCPU_BackColor);
    }
    if (MilliSecounds / 1000 != DrawUpdate)
    {
        DrawUpdate = MilliSecounds / 1000;
        Draw();
    }
    // 1000Hz固定
    while (MilliSecounds == TempMs)
    {
        MilliSecounds = millis();
        systemData.LoopCount++;
    }
    SystemData::LPS += 1;
    TempMs = MilliSecounds;
    if (ScreenshotRequest)
    {
        // Serial.println("screenshot requested.");
        SPIFFS.begin(1);
        fs::FS fs = SD;
        if (fs.exists("/scrnshot.bmp"))
            fs.remove("/scrnshot.bmp");
        File screenshot = fs.open("/scrnshot.bmp", FILE_WRITE);

        // Serial.println("/scrnshot.bmp opened.");
        screenshot.write(BMP_Header, 54);
        uint8_t *rect = new uint8_t[960 * 3];
        for (int y = 239; y >= 0; y -= 3)
        {
            m5.lcd.readRectRGB(0, y - 2, 320, 3, rect);
            for (int y1 = 2; y1 >= 0; y1--)
            {
                for (int i = 0; i < 320; i++)
                {
                    screenshot.write(rect[i * 3 + 2 + 960 * y1]);
                    screenshot.write(rect[i * 3 + 1 + 960 * y1]);
                    screenshot.write(rect[i * 3 + 0 + 960 * y1]);
                }
            }

            // Serial.printf("\rCompleted %d/240 Lines. Heap:%d", 240 - y, esp_get_free_heap_size());
        }
        delete[] rect;
        rect = nullptr;

        // Serial.println("\n/scrnshot.bmp closed.");
        screenshot.close();
        screenshot = fs.open("/scrnshot.bmp");
        // Serial.printf("File size: %d Bytes\n", screenshot.size());
        screenshot.close();
        SPIFFS.end();
        ScreenshotRequest = 0;
    }
}
void Main::ControlThread(void *arg)
{
    drawUI.Battery(297, 0, BatteryPercent, true);
    int UpdateTime = 0;
    while (1)
    {
        if (UpdateTime != millis() / 1000)
        {
            UpdateTime = millis() / 1000;
            BatteryPercent = M5.Power.getBatteryLevel() + M5.Power.isCharging() * 200 + M5.Power.isChargeFull() * 400;
            MainLPS = systemData.LoopCount;
            SystemAPI::FLPS = systemData.LoopCount;
            SystemAPI::LPS = SystemData::LPS;
            SystemData::LPS = 0;
            systemData.LoopCount = 0;
            if (systemData.TempBatteryPercent != BatteryPercent)
            {
                systemData.TempBatteryPercent = BatteryPercent;
                systemData.UpdateBatteryUI = true;
            }
            SystemAPI::BatteryLeft = BatteryPercent % 200;
            SystemAPI::BatteryIsCharging = BatteryPercent / 200 % 2;
            SystemAPI::BatteryIsFull = BatteryPercent / 400 % 2;
            SystemAPI::BatteryIsSupply = BatteryPercent >= 200;
            FreeHeapMemory = esp_get_free_heap_size();
            SystemAPI::FreeRAM = FreeHeapMemory;
            int rssi = WiFi.RSSI();
            if (rssi != SystemAPI::WiFiLevel)
                systemData.UpdateSignalUI = 1;
            if (rssi > SystemAPI::WiFiLevel + 5)
            {
                SystemAPI::WiFiLevel += 5;
            }
            else if (rssi < SystemAPI::WiFiLevel - 5)
            {
                SystemAPI::WiFiLevel -= 5;
            }
            else
            {
                SystemAPI::WiFiLevel = rssi;
            }
            SystemAPI::WiFiIsConnected = WiFi.status() == WL_CONNECTED;
        }
        vTaskDelay(1000);
    }
}
int Main::DrawUpdate = 0;
String Main::DrawTemp = "";
int Main::MainLPS = 0;
int Main::BatteryPercent = 0;
bool Main::DisableUI = false;

void Main::SoundThread(void *arg)
{
    while (1)
    {

        wavePlayer.Loop();
    }
}
int Core::SystemData::LoopCount = 0;
bool Core::SystemData::UpdateBatteryUI = false;
int Core::SystemData::TempBatteryPercent = 0;
int Core::SystemConfig::BatteryPosX = 0;
int Core::SystemConfig::BatteryPosY = 0;
bool Core::SystemConfig::EnableALLUpdate = 0;
Apps::System::Select Main::appSelecter;
int Main::FreeHeapMemory = 0;