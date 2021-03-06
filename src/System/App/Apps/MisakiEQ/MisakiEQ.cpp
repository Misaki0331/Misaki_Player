#include "MisakiEQ.hpp"
#include "../../../Fonts/FastFont.h"
#include "../../../sound/sound.h"
#include "../../../config.h"
using namespace App::MisakiEQ;
using namespace Core::Draw;
using namespace Core::Sound;

EEW::EEW()
{
    First = 0;
}
EEW::~EEW()
{
    RunThread = false;
}
void EEW::Begin()
{
    Core::SystemData::UpdateBatteryUI = 1;
    Core::SystemData::UpdateSignalUI = 1;
    Serial.println("MisakiEQ Begining");
    TestTime = 0;
    MapSize = 1;
    IsActive = false;
    toHome = 0;
    FirstCheck = false;
    IsFirstEEWForecast = false;
    IsFirstEEWWarn = false;
    mode = ClockMode;
    sellectMode = ClockMode;
    JsonOldState = 0;
    IsFirstDrawed = 0;
    IsUpdated = 0;
    settingSellect = 0;
    IsButtonUIUpdate = 0;
    FinishedThread = 0;
    RunThread = 1;
    WarnRegionDisplay = 1;
    IsPingUpdate = false;
    IsNotCursorMode = false;
    LCDTimer = millis();
    IsnotLCDLight = false;
    Reboottimer = 0;
    IsAutoRotateStop = true;
    IsUserPressed = true;
    TempMaxGal = "               ";
    TempAvgGal = "               ";
    TempIsBatterySupply = Core::SystemAPI::BatteryIsSupply;
    pg = 1;
    http = new HTTPClient();
    PingValue = new short[PingData];
    PingValue12sec = new short[PingData];
    PingValue60sec = new short[PingData];
    for (int i = 0; i < PingData; i++)
    {
        PingValue[i] = 0;
        PingValue12sec[i] = 0;
        PingValue60sec[i] = 0;
    }
    IsPingOpen = true;
    Serial.println("MisakiEQ TaskCreating...");
    xTaskCreatePinnedToCore(GetNetworkFile, "MisakiEQ_EEW", 16800, NULL, 5, NULL, 1);
    Serial.println("MisakiEQ Config Reading...");
    ReadConfig();
    Serial.println("MisakiEQ Init...");
    LCDLightUp();
    PingLoad();
    if (SPIFFS.begin(0))
    {

        fs::FS fs = SPIFFS;
        File fp = fs.open("/MisakiEQ.flg", FILE_WRITE);
        if (fp)
        {
            fp.close();
        }
        else
        {
            Serial.println("Error! Cannot created!");
        }
        SPIFFS.end();
    }
    Serial.println("MisakiEQ init done!");
}
void EEW::Loop()
{
    int SettingTime;
    if (Core::SystemAPI::BatteryIsSupply)
    {
        SettingTime = config.LCDoffTimer;
    }
    else
    {
        SettingTime = config.LCDoffTimerBattery;
    }
    if (SettingTime != 0 && !IsnotLCDLight)
    {
        int t = millis() - LCDTimer;

        if (t > SettingTime * 1000)
        {
            IsnotLCDLight = true;
            tempoffmode = mode;
            mode = -1000;
            ModeEnter();
            M5.Lcd.setBrightness(0);
        }
    }
    if (IsnotLCDLight)
    {
        int t = millis() - LCDTimer;
        if (t <= SettingTime * 1000)
        {
            IsnotLCDLight = false;
            LCDLightUp();
        }
    }
    if (TempIsBatterySupply != Core::SystemAPI::BatteryIsSupply)
    {
        TempIsBatterySupply = Core::SystemAPI::BatteryIsSupply;
        LCDTimer = millis();
        IsnotLCDLight = false;
        LCDLightUp();
    }
    if (IsPingOpen && !IsPingUpdate)
    {
        if (PingCount % 1200 == 1199)
        {
            PingSave();
        }
        if (!(mode == EEWMode || mode == PingMode))
        {
            IsPingUpdate = true;
        }
    }
    if (config.RebootTimer != 0 && Reboottimer != 0)
    {
        int t = millis() - Reboottimer;
        if (t > config.RebootTimer * 1000)
        {
            M5.Lcd.setBrightness(0);
            PingSave();
            int *overflow = new int[655360];
        }
    }
    if (config.AutoChangeEEW&&!IsUserPressed || !IsAutoRotateStop)
    {
        if (LatestEarthquake + 180000 < millis())
        {
            IsUserPressed = true;
            IsAutoRotateStop = true;
            sellectMode = ClockMode;
            ModeEnter();
        }
    }
    if (mode != EEWMode)
    {
        if (!IsCheck)
        {
            IsCheck = true;
            int a = json["Status"]["Code"];
            int b = json["Serial"];
            JsonState = a * 10000 + b;

            if (JsonState != JsonOldState && json["ParseStatus"] == "Success")
            {
                if (!FirstCheck)
                {
                    FirstCheck = true;
                    JsonOldState = JsonState;
                    IsUpdated = 0;
                }
                else
                {
                    LatestEarthquake = millis();
                    IsNotCursorMode = false;
                    IsUserPressed = false;
                    switch (mode)
                    {
                    case SettingNum:
                        cNum.Cancel();
                        cNum.Release();
                        break;
                    case SettingPath:
                        cPath.Cancel();
                        cPath.Release();
                        break;
                    case SettingList:
                        cList.Cancel();
                        cList.Release();
                    }
                    JsonOldState = JsonState;

                    IsUpdated = 0;
                    LCDTimer = millis();
                    if (config.AutoChangeEEW&&mode != MapMode)
                    {
                        if(config.AutoChangeMode)
                            sellectMode = MapMode;
                        else
                            sellectMode = EEWMode;
                        
                        
                        ModeEnter();
                    }
                    else
                    {
                        IsUpdated = false;
                    }
                    if (json["Warn"])
                    {
                        IsAutoRotateStop = false;
                    }

                    if (json["Warn"])
                    {
                        if (config.OnlyListEvent == "???????????????")
                        {
                            Reboottimer = millis();
                            CallSoundWarn();
                        }
                        else
                        {
                            for (int i = 0;; i++)
                            {
                                String name = json["WarnForecast"]["LocalAreas"][i];
                                if (config.OnlyListEvent == name)
                                {
                                    Reboottimer = millis();
                                    CallSoundWarn();
                                    break;
                                }
                                if (name == "null")
                                    break;
                            }
                        }
                    }
                    else
                    {

                        if (config.ForecastSoundPath != "")
                        {
                            Reboottimer = millis();
                            CallSoundForecast();
                        }
                    }
                    if (a % 10 == 1 || a % 10 == 9)
                    {
                        CallSoundReset();
                    }
                }
            }
        }
    }
    switch (mode)
    {
    case ClockMode:
        IsUpdated = 0;
        break;
    case EEWMode:
        if (!IsCheck)
        {

            IsCheck = false;
            int a = json["Status"]["Code"];
            int b = json["Serial"];
            JsonState = a * 10000 + b;

            if (JsonState != JsonOldState && json["ParseStatus"] == "Success")
            {

                if (!FirstCheck)
                {
                    FirstCheck = true;
                    JsonOldState = JsonState;
                    IsUpdated = 0;
                }
                else
                {
                    IsUserPressed = false;

                    LatestEarthquake = millis();
                    JsonOldState = JsonState;
                    IsUpdated = 0;
                    LCDTimer = millis();

                    if (json["Warn"])
                    {
                        IsAutoRotateStop = false;
                    }

                    if (json["Warn"])
                    {
                        if (config.OnlyListEvent == "???????????????")
                        {
                            Reboottimer = millis();
                            CallSoundWarn();
                        }
                        else
                        {
                            for (int i = 0;; i++)
                            {
                                String name = json["WarnForecast"]["LocalAreas"][i];
                                if (config.OnlyListEvent == name)
                                {
                                    Reboottimer = millis();
                                    CallSoundWarn();
                                    break;
                                }
                                if (name == "null")
                                    break;
                            }
                        }
                    }
                    else
                    {

                        if (config.ForecastSoundPath != "")
                        {
                            Reboottimer = millis();
                            CallSoundForecast();
                        }
                    }
                    if (a % 10 == 1 || a % 10 == 9)
                    {
                        CallSoundReset();
                    }
                }
            }
        }

        if (IsRegionUpdate && millis() >= regionUpdate + 5000)
        {
            WarnRegionDisplay = 0;
            regionUpdate = millis();
        }
        break;
    case MapMode:
        if (config.AutoModeRotate&&IsRegionUpdate && !IsAutoRotateStop)
        {
            if (millis() >= regionUpdate + 5000)
            {
                WarnRegionDisplay = 0;
                regionUpdate = millis();
                regPos = -1;
                sellectMode = EEWMode;
                ModeEnter();
            }
        }
        break;
    case SettingNum:
        if (!cNum.GetIsSetting())
        {
            sellectMode = SettingMode;
            cNum.Release();
            ModeEnter();
            LCDLightUp();
        }
        break;
    case SettingList:
        if (!cList.GetIsSetting())
        {
            sellectMode = SettingMode;
            cList.Release();
            ModeEnter();
        }
        break;
    case SettingPath:

        if (!cPath.GetIsSetting())
        {
            sellectMode = SettingMode;
            cPath.Release();
            ModeEnter();
        }
    }
}
void EEW::LCDLightUp()
{
    int lev = 0;
    if (Core::SystemAPI::BatteryIsSupply)
    {
        lev = config.LCDLightLvSupply;
    }
    else
    {
        lev = config.LCDLightLvBattery;
    }

    M5.Lcd.setBrightness(lev);
}
void EEW::BackGround()
{
    if (!First)
    {
        First = true;
        if (SPIFFS.begin(0))
        {

            fs::FS fs = SPIFFS;
            if (fs.exists("/MisakiEQ.flg"))
            {
                IsActive = true;
                Serial.println("MisakiEQ Found!");
            }
            else
            {
                Serial.println("MisakiEQ Not Found!");
            }
            SPIFFS.end();
        }
        else
        {
            Serial.println("SPIFFS Error!");
        }
    }
}
bool EEW::GetBackGround()
{
    return !First;
}
int EEW::GetColor(unsigned int col)
{
    int R = col / 65536 % 256;
    int G = col / 256 % 256;
    int B = col % 256;
    R /= 8;
    G /= 4;
    B /= 8;
    int rgb = R * 64;
    rgb += G;
    rgb *= 32;
    rgb += B;
    return rgb;
}
void EEW::Draw()
{
    char *text;
    switch (mode)
    {
    case ClockMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = 1;
            M5.Lcd.fillRect(0, 14, 320, 210, BLACK);
            clockString.day = 0;
            clockString.month = 0;
            clockString.year = 0;
            clockString.time = 0;
            clockString.time_str = "            ";
            clockString.AccelMax = "      ";
            clockString.AccelAvg = "      ";
            clockString.AccelMax_1 = 255;
            clockString.AccelAvg_1 = 255;
            clockString.UpdateAccel = -1;

            FastFont::printUtf8("????????????", 0, 15, WHITE, 1, BLACK);

            FastFont::printUtf8("???", 72 + 41, 47, WHITE, 1, BLACK);
            FastFont::printUtf8("???", 122 + 41, 47, WHITE, 1, BLACK);
            FastFont::printUtf8("???", 172 + 41, 47, WHITE, 1, BLACK);

            FastFont::printUtf8("(???)", 186 + 41, 36, WHITE, 2, BLACK);

            FastFont::printUtf8("???????????????(gal)", 30, 130, WHITE, 1, BLACK);
            FastFont::printUtf8("???????????????(gal)", 190, 130, WHITE, 1, BLACK);
            M5.Lcd.drawRect(0, 128, 160, 60, WHITE);
            M5.Lcd.drawRect(160, 128, 160, 60, WHITE);
            FastFont::printRom("0", 0, 207, WHITE, 1, BLACK);
            FastFont::printRom("6", 48, 207, WHITE, 1, BLACK);
            FastFont::printRom("20", 95, 207, WHITE, 1, BLACK);
            FastFont::printRom("60", 145, 207, WHITE, 1, BLACK);
            FastFont::printRom("200", 191, 207, WHITE, 1, BLACK);
            FastFont::printRom("600", 241, 207, WHITE, 1, BLACK);
            FastFont::printRom("1500", 297, 207, WHITE, 1, BLACK);
        }
        if (!IsUpdated)
        {
            IsUpdated = 1;
            if (clockString.year != Core::SystemAPI::Time_year)
            {
                clockString.year = Core::SystemAPI::Time_year;
                text = new char[7];
                sprintf(text, "%4d", clockString.year);
                FastFont::printRom(text, 0 + 41, 38, WHITE, 3, BLACK);
                delete[] text;
            }
            if (clockString.month != Core::SystemAPI::Time_month)
            {
                clockString.month = Core::SystemAPI::Time_month;
                text = new char[4];
                sprintf(text, "%2d", clockString.month);
                FastFont::printRom(text, 87 + 41, 38, WHITE, 3, BLACK);
                delete[] text;
            }
            if (clockString.day != Core::SystemAPI::Time_day)
            {
                clockString.day = Core::SystemAPI::Time_day;
                text = new char[30];
                sprintf(text, "%2d", clockString.day);
                FastFont::printRom(text, 137 + 41, 38, WHITE, 3, BLACK);

                switch (Core::SystemAPI::Time_day_of_week)
                {
                case 0:
                    sprintf(text, "???");
                    break;
                case 1:
                    sprintf(text, "???");
                    break;
                case 2:
                    sprintf(text, "???");
                    break;
                case 3:
                    sprintf(text, "???");
                    break;
                case 4:
                    sprintf(text, "???");
                    break;
                case 5:
                    sprintf(text, "???");
                    break;
                case 6:
                    sprintf(text, "???");
                    break;
                }
                if (Core::SystemAPI::Time_day_of_week == 0)
                {
                    FastFont::printUtf8(text, 200 + 41, 36, RED, 2, BLACK);
                }
                else if (Core::SystemAPI::Time_day_of_week == 6)
                {
                    FastFont::printUtf8(text, 200 + 41, 36, CYAN, 2, BLACK);
                }
                else
                {
                    FastFont::printUtf8(text, 200 + 41, 36, WHITE, 2, BLACK);
                }
                delete[] text;
            }
            if (clockString.time != Core::SystemAPI::Time_currentTime / 1000)
            {
                clockString.time = Core::SystemAPI::Time_currentTime / 1000;
                text = new char[20];
                sprintf(text, "%2d:%02d:%02d", clockString.time / 3600, clockString.time / 60 % 60, clockString.time % 60);
                FastFont::printFastRom(clockString.time_str, text, 19, 70, WHITE, 6, BLACK);
                clockString.time_str = text;
                delete[] text;
            }
            if (clockString.UpdateAccel != millis() / 20)
            {
                clockString.UpdateAccel = millis() / 20;
                int max = Core::SystemAPI::AccelMax;
                int avg = Core::SystemAPI::AccelAvg;
                text = new char[20];
                sprintf(text, "%4d", max / 10);
                FastFont::printFastRom(clockString.AccelMax, text, 20, 145, WHITE, 5, BLACK);
                clockString.AccelMax = text;
                if (clockString.AccelMax_1 != max % 10)
                {
                    clockString.AccelMax_1 = max % 10;
                    sprintf(text, "%d", max % 10);
                    FastFont::printRom(text, 140, 160, WHITE, 3, BLACK);
                }
                sprintf(text, "%4d", avg / 10);
                FastFont::printFastRom(clockString.AccelAvg, text, 180, 145, WHITE, 5, BLACK);
                clockString.AccelAvg = text;
                if (clockString.AccelAvg_1 != avg % 10)
                {
                    clockString.AccelAvg_1 = avg % 10;
                    sprintf(text, "%d", avg % 10);
                    FastFont::printRom(text, 300, 160, WHITE, 3, BLACK);
                }
                delete[] text;
                // 188
                if (clockString.UpdateAccel % 5 == 0)
                {
                    int height = 18;
                    float point = 0;
                    if (avg < 60)
                    {
                        point = 50 * avg / 60.0;
                        point = (int)point;
                        M5.Lcd.fillRect(0, 188, point, height, CYAN);
                        M5.Lcd.fillRect(point, 188, 50 - point, height, GetColor(0x003F3F));
                        // FD20
                    }
                    else
                    {
                        M5.Lcd.fillRect(0, 188, 50, height, CYAN);
                    }
                    if (avg >= 60 && avg < 200)
                    {
                        point = 50 * (avg - 60) / 140.0;
                        point = (int)point;
                        M5.Lcd.fillRect(50, 188, point, height, GREEN);
                        M5.Lcd.fillRect(50 + point, 188, 50 - point, height, GetColor(0x003F00));
                    }
                    else
                    {
                        if (avg < 60)
                        {
                            M5.Lcd.fillRect(50, 188, 50, height, GetColor(0x003F00));
                        }
                        else
                        {
                            M5.Lcd.fillRect(50, 188, 50, height, GREEN);
                        }
                    }
                    if (avg >= 200 && avg < 600)
                    {
                        point = 50 * (avg - 200) / 400.0;
                        point = (int)point;
                        M5.Lcd.fillRect(100, 188, point, height, YELLOW);
                        M5.Lcd.fillRect(100 + point, 188, 50 - point, height, GetColor(0x3F3F00));
                    }
                    else
                    {
                        if (avg < 200)
                        {
                            M5.Lcd.fillRect(100, 188, 50, height, GetColor(0x3F3F00));
                        }
                        else
                        {
                            M5.Lcd.fillRect(100, 188, 50, height, YELLOW);
                        }
                    }
                    if (avg >= 600 && avg < 2000)
                    {
                        point = 50 * (avg - 600) / 1400.0;
                        point = (int)point;
                        M5.Lcd.fillRect(150, 188, point, height, ORANGE);
                        M5.Lcd.fillRect(150 + point, 188, 50 - point, height, 0x3940);
                    }
                    else
                    {
                        if (avg < 600)
                        {
                            M5.Lcd.fillRect(150, 188, 50, height, 0x3940);
                        }
                        else
                        {
                            M5.Lcd.fillRect(150, 188, 50, height, ORANGE);
                        }
                    }
                    if (avg >= 2000 && avg < 6000)
                    {
                        point = 50 * (avg - 2000) / 4000.0;
                        point = (int)point;
                        M5.Lcd.fillRect(200, 188, point, height, RED);
                        M5.Lcd.fillRect(200 + point, 188, 50 - point, height, GetColor(0x3F0000));
                    }
                    else
                    {
                        if (avg < 2000)
                        {
                            M5.Lcd.fillRect(200, 188, 50, height, GetColor(0x3F0000));
                        }
                        else
                        {
                            M5.Lcd.fillRect(200, 188, 50, height, RED);
                        }
                    }
                    if (avg >= 6000)
                    {
                        point = 50 * (avg - 6000) / 9000.0;
                        point = (int)point;
                        M5.Lcd.fillRect(250, 188, point, height, GetColor(0xFF00FF));
                        M5.Lcd.fillRect(250 + point, 188, 50 - point, height, GetColor(0x3F003F));
                    }
                    else
                    {
                        M5.Lcd.fillRect(250, 188, 70, height, GetColor(0x3F003F));
                    }
                    if (max < 60)
                    {
                        point = 50 * max / 60.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(point, 188, height, CYAN);
                    }
                    else if (max < 200)
                    {
                        point = 50 * (max - 60) / 140.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(50 + point, 188, height, GREEN);
                    }
                    else if (max < 600)
                    {
                        point = 50 * (max - 200) / 400.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(100 + point, 188, height, YELLOW);
                    }
                    else if (max < 2000)
                    {
                        point = 50 * (max - 600) / 1400.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(150 + point, 188, height, ORANGE);
                    }
                    else if (max < 6000)
                    {
                        point = 50 * (max - 2000) / 4000.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(200 + point, 188, height, RED);
                    }
                    else if (max < 15000)
                    {
                        point = 50 * (max - 6000) / 9000.0;
                        point = (int)point;
                        M5.Lcd.drawFastVLine(250 + point, 188, height, GetColor(0xFF00FF));
                    }
                    else
                    {
                        M5.Lcd.drawFastVLine(319, 188, height, GetColor(0xFF00FF));
                    }
                }
            }
        }

        break;

    case EEWMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = 1;

            M5.Lcd.fillRect(0, 14, 320, 210, BLACK);
            if (!IsMapMode)
            {
                FastFont::printUtf8("???", 100, 27, WHITE, 1, BLACK);
                FastFont::printUtf8("???", 113 + 6 * 3 * 2, 27, WHITE, 1, BLACK);
                FastFont::printUtf8("????????????", 0, 50, WHITE, 1, BLACK);
                FastFont::printUtf8("?????????", 13, 78, WHITE, 1, BLACK);
                FastFont::printUtf8("??????", 26, 106, WHITE, 1, BLACK);
                FastFont::printUtf8("??????", 160, 106, WHITE, 1, BLACK);
                FastFont::printRom("Ping", 320 - 4 * 6, 15, WHITE, 1, BLACK);
                FastFont::printUtf8("????????????", 320 - 13 * 4, 126, WHITE, 1, BLACK);
            }
            else
            {
            }
        }

        if (!IsUpdated)
        {
            IsUpdated = 1;
            if (!IsMapMode)
            {
                IsRegionUpdate = false;
                if (json["ParseStatus"] == "Success")
                {
                    if (json["Status"]["Code"] == "00" || json["Status"]["Code"] == "01")
                    {
                        if (json["Warn"])
                        {
                            M5.lcd.fillRect(0, 15, 94, 24, RED);
                            IsRegionUpdate = true;
                            FastFont::printUtf8(" ??? ??? ", 0, 15, WHITE, 2, RED);
                        }
                        else
                        {

                            M5.lcd.fillRect(0, 15, 94, 24, BLUE);
                            FastFont::printUtf8(" ??? ??? ", 0, 15, WHITE, 2, BLUE);
                        }
                    }
                    else if (json["Status"]["Code"] == "10" || json["Status"]["Code"] == "11")
                    {
                        M5.lcd.fillRect(0, 15, 94, 24, GREEN);
                        FastFont::printUtf8(" ??? ??? ", 0, 15, WHITE, 2, GREEN);
                    }
                }
                else
                {
                    M5.lcd.fillRect(0, 15, 94, 24, BLACK);
                    FastFont::printUtf8(" ??? ??? ", 0, 15, WHITE, 2, BLACK);
                }
                if (1)
                { // json["ParseStatus"]=="Success"){
                    text = new char[80];
                    int num = json["Serial"];
                    if (num > 999)
                        num = 999;
                    if (num < -99)
                        num = -99;
                    sprintf(text, "%d", num);
                    M5.Lcd.fillRect(113, 23, 36, 16, BLACK);
                    FastFont::printRom(text, 113 + (3 - (strlen(text))) * 6, 25, WHITE, 2, BLACK);

                    M5.Lcd.fillRect(113 + 6 * 3 * 2 + 1 + 12, 27, 110, 12, BLACK);
                    if (json["Type"]["Code"] == 9)
                        FastFont::printUtf8("(?????????)", 113 + 6 * 3 * 2 + 1 + 12, 27, WHITE, 1, BLACK);
                    if (json["Status"]["Code"] == "01")
                    {
                        FastFont::printUtf8("?????????", 113 + 6 * 3 * 2 + 1 + 12 + 55, 27, WHITE, 1, BLACK);
                    }
                    else if (json["Status"]["Code"] == "11")
                    {
                        FastFont::printUtf8("????????????", 113 + 6 * 3 * 2 + 1 + 12 + 55, 27, RED, 1, BLACK);
                    }
                    FastFont::printRom(json["OriginTime"]["String"], 56, 46, WHITE, 2, BLACK);
                    M5.Lcd.fillRect(56, 66, 264, 24, BLACK);
                    FastFont::printUtf8(json["Hypocenter"]["Name"], 56, 66, WHITE, 2, -1);
                    FastFont::printRom(json["Hypocenter"]["Magnitude"]["String"], 56, 97, WHITE, 3, BLACK);
                    num = json["Hypocenter"]["Location"]["Depth"]["Int"];
                    m5.lcd.fillRect(194, 94, 126, 24, BLACK);
                    switch (num)
                    {
                    case 0:
                        FastFont::printUtf8("????????????", 194, 94, WHITE, 2, BLACK);
                        break;
                    case -1:
                        FastFont::printUtf8("??????", 194, 94, WHITE, 2, BLACK);
                        break;
                    default:
                        sprintf(text, "%d km", num);
                        FastFont::printRom(text, 194, 97, WHITE, 3, BLACK);
                        break;
                    }
                    int col;
                    int textc = WHITE;
                    String str = json["MaxIntensity"]["To"];
                    int x0 = 6;
                    if (str == "1")
                    {
                        col = GetColor(0x808080);
                    }
                    else if (str == "2")
                    {
                        col = GetColor(0x4169E1);
                    }
                    else if (str == "3")
                    {
                        col = GetColor(0x2E8B57);
                    }
                    else if (str == "4")
                    {
                        col = GetColor(0xbfbf0f);
                    }
                    else if (str == "5-")
                    {
                        x0 = 13;
                        col = GetColor(0xFF4500);
                    }
                    else if (str == "5+")
                    {
                        x0 = 13;
                        col = GetColor(0xFF4500);
                    }
                    else if (str == "6-")
                    {
                        x0 = 13;
                        x0 = 42 - (13 * 3) / 2;
                        col = GetColor(0xFFC0CB);
                    }
                    else if (str == "6+")
                    {
                        textc = RED;
                        x0 = 13;
                        col = GetColor(0xFFC0CB);
                    }
                    else if (str == "7")
                    {
                        x0 = 13;
                        col = GetColor(0x800080);
                    }
                    else
                    {
                        x0 = 6;
                        str = "-";
                        col = GetColor(0x808080);
                    }
                    regionUpdate = millis();
                    regPos = -1;
                    pg = 1;
                    m5.Lcd.fillRect(268, 140, 51, 51, col);
                    FastFont::printUtf8(str, 268 + (51 - x0 * 3) / 2 + 2, 140 + (42 - 24) / 2, textc, 3, col);
                    int x1 = 2;
                    int y1 = 142;
                    M5.lcd.fillRect(0, 127, 267, 82, BLACK);
                    WarnRegionDisplay = false;
                }
            }
        }

        if (!WarnRegionDisplay)
        {
            switch (mode)
            {
            case EEWMode:
                if (!IsMapMode)
                {
                    WarnRegionDisplay = true;
                    text = new char[100];
                    int x1 = 2;
                    int y1 = 142;
                    if (config.AutoModeRotate&&regPos < -100 && !IsAutoRotateStop)
                    {
                        regPos = -1;
                        sellectMode = MapMode;
                        MapSize = 3;
                        ModeEnter();
                        return;
                    }
                    else if (regPos < 0)
                    {
                        regPos = -1;
                    }
                    M5.lcd.fillRect(0, 127, 267, 82, BLACK);
                    if (IsRegionUpdate)
                    {
                        M5.lcd.drawRect(0, 140, 267, 69, json["Warn"] ? RED : BLACK);
                        if (regPos == -1)
                        {
                            M5.lcd.fillRect(0, 127, 267, 13, RED);
                            sprintf(text, "???????????????????????????????????????      ????????????");
                            FastFont::printUtf8(text, 1, 128, WHITE, 1, RED);
                            for (int i = 0;; i++)
                            {
                                String name = json["WarnForecast"]["LocalAreas"][i];
                                if (name == "null")
                                {
                                    break;
                                }
                                int x = name.length() / 3 * 13 + 6;
                                if (x1 + x > 265)
                                {
                                    x1 = 2;
                                    y1 += 13;
                                    if (y1 >= 207)
                                    {
                                        break;
                                    }
                                }
                                FastFont::printUtf8(name, x1, y1, WHITE, 1, BLACK);
                                x1 += x;
                            }
                            pg = 1;
                            regPos = 0;
                        }
                        else
                        {
                            M5.lcd.fillRect(0, 127, 267, 13, RED);
                            sprintf(text, "???????????????????????????????????????        ??????%2d", pg);
                            FastFont::printUtf8(text, 1, 128, WHITE, 1, RED);
                            for (int i = regPos;; i++)
                            {
                                String name = json["WarnForecast"]["Regions"][i];
                                if (name == "null")
                                {
                                    regPos = -101;
                                    break;
                                }
                                int x = name.length() / 3 * 13 + 6;
                                if (x1 + x > 265)
                                {
                                    x1 = 2;
                                    y1 += 13;
                                    if (y1 >= 207)
                                    {
                                        regPos = i;
                                        pg++;
                                        break;
                                    }
                                }
                                FastFont::printUtf8(name, x1, y1, WHITE, 1, BLACK);
                                x1 += x;
                            }
                        }
                    }
                    delete[] text;
                }
                break;
            }
        }
        if (!IsPingUpdate)
        {
            if (!IsMapMode)
            {

                IsPingUpdate = true;
                text = new char[12];
                sprintf(text, "%d", JsonReadTime > 9999 ? 9999 : JsonReadTime);
                int col = 0;
                if (JsonReadTime > 500)
                {
                    col = RED;
                }
                else if (JsonReadTime > 100)
                {
                    col = YELLOW;
                }
                else
                {
                    col = GREEN;
                }
                m5.Lcd.fillRect(320 - 4 * 6, 23, 24, 8, BLACK);
                FastFont::printRom(text, 320 - 4 * 6 + 3 * (4 - strlen(text)), 23, col, 1, -1);
                delete[] text;
            }
        }
        text = new char[40];
        sprintf(text, "Max%4d.%d", Core::SystemAPI::AccelMax / 10, Core::SystemAPI::AccelMax % 10);
        FastFont::printFastRom(TempMaxGal, text, 320 - 9 * 6+1, 192, WHITE, 1, BLACK);
        TempMaxGal = text;
        sprintf(text, "Avg%4d.%d", Core::SystemAPI::AccelAvg / 10, Core::SystemAPI::AccelAvg % 10);
        FastFont::printFastRom(TempAvgGal, text, 320 - 9 * 6+1, 200, WHITE, 1, BLACK);
        TempAvgGal = text;
        delete[] text;
        break;
    case MapMode:

        if (!IsFirstDrawed)
        {
            IsFirstDrawed = true;
        }
        if (!IsUpdated)
        {
            IsUpdated = true;
            bool IsWarn = json["Warn"];
            map.Draw(json["Hypocenter"]["Location"]["Long"], json["Hypocenter"]["Location"]["Lat"], MapSize);
            if (IsWarn)
            {
                for (int ii = 1; ii < 10; ii++)
                {
                    for (int i = 0;; i++)
                    {
                        int code = json["Forecast"][i]["Intensity"]["Code"];
                        if (code != 0)
                        {
                            String val = json["Forecast"][i]["Intensity"]["From"];
                            if (map.GetShindoName(ii) == val)
                                map.DrawShindo(code, val);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            map.DrawHypocenter(json["Hypocenter"]["Location"]["Long"], json["Hypocenter"]["Location"]["Lat"]);
            text = new char[128];
            sprintf(text, "??? %d ???", (int)json["Serial"]);
            if (json["Warn"])
            {
                FastFont::printUtf8("[??????]", 0, 14, WHITE, 1, RED);
            }
            else
            {
                FastFont::printUtf8("[??????]", 0, 14, WHITE, 1, BLUE);
            }
            if (json["Type"]["Code"] == 9)
                sprintf(text, "??? %d ???(?????????)", (int)json["Serial"]);
            FastFont::printUtf8(text, 40, 14, WHITE, 1, BLACK);
            M5.Lcd.fillRect(0, 210, 320, 14, BLACK);
            FastFont::printUtf8(json["Hypocenter"]["Name"], 0, 211, WHITE, 1, INVISIBLE_COLOR);
            FastFont::printUtf8(json["Hypocenter"]["Magnitude"]["String"], 140, 211, WHITE, 1, INVISIBLE_COLOR);
            sprintf(text, "??????:%3d km", (int)json["Hypocenter"]["Location"]["Depth"]["Int"]);
            FastFont::printUtf8(text, 177, 211, WHITE, 1, INVISIBLE_COLOR);
            String str = json["MaxIntensity"]["To"];
            sprintf(text, "??????:%s", str.c_str());
            FastFont::printUtf8(text, 262, 211, WHITE, 1, INVISIBLE_COLOR);
            FastFont::printUtf8(json["OriginTime"]["String"], 320 - 7 * 19, 14, WHITE, 1, BLACK);
            if((bool)json["Hypocenter"]["isSea"]
            &&(float)json["Hypocenter"]["Magnitude"]["Float"]>=6.0
            &&(int)json["Hypocenter"]["Location"]["Depth"]["Int"]<=40
            &&(int)json["Hypocenter"]["Location"]["Depth"]["Int"]!=0){
                FastFont::printUtf8("   ????????????????????????????????????????????????",0,198,YELLOW,1,BLACK);
                FastFont::printUtf8("[!]",0,198,RED,1,INVISIBLE_COLOR);
            }
            M5.Lcd.fillRect(320-13*6,195,13*6,16, BLACK);
            TempAvgGal="         ";
            TempMaxGal="         ";
            delete[] text;
        }
        text = new char[40];
        sprintf(text, "Max:%4d.%dgal", Core::SystemAPI::AccelMax / 10, Core::SystemAPI::AccelMax % 10);
        FastFont::printFastRom(TempMaxGal, text, 320 - 13 * 6, 195, WHITE, 1, BLACK);
        TempMaxGal = text;
        sprintf(text, "Avg:%4d.%dgal", Core::SystemAPI::AccelAvg / 10, Core::SystemAPI::AccelAvg % 10);
        FastFont::printFastRom(TempAvgGal, text, 320 - 13 * 6, 203, WHITE, 1, BLACK);
        TempAvgGal = text;
        delete[] text;

        break;

    case PingMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = 1;
            FastFont::printRom("10s", 0, 24, RED);
            FastFont::printRom(" 1s", 0, 74 - 4, RED);
            FastFont::printRom("500", 0, 124 - 4, YELLOW);
            FastFont::printRom("100", 0, 174 - 4, GREEN);

            FastFont::printRom("  0", 0, 224 - 8, GREEN);

            M5.Lcd.drawRect(19, 23, 302, 202, WHITE);
            FastFont::printRom("Ping:", 320 - 6 * 11 + 1, 14, WHITE);
            FastFont::printRom("Mode:", 20, 14, WHITE);
            switch (pingGraphMode)
            {
            case Ping_60secondsMode:
                FastFont::printRom("60sec", 50, 14, GREEN, 1, BLACK);
                break;
            case Ping_5minsMode:
                FastFont::printRom(" 5min", 50, 14, YELLOW, 1, BLACK);
                break;
            case Ping_1hoursMode:
                FastFont::printRom("60min", 50, 14, RED, 1, BLACK);
                break;
            case Ping_5hoursMode:
                FastFont::printRom("8.3hr", 50, 14, GetColor(0xFF00FF), 1, BLACK);
                break;
            }
            FastFont::printRom("Rcv:", 86, 14, WHITE, 1);
            FastFont::printRom("Avg:", 164, 14, WHITE, 1);
        }
        if (!IsPingUpdate)
        {
            IsPingUpdate = 1;

            for (int i = 0; i < 300; i++)
            {
                int val = 0;
                int data = 0;
                int graph_x_lightgray = 0;
                int graph_x_gray = 0;
                int graph_x_loght_offset = 0;
                switch (pingGraphMode)
                {
                case Ping_60secondsMode:
                    if (i % 5 == 4)
                    {
                        val = PingValue[PingData - 1 - 60 + (i + 1) / 5];
                    }
                    else
                    {
                        int m1 = PingValue[PingData - 1 - 60 + (i + 1) / 5];
                        int m2 = PingValue[PingData - 1 - 60 + (i + 1) / 5 + 1];
                        double v = m2 - m1;
                        v /= 5;
                        v *= (i + 1) % 5;
                        val = m1 + v;
                    }
                    graph_x_gray = 25;
                    graph_x_lightgray = 75;
                    break;
                case Ping_5minsMode:
                    val = PingValue[i];
                    graph_x_gray = 20;
                    graph_x_lightgray = 60;
                    break;
                case Ping_1hoursMode:
                    val = PingValue12sec[i];
                    graph_x_gray = 25;
                    graph_x_lightgray = 50;
                    break;
                case Ping_5hoursMode:
                    val = PingValue60sec[i];
                    graph_x_gray = 12;
                    graph_x_lightgray = 36;
                    graph_x_loght_offset = 12;
                    break;
                }
                data = val;
                if (val > 9999)
                    val = 9999;
                if (val < 0)
                    val = 0;
                if (data >= 1000)
                { // 10000?????? 9000/180 50
                    val -= 1000;
                    val /= 180;
                    val += 150;

                    M5.Lcd.drawFastVLine(20 + i, 24 + (200 - val), val, RED);
                }
                else if (data >= 500)
                { // 1000?????? 500/10 50
                    val -= 500;
                    val /= 10;
                    val += 100;

                    M5.Lcd.drawFastVLine(20 + i, 24 + (200 - val), val, RED);
                }
                else if (data >= 100)
                { // 500??????  400/8 50
                    val -= 100;
                    val /= 8;
                    val += 50;

                    M5.Lcd.drawFastVLine(20 + i, 24 + (200 - val), val, YELLOW);
                }
                else
                { // 100?????? /2 +50
                    val /= 2;
                    M5.Lcd.drawFastVLine(20 + i, 24 + (200 - val), val, GREEN);
                }
                if (i % graph_x_lightgray == graph_x_loght_offset)
                {
                    M5.Lcd.drawFastVLine(20 + i, 24, 200 - val, GetColor(0x808080));
                }
                else if (i % graph_x_gray == 0)
                {
                    M5.Lcd.drawFastVLine(20 + i, 24, 200 - val, GetColor(0x404040));
                }
                else
                {
                    M5.Lcd.drawFastVLine(20 + i, 24, 200 - val, BLACK);
                }
                for (float pi = 0; pi < 50; pi += 10)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 24 + (199 - pi), GetColor(0x404040));
                for (float pi = 50; pi < 100; pi += 12.5)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 24 + (199 - pi), GetColor(0x404040));
                for (float pi = 100; pi < 150; pi += 10)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 24 + (199 - pi), GetColor(0x404040));
                for (float pi = 150; pi < 200; pi += 5.55555555555555555555556)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 24 + (199 - pi), GetColor(0x404040));
                if (50 > val)
                    M5.Lcd.drawPixel(20 + i, 24 + (199 - 50), GetColor(0x208020));
                if (100 > val)
                    M5.Lcd.drawPixel(20 + i, 24 + (199 - 100), GetColor(0x808020));
                if (150 > val)
                    M5.Lcd.drawPixel(20 + i, 24 + (199 - 150), GetColor(0x802020));
            }
            text = new char[20];
            short col;
            sprintf(text, "%4dms", JsonReadTime);
            if (JsonReadTime >= 10000)
            {
                col = RED;
                sprintf(text, "----ms");
            }
            else if (JsonReadTime >= 500)
            {
                col = RED;
            }
            else if (JsonReadTime >= 100)
            {
                col = YELLOW;
            }
            else
            {
                col = GREEN;
            }
            FastFont::printRom(text, 320 - 6 * 6 + 1, 14, col, 1, BLACK);
            sprintf(text, "%8d", (PingCount > 99999999 ? 99999999 : PingCount));
            FastFont::printRom(text, 110, 14, GREEN, 1, BLACK);
            uint32_t total = 0;
            short cnt = 0;
            for (int i = 0; i < PingData; i++)
            {
                if (PingValue[i] != 0)
                {
                    total += PingValue[i] * 100;

                    cnt++;
                }
            }
            if (cnt != 0)
            {
                total /= cnt;
            }
            else
            {
                total = 0;
            }
            sprintf(text, "%4d.%02dms", total / 100, total % 100);

            if (total >= 50000)
            {
                col = RED;
            }
            else if (total >= 10000)
            {
                col = YELLOW;
            }
            else if (total != 0)
            {
                col = GREEN;
            }
            else
            {
                col = GetColor(0xFF00FF);
            }
            FastFont::printRom(text, 188, 14, col, 1, BLACK);
            delete[] text;
        }
        break;
    case SettingMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = true;
        }
        if (!IsSettingUIUpdate)
        {
            IsSettingUIUpdate = true;
            char *t = new char[100];
            for (int i = 0; i <= ExitSetting + 1; i++)
            {
                switch (i)
                {
                case ForecastSoundPath:
                    sprintf(t, "?????????????????????????????? : %s", config.ForecastSoundPath.c_str());
                    break;
                case ForecastSoundPerSerial:
                    if (config.ForecastSoundPerSerial)
                    {
                        sprintf(t, "???????????????????????? : ??????");
                    }
                    else
                    {
                        sprintf(t, "???????????????????????? : ??????");
                    }
                    break;
                case WarnSoundPath:
                    sprintf(t, "?????????????????????????????? : %s", config.WarnSoundPath.c_str());
                    break;
                    break;
                case WarnSoundPerSerial:
                    if (config.WarnSoundPerSerial)
                    {
                        sprintf(t, "???????????????????????? : ??????");
                    }
                    else
                    {
                        sprintf(t, "???????????????????????? : ??????");
                    }
                    break;
                case OnlyListEvent:
                    sprintf(t, "?????????????????????(????????????) : %s", config.OnlyListEvent.c_str());
                    break;
                case LCDoffTimer:
                    if (config.LCDoffTimer == 0)
                    {
                        sprintf(t, "LCD??????OFF(???????????????) : ??????");
                    }
                    else
                    {
                        sprintf(t, "LCD??????OFF(???????????????) : %5d??????", config.LCDoffTimer);
                    }
                    break;
                case LCDoffTimerBattery:
                    if (config.LCDoffTimerBattery == 0)
                    {
                        sprintf(t, "LCD??????OFF(???????????????) : ??????");
                    }
                    else
                    {
                        sprintf(t, "LCD??????OFF(???????????????) : %5d??????", config.LCDoffTimerBattery);
                    }
                    break;
                case RebootTimer:
                    if (config.RebootTimer == 0)
                    {
                        sprintf(t, "????????????????????? : ??????");
                    }
                    else
                    {
                        sprintf(t, "????????????????????? : %5d??????", config.RebootTimer);
                    }
                    break;
                case LCDLightLvBattery:
                    sprintf(t, "??????????????????LCD???????????? : %3d", config.LCDLightLvBattery);
                    break;
                case LCDLightLvSupply:
                    sprintf(t, "??????????????????LCD???????????? : %3d", config.LCDLightLvSupply);
                    break;
                case AutoChangeEEW:
                    if (config.AutoChangeEEW)
                        sprintf(t, "EEW?????????????????????????????? : ??????");
                    else
                        sprintf(t, "EEW?????????????????????????????? : ??????");
                    break;
                case AutoModeRotate:
                    if (config.AutoModeRotate)
                        sprintf(t, "EEW????????????????????????????????? : ??????");
                    else
                        sprintf(t, "EEW????????????????????????????????? : ??????");
                    break;
                case AutoChangeMode:
                    if (config.AutoChangeMode)
                        sprintf(t, "EEW??????????????????????????????????????? : ????????????");
                    else
                        sprintf(t, "EEW??????????????????????????????????????? : ????????????");
                    break;
                case ExitSetting:
                    sprintf(t, "???????????????????????????");
                    break;
                case ExitAndCancel:
                    sprintf(t, "??????????????????????????????????????????");

                    break;
                case TestMode:
                    sprintf(t, "???????????????(60????????????????????????????????????) : %s", TestTime == 0 ? "??????" : "??????");
                    break;
                }
                if (i == TestMode)
                {
                    if (TestTime == 0)
                    {
                        M5.lcd.fillRect(0, 20 + 12 * i, 320, 12, settingSellect == i && IsNotCursorMode ? RED : BLACK);
                        FastFont::printUtf8(t, 0, 20 + 12 * i, settingSellect == i && IsNotCursorMode ? BLACK : RED, 1, INVISIBLE_COLOR);
                    }
                    else
                    {
                        M5.lcd.fillRect(0, 20 + 12 * i, 320, 12, settingSellect == i && IsNotCursorMode ? GREEN : BLACK);
                        FastFont::printUtf8(t, 0, 20 + 12 * i, settingSellect == i && IsNotCursorMode ? BLACK : GREEN, 1, INVISIBLE_COLOR);
                    }
                }
                else
                {
                    M5.lcd.fillRect(0, 20 + 12 * i, 320, 12, settingSellect == i && IsNotCursorMode ? WHITE : BLACK);
                    FastFont::printUtf8(t, 0, 20 + 12 * i, settingSellect == i && IsNotCursorMode ? BLACK : WHITE, 1, INVISIBLE_COLOR);
                }
            }
            delete[] t;
        }
        break;
    }
    switch (mode)
    {
    case SettingNum:
        cNum.Draw();
        break;
    case SettingList:
        cList.Draw();
        break;
    case SettingPath:
        cPath.Draw();
        break;
    }
    if (!IsButtonUIUpdate && mode >= 0)
    {
        IsButtonUIUpdate = true;
        M5.Lcd.fillRect(0, 224, 320, 16, BLACK);
        M5.Lcd.drawFastHLine(0, 224, 320, WHITE);
        if (!IsNotCursorMode)
        {
            int c = 0;
            for (int i = sellectMode - 1; i <= sellectMode + 1; i++)
            {
                int x = 19 + c * 94;
                int fillx = 0;
                String FcName = "";
                switch (i)
                {
                case ExitMode:
                    x += (100 - 25) / 2;
                    fillx = 25;
                    FcName = "??????";
                    break;
                case ClockMode:
                    x += (100 - 51) / 2;
                    fillx = 51;
                    FcName = "????????????";
                    break;
                case EEWMode:
                    x += (100 - 46) / 2;
                    fillx = 46;
                    FcName = "EEW??????";
                    break;
                case MapMode:
                    x += (100 - 46) / 2;
                    fillx = 46;
                    FcName = "EEW??????";
                    break;
                case PingMode:
                    x += (100 - 77) / 2;
                    fillx = 77;
                    FcName = "??????????????????";
                    break;
                case SettingMode:
                    x += (100 - 25) / 2;
                    fillx = 25;
                    FcName = "??????";
                    break;
                
                }
                if (i == mode)
                {
                    M5.Lcd.fillRect(x - 1, 225, fillx + 2, 14, WHITE);
                    FastFont::printUtf8(FcName, x, 226, BLACK, 1, INVISIBLE_COLOR);
                }
                else
                {
                    FastFont::printUtf8(FcName, x, 226, WHITE, 1, INVISIBLE_COLOR);
                }
                c++;
            }
        }
        else
        {

            FastFont::printUtf8("???", 19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);

            FastFont::printUtf8("??????", 19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);

            FastFont::printUtf8("???", 19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        }
    }
}
void EEW::ModeEnter()
{
    if (mode == sellectMode)
    {
        switch (mode)
        {
        case EEWMode:
            break;
        case MapMode:
            MapSize++;
            if (MapSize > 5)
                MapSize = 1;
            IsFirstDrawed = false;
            IsUpdated = false;
            break;
        case PingMode:
            pingGraphMode++;
            if (pingGraphMode > Ping_5hoursMode)
                pingGraphMode = Ping_60secondsMode;
            IsFirstDrawed = false;
            IsUpdated = false;
            IsPingUpdate = false;
            break;
        case SettingMode:
            IsNotCursorMode = true;
            IsButtonUIUpdate = false;
            settingSellect = 0;
            break;
        }
    }

    if (sellectMode == SettingMode)
    {
        IsSettingUIUpdate = false;
    }
    if (mode == sellectMode)
        return;

    mode = sellectMode;
    if (mode == ExitMode)
    {
        toHome = 1;
        return;
    }
    M5.Lcd.fillRect(0, 14, 320, 226, BLACK);

    Core::SystemData::UpdateBatteryUI = 1;
    Core::SystemData::UpdateSignalUI = 1;

    TempMaxGal = "               ";
    TempAvgGal = "               ";
    IsPingUpdate = false;
    IsButtonUIUpdate = false;
    IsFirstDrawed = false;
    IsUpdated = false;
}
void EEW::Exit()
{
    RunThread = 0;
    while (!FinishedThread)
        delay(1);
    json.clear();
    http[0].end();
    http[0].~HTTPClient();
    delete http;
    if (SPIFFS.begin(0))
    {

        fs::FS fs = SPIFFS;
        if (fs.remove("/MisakiEQ.flg"))
        {
        }
        else
        {
            Serial.println("Error! Cannot remove!");
        }
        SPIFFS.end();
    }
    PingSave();
    IsPingOpen = false;
    delete[] PingValue;
    delete[] PingValue12sec;
    delete[] PingValue60sec;
}
bool EEW::GetActive()
{
    return IsActive;
}
void EEW::GetNetworkFile(void *args)
{
    while (RunThread)
    {
        int start = millis();
        if (WiFi.status() == WL_CONNECTED)
        {
            http[0].begin("https://api.iedred7584.com/eew/json/");

            int httpcode = http[0].GET();
            if (httpcode < 0)
            {
                IsHttpError = true;
                LatestHttpError = http[0].errorToString(httpcode);
            }
            else
            {
                switch (httpcode)
                {
                case 200:
                    if (TestTime != 0)
                    {
                        if (millis() > TestTime && millis() < TestTime + 30000)
                        {
                            deserializeJson(json, TestJson);
                        }
                        else if (millis() > TestTime + 30000)
                        {
                            TestTime = 0;
                            deserializeJson(json, http[0].getString());
                        }
                        else
                        {
                            deserializeJson(json, http[0].getString());
                        }
                    }
                    else
                    {
                        deserializeJson(json, http[0].getString());
                    }
                    break;
                default:
                    LatestHttpError = httpcode;
                    break;
                }
            }
            http[0].end();
        }
        int t = millis();
        JsonReadTime = t - start;
        if (WiFi.status() != WL_CONNECTED)
            JsonReadTime = 0;
        if (IsPingOpen)
        {
            for (int i = 1; i < PingData; i++)
                PingValue[i - 1] = PingValue[i];
            PingValue[PingData - 1] = JsonReadTime;
            PingCount++;

            int calcCount = 0;
            int calcData = 0;
            if (PingCount % 12 == 11)
            {
                for (int i = PingData - 1 - 12; i < PingData; i++)
                {
                    if (PingValue[i] != 0)
                    {
                        calcData += PingValue[i];
                        calcCount++;
                    }
                }
                for (int i = 1; i < PingData; i++)
                    PingValue12sec[i - 1] = PingValue12sec[i];
                if (calcCount != 0)
                {
                    PingValue12sec[PingData - 1] = calcData / calcCount;
                }
                else
                {
                    PingValue12sec[PingData - 1] = 0;
                }
            }
            if (PingCount % 100 == 99)
            {
                for (int i = PingData - 1 - 100; i < PingData; i++)
                {
                    if (PingValue[i] != 0)
                    {
                        calcData += PingValue[i];
                        calcCount++;
                    }
                }
                for (int i = 1; i < PingData; i++)
                    PingValue60sec[i - 1] = PingValue60sec[i];
                if (calcCount != 0)
                {
                    PingValue60sec[PingData - 1] = calcData / calcCount;
                }
                else
                {
                    PingValue60sec[PingData - 1] = 0;
                }
            }
        }
        LatestReadTime = t;
        IsPingUpdate = 0;
        IsCheck = 0;
        while (millis() - start < 1000)
            vTaskDelay(1);
    }
    FinishedThread = 1;
    vTaskDelete(NULL);
    vTaskDelay(250);
}
int EEW::LoadTime()
{
    return JsonReadTime;
}
bool EEW::GetDrawUpdate()
{
    if (mode >= 0)
    {
        if (mode == 1 || mode == 2)
            return true;
        return !IsUpdated || !IsFirstDrawed || !WarnRegionDisplay || !IsButtonUIUpdate || !IsPingUpdate;
    }
    else
    {
        switch (mode)
        {
        case SettingNum:
            return cNum.GetIsUpdate();
            break;
        case SettingList:
            return cList.GetIsUpdate();
            break;
        case SettingPath:
            return cPath.GetIsUpdate();
            break;
        }
    }
    return 0;
}
String EEW::GetAppName()
{
    return "MisakiEQ";
}
bool EEW::IsHome()
{
    return toHome;
}
void EEW::PressButton(int type)
{
    if (!IsnotLCDLight)
    {

        IsUserPressed = true;
        IsAutoRotateStop = true;
        if (mode >= 0)
        {
            if (!IsNotCursorMode)
            {
                switch (type)
                {
                case 1:
                    if (sellectMode > -1)
                        sellectMode--;
                    IsButtonUIUpdate = 0;
                    break;
                case 2:
                    //???????????????
                    ModeEnter();
                    break;
                case 3:
                    if (sellectMode < 4)
                        sellectMode++;
                    IsButtonUIUpdate = 0;
                    break;
                }
            }
            else
            {
                switch (type)
                {
                case 1:
                    if (settingSellect > 0)
                        settingSellect--;
                    break;
                case 2:
                    //??????????????????
                    SettingEnter();
                    IsButtonUIUpdate = false;
                    break;
                case 3:
                    if (settingSellect < ExitSetting + 1)
                        settingSellect++;
                    break;
                }
                IsSettingUIUpdate = false;
            }
        }
        else
        {
            switch (mode)
            {
            case SettingNum:
                cNum.Button(type);
                break;
            case SettingList:
                cList.Button(type);
                break;
            case SettingPath:
                cPath.Button(type);
                break;
            }
        }
    }
    LCDTimer = millis();
}
void EEW::SettingEnter()
{
    switch (settingSellect)
    {
    case ForecastSoundPath:
        cPath.Begin(&config.ForecastSoundPath, ".wav");
        sellectMode = SettingPath;
        cPath.SetTitle("?????????????????????????????????", "???????????????????????????(??????)????????????????????????");
        ModeEnter();
        break;
    case ForecastSoundPerSerial:
        config.ForecastSoundPerSerial = !config.ForecastSoundPerSerial;
        IsUpdated = false;
        break;
    case WarnSoundPath:
        cPath.Begin(&config.WarnSoundPath, ".wav");
        sellectMode = SettingPath;
        cPath.SetTitle("?????????????????????????????????", "???????????????????????????????????????????????????????????????");
        ModeEnter();
        break;
    case WarnSoundPerSerial:
        config.WarnSoundPerSerial = !config.WarnSoundPerSerial;
        IsUpdated = false;
        break;
    case OnlyListEvent:
        cList.Begin(&config.OnlyListEvent, PrefList, 48);
        sellectMode = SettingList;
        cList.SetTitle("????????????", "????????????????????????????????????????????????????????????????????????");
        ModeEnter();
        break;
    case LCDoffTimer:
        cNum.Begin(&config.LCDoffTimer, 5);
        sellectMode = SettingNum;
        cNum.SetTitle("LCD????????????????????????(???)", "???????????????????????????????????????????????????????????????????????????\n????????????????????????(?????????=0?????????)");
        ModeEnter();
        break;
    case LCDoffTimerBattery:
        cNum.Begin(&config.LCDoffTimerBattery, 5);
        sellectMode = SettingNum;
        cNum.SetTitle("LCD????????????????????????(???)", "???????????????????????????????????????????????????????????????????????????\n?????????????????????????????????(?????????=0?????????)");
        ModeEnter();
        break;
    case RebootTimer:
        cNum.Begin(&config.RebootTimer, 5);
        sellectMode = SettingNum;
        cNum.SetTitle("???????????????????????????(???)", "???????????????????????????????????????????????????????????????\n??????????????????????????????????????????????????????\n(?????????=0?????????)");
        ModeEnter();
        break;
    case LCDLightLvSupply:
        cNum.Begin(&config.LCDLightLvSupply, 3);
        cNum.SetMin(1);
        cNum.SetMax(255);
        sellectMode = SettingNum;
        cNum.SetTitle("???????????????LCD????????????", "??????????????????LCD?????????????????????????????????\n(1???255?????????????????????????????????");
        ModeEnter();
        break;
    case LCDLightLvBattery:
        cNum.Begin(&config.LCDLightLvBattery, 3);
        cNum.SetMin(1);
        cNum.SetMax(255);
        sellectMode = SettingNum;
        cNum.SetTitle("???????????????LCD????????????", "??????????????????LCD?????????????????????????????????\n(1???255?????????????????????????????????");
        ModeEnter();
        break;
    case AutoChangeEEW:
        config.AutoChangeEEW=!config.AutoChangeEEW;
        IsUpdated = false;
        break;
    case AutoModeRotate:
        config.AutoModeRotate=!config.AutoModeRotate;
        IsUpdated = false;
        break;
    case AutoChangeMode:
        config.AutoChangeMode=!config.AutoChangeMode;
        IsUpdated = false;
        break;
    case ExitSetting:
        SaveConfig();
        IsNotCursorMode = false;
        break;
    case ExitAndCancel:
        ReadConfig();
        IsNotCursorMode = false;
        break;
    case TestMode:
        TestTime = millis() + 60000;
        break;
    }
}
void EEW::SaveConfig()
{
    SPIFFS.begin(true);

    fs::FS fs = SPIFFS;
    if (!fs.exists("/config"))
        fs.mkdir("/config");
    if (fs.exists("/config/MisakiEQ.cfg"))
        fs.remove("/config/MisakiEQ.cfg");
    File configFile = fs.open("/config/MisakiEQ.cfg", FILE_WRITE);
    if (!configFile)
        return;
    configFile.printf("ForecastSoundPath=%s\n", config.ForecastSoundPath.c_str());
    configFile.printf("ForecastSoundPerSerial=%s\n", BoolToStr(config.ForecastSoundPerSerial).c_str());
    configFile.printf("WarnSoundPath=%s\n", config.WarnSoundPath.c_str());
    configFile.printf("WarnSoundPerSerial=%s\n", BoolToStr(config.WarnSoundPerSerial).c_str());
    configFile.printf("OnlyListEvent=%s\n", config.OnlyListEvent.c_str());
    configFile.printf("LCDoffTimer=%d\n", config.LCDoffTimer);
    configFile.printf("LCDoffTimerBattery=%d\n", config.LCDoffTimerBattery);
    configFile.printf("RebootTimer=%d\n", config.RebootTimer);
    configFile.printf("LCDLightLvSupply=%d\n", config.LCDLightLvSupply);
    configFile.printf("LCDLightLvBattery=%d\n", config.LCDLightLvBattery);
    configFile.printf("AutoChangeEEW=%s\n",BoolToStr(config.AutoChangeEEW).c_str());
    configFile.printf("AutoModeRotate=%s\n",BoolToStr(config.AutoModeRotate).c_str());
    configFile.printf("AutoChangeMode=%s\n",BoolToStr(config.AutoChangeMode).c_str());
    configFile.close();
    SPIFFS.end();
}
String EEW::BoolToStr(bool val)
{
    if (val)
        return "true";
    return "false";
}
void EEW::ReadConfig()
{
    SPIFFS.begin(true);
    fs::FS fs = SPIFFS;
    File configFile = fs.open("/config/MisakiEQ.cfg", FILE_READ);
    if (!configFile || !fs.exists("/config/MisakiEQ.cfg"))
    {
        config.OnlyListEvent = "???????????????";
        config.LCDLightLvBattery = 50;
        config.LCDLightLvSupply = 100;
        SPIFFS.end();
        return;
    }
    String *tmps = new String[2];
    for (int i = 0; i < 999; i++)
    {
        if (!configFile.available())
            break;
        String tmp = configFile.readStringUntil('\n');
        if (tmp.length() == 0)
            continue;
        if (tmp.endsWith("\n"))
            tmp.remove(tmp.length() - 1, 1);
        tmps[0].clear();
        tmps[1].clear();
        int splits = Split(tmp, '=', tmps);
        if (splits == 2)
        {

            tmp = tmps[1];
            switch (GetConfigName(tmps[0]))
            {
            case ForecastSoundPath:
                config.ForecastSoundPath = tmp;
                break;
            case ForecastSoundPerSerial:
                tmp == "true" ? config.ForecastSoundPerSerial = 1 : config.ForecastSoundPerSerial = 0;
                break;
            case WarnSoundPath:
                config.WarnSoundPath = tmp;
                break;
            case WarnSoundPerSerial:
                tmp == "true" ? config.WarnSoundPerSerial = 1 : config.WarnSoundPerSerial = 0;
                break;
            case OnlyListEvent:
                config.OnlyListEvent = tmp;
                break;
            case LCDoffTimer:
                config.LCDoffTimer = tmp.toInt();
                break;
            case LCDoffTimerBattery:
                config.LCDoffTimerBattery = tmp.toInt();
                break;
            case RebootTimer:
                config.RebootTimer = tmp.toInt();
                break;
            case LCDLightLvSupply:
                config.LCDLightLvSupply = tmp.toInt();
                break;
            case LCDLightLvBattery:
                config.LCDLightLvBattery = tmp.toInt();
                break;
            case AutoChangeEEW:
                tmp == "true" ? config.AutoChangeEEW = 1 : config.AutoChangeEEW = 0;
                break;
            case AutoModeRotate:
                tmp == "true" ? config.AutoModeRotate = 1 : config.AutoModeRotate = 0;
                break;
            case AutoChangeMode:
                tmp == "true" ? config.AutoChangeMode = 1 : config.AutoChangeMode = 0;
                break;
            }
        }
    }

    delete[] tmps;
    tmps = nullptr;
    if (config.OnlyListEvent == "")
        config.OnlyListEvent = "???????????????";
    if (config.LCDLightLvBattery < 1)
        config.LCDLightLvBattery = 100;
    if (config.LCDLightLvSupply < 1)
        config.LCDLightLvSupply = 100;
    configFile.close();
    SPIFFS.end();
}
int EEW::GetConfigName(String str)
{
    if (str == "ForecastSoundPath")
        return ForecastSoundPath;
    if (str == "ForecastSoundPerSerial")
        return ForecastSoundPerSerial;
    if (str == "WarnSoundPath")
        return WarnSoundPath;
    if (str == "WarnSoundPerSerial")
        return WarnSoundPerSerial;
    if (str == "OnlyListEvent")
        return OnlyListEvent;
    if (str == "LCDoffTimer")
        return LCDoffTimer;
    if (str == "LCDoffTimerBattery")
        return LCDoffTimerBattery;
    if (str == "RebootTimer")
        return RebootTimer;
    if (str == "LCDLightLvSupply")
        return LCDLightLvSupply;
    if (str == "LCDLightLvBattery")
        return LCDLightLvBattery;
    if (str == "AutoChangeEEW")
        return AutoChangeEEW;
    if (str == "AutoModeRotate")
        return AutoModeRotate;
    if (str == "AutoChangeMode")
        return AutoChangeMode;
    return -1;
}
void EEW::CallSoundForecast()
{
    if (config.ForecastSoundPath != "")
    {
        if (config.ForecastSoundPerSerial)
        {
            WavePlayer::Stop();
            WavePlayer::Play(config.ForecastSoundPath);
        }
        else
        {
            if (!IsFirstEEWForecast)
            {
                IsFirstEEWForecast = true;
                WavePlayer::Stop();
                WavePlayer::Play(config.ForecastSoundPath);
            }
        }
    }
}
void EEW::CallSoundWarn()
{
    if (config.WarnSoundPath != "")
    {
        if (config.WarnSoundPerSerial)
        {
            WavePlayer::Stop();
            WavePlayer::Play(config.WarnSoundPath);
        }
        else
        {
            if (!IsFirstEEWWarn)
            {
                IsFirstEEWWarn = true;
                WavePlayer::Stop();
                WavePlayer::Play(config.WarnSoundPath);
            }
        }
    }
}
void EEW::CallSoundReset()
{
    IsFirstEEWWarn = false;
    IsFirstEEWForecast = false;
}
DynamicJsonDocument EEW::json(15000);
bool EEW::IsPingUpdate = false;
bool EEW::IsCheck = false;
bool EEW::FinishedThread = false;
bool EEW::RunThread = false;
bool EEW::IsHttpError = false;
int EEW::LatestReadTime = 0;
int EEW::JsonReadTime = 0;
String EEW::LatestHttpError = "";
HTTPClient *EEW::http;
short *EEW::PingValue;
short *EEW::PingValue12sec;
short *EEW::PingValue60sec;
int EEW::PingCount = 0;
bool EEW::IsPingOpen = false;
const String EEW::PrefList[] = {
    "???????????????",
    "?????????",
    "??????","??????","??????","??????","??????","??????",
    "??????","??????","??????","??????","??????","??????","?????????",
    "??????","??????","??????","??????",
    "??????","??????","??????","??????","??????",
    "??????","??????","??????","??????","??????","??????","?????????",
    "??????","??????","??????","??????","??????",
    "??????","??????","??????","??????",
    "??????","??????","??????","??????","??????","??????","?????????",
    "??????"};
const String EEW::TestJson = "{\"ParseStatus\":\"Success\",\"Title\":{\"Code\":37,\"String\":\"??????????????????????????????\",\"Detail\":\"?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????3??? ???????????? ???????????????EPOS?????????????????????\"},\"Source\":{\"Code\":3,\"String\":\"???????????????\"},\"Status\":{\"Code\":\"01\",\"String\":\"??????\",\"Detail\":\"??????\"},\"AnnouncedTime\":{\"String\":\"2018/06/18 07:58:49\",\"UnixTime\":1529276329,\"RFC1123\":\"Sun, 17 Jun 2018 22:58:49 UTC\"},\"OriginTime\":{\"String\":\"2018/06/18 07:58:34\",\"UnixTime\":1529276314,\"RFC1123\":\"Sun, 17 Jun 2018 22:58:34 UTC\"},\"EventID\":\"20180618075838\",\"Type\":{\"Code\":0,\"String\":\"??????\",\"Detail\":\"??????(??????)\"},\"Serial\":11,\"Hypocenter\":{\"Code\":520,\"Name\":\"???????????????\",\"isAssumption\":false,\"Location\":{\"Lat\":34.8,\"Long\":135.6,\"Depth\":{\"Int\":10,\"String\":\"???10km\"}},\"Magnitude\":{\"Float\":6.3,\"String\":\"M6.3\",\"LongString\":\"M6.3\"},\"Accuracy\":{\"Epicenter\":{\"Code\":6,\"String\":\"????????????????????????(5?????????)??????????????? Hi-net ????????????\",\"Rank2\":4,\"String2\":\"IPF???(5???)\"},\"Depth\":{\"Code\":6,\"String\":\"????????????????????????(5?????????)??????????????? Hi-net ????????????\"},\"Magnitude\":{\"Code\":4,\"String\":\"P???/????????????\"},\"NumberOfMagnitudeCalculation\":5},\"isSea\":false},\"MaxIntensity\":{\"From\":\"6+\",\"To\":\"6+\",\"String\":\"6???\",\"LongString\":\"6?????????\"},\"Warn\":true,\"WarnForecast\":{\"Hypocenter\":{\"Code\":9270,\"Name\":\"?????????\"},\"District\":[\"??????\",\"??????\",\"??????\",\"??????\"],\"LocalAreas\":[\"??????\",\"??????\",\"??????\",\"??????\",\"??????\",\"??????\",\"?????????\",\"??????\",\"??????\"],\"Regions\":[\"???????????????\",\"???????????????\",\"???????????????\",\"??????????????????\",\"?????????\",\"???????????????\",\"???????????????\",\"??????????????????\",\"???????????????\",\"???????????????\",\"???????????????\",\"??????????????????\",\"???????????????\",\"??????????????????\",\"???????????????\"]},\"Option\":{\"Change\":{\"Code\":0,\"String\":\"????????????????????????\",\"Reason\":{\"Code\":0,\"String\":\"??????\"}}},\"Forecast\":[{\"Intensity\":{\"Code\":520,\"Name\":\"???????????????\",\"From\":\"6???\",\"To\":\"6???\",\"Description\":\"??????6???????????????6?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":511,\"Name\":\"???????????????\",\"From\":\"6???\",\"To\":\"5???\",\"Description\":\"??????5???????????????6?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":501,\"Name\":\"???????????????\",\"From\":\"6???\",\"To\":\"6???\",\"Description\":\"??????6?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":531,\"Name\":\"??????????????????\",\"From\":\"6???\",\"To\":\"6???\",\"Description\":\"??????6?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":540,\"Name\":\"?????????\",\"From\":\"5???\",\"To\":\"5???\",\"Description\":\"??????5???????????????5?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":521,\"Name\":\"???????????????\",\"From\":\"5???\",\"To\":\"5???\",\"Description\":\"??????5?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":true,\"Condition\":\"??????????????????????????????\"}},{\"Intensity\":{\"Code\":461,\"Name\":\"???????????????\",\"From\":\"5???\",\"To\":\"4\",\"Description\":\"??????4????????????5?????????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:51\"}},{\"Intensity\":{\"Code\":550,\"Name\":\"??????????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:50\"}},{\"Intensity\":{\"Code\":500,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:54\"}},{\"Intensity\":{\"Code\":510,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:54\"}},{\"Intensity\":{\"Code\":401,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:56\"}},{\"Intensity\":{\"Code\":535,\"Name\":\"??????????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:56\"}},{\"Intensity\":{\"Code\":460,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:57\"}},{\"Intensity\":{\"Code\":532,\"Name\":\"??????????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:57\"}},{\"Intensity\":{\"Code\":462,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:02\"}},{\"Intensity\":{\"Code\":432,\"Name\":\"????????????????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:04\"}},{\"Intensity\":{\"Code\":551,\"Name\":\"??????????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:06\"}},{\"Intensity\":{\"Code\":451,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:08\"}},{\"Intensity\":{\"Code\":610,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":true,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:09\"}},{\"Intensity\":{\"Code\":581,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"4\",\"Description\":\"??????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:17\"}},{\"Intensity\":{\"Code\":530,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"3\",\"Description\":\"??????3????????????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:58:59\"}},{\"Intensity\":{\"Code\":600,\"Name\":\"???????????????\",\"From\":\"4\",\"To\":\"3\",\"Description\":\"??????3????????????4??????\"},\"Warn\":false,\"Arrival\":{\"Flag\":false,\"Condition\":\"??????????????????\",\"Time\":\"07:59:06\"}}],\"OriginalText\":\"37 03 00 180618075849 C11 180618075834 ND20180618075838 NCN011 JD////////////// JN/// 520 N348 E1356 010 63 6+ RK66454 RT01/// RC0//// EBI 520 S6+6- ////// 11 511 S6-5+ ////// 11 501 S6-6- ////// 11 531 S6-6- ////// 11 540 S5+5- ////// 11 521 S5+5+ ////// 11 461 S5-04 075851 10 550 S0404 075850 10 500 S0404 075854 10 510 S0404 075854 10 401 S0404 075856 10 535 S0404 075856 10 460 S0404 075857 10 532 S0404 075857 10 462 S0404 075902 00 432 S0404 075904 00 551 S0404 075906 00 451 S0404 075908 00 610 S0404 075909 10 581 S0404 075917 00 530 S0403 075859 00 600 S0403 075906 00 9999=\"}";
int EEW::TestTime = 0;
void EEW::PingSave()
{
    SPIFFS.begin(true);

    fs::FS fs = SPIFFS;
    if (!fs.exists("/config"))
        fs.mkdir("/config");
    File configFile = fs.open("/config/MEQ_PngM.ini", FILE_WRITE);
    if (!configFile)
        return;
    for (int i = 0; i < PingData; i++)
    {
        configFile.println(PingValue60sec[i]);
    }

    configFile.close();
    SPIFFS.end();
}
void EEW::PingLoad()
{
    SPIFFS.begin(true);
    fs::FS fs = SPIFFS;
    File configFile = fs.open("/config/MEQ_PngM.ini", FILE_READ);
    if (!configFile)
        return;
    for (int i = 0; i < PingData; i++)
    {
        String tmp = configFile.readStringUntil('\n');
        tmp.remove(tmp.length() - 1, 1);
        PingValue60sec[i] = tmp.toInt();
    }
    configFile.close();
    SPIFFS.end();
}
int EEW::Split(String data, char delimiter, String *dst)
{
    int index = 0;
    int arraySize = (sizeof(data) / sizeof((data)[0]));
    int datalength = data.length();
    for (int i = 0; i < datalength; i++)
    {
        char tmp = data.charAt(i);
        if (tmp == delimiter)
        {
            index++;
        }
        else if (index < 2)
            dst[index] += tmp;
    }
    return (index + 1);
}