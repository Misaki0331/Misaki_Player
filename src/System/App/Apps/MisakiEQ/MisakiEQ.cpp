#include "MisakiEQ.hpp"
#include "../../../Fonts/FastFont.h"
using namespace App::MisakiEQ;
using namespace Core::Draw;
/*
M5.begin();


    }*/
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
    IsActive = false;
    toHome = 0;
    mode = EEWMode;
    sellectMode = EEWMode;
    IsFirstDrawed = 0;
    IsUpdated = 0;
    settingSellect = 0;
    IsButtonUIUpdate = 0;
    FinishedThread = 0;
    RunThread = 1;
    WarnRegionDisplay = 1;
    IsPingUpdate = false;
    IsNotCursorMode = false;
    pg = 1;
    http = new HTTPClient();
    PingValue = new short[PingData];
    for (int i = 0; i < PingData; i++)
        PingValue[i] = 0;
    IsPingOpen = true;
    xTaskCreatePinnedToCore(GetNetworkFile, "MisakiEQ_EEW", 16800, NULL, 5, NULL, 1);
    ReadConfig();
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
}
void EEW::Loop()
{

    if (mode != EEWMode)
    {
        if (!IsCheck)
        {
            IsCheck = true;
            int a = json["Status"]["Code"];
            int b = json["Serial"];
            JsonState = a * 10000 + b;

            if (JsonState != JsonOldState)
            {
                IsNotCursorMode = false;
                switch (mode)
                {
                case SettingNum:
                    cNum.Cancel();
                    cNum.Release();
                    break;
                }
                JsonOldState = JsonState;
                sellectMode = EEWMode;
                ModeEnter();
                IsUpdated = 0;
            }
        }
    }
    switch (mode)
    {
    case EEWMode:
        if (!IsCheck)
        {
            IsCheck = false;
            int a = json["Status"]["Code"];
            int b = json["Serial"];
            JsonState = a * 10000 + b;

            if (JsonState != JsonOldState)
            {
                JsonOldState = JsonState;
                IsUpdated = 0;
            }
        }

        if (millis() >= regionUpdate + 5000 && IsRegionUpdate)
        {
            WarnRegionDisplay = 0;
            regionUpdate = millis();
        }
        break;
    case SettingNum:
        if (!cNum.GetIsSetting())
        {
            sellectMode = SettingMode;
            cNum.Release();
            ModeEnter();
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
    switch (mode)
    {
    case EEWMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = 1;

            FastFont::printUtf8("第", 100, 27, WHITE, 1, BLACK);
            FastFont::printUtf8("報", 113 + 6 * 3 * 2, 27, WHITE, 1, BLACK);
            FastFont::printUtf8("発生時刻", 0, 50, WHITE, 1, BLACK);
            FastFont::printUtf8("震源地", 13, 78, WHITE, 1, BLACK);
            FastFont::printUtf8("規模", 26, 106, WHITE, 1, BLACK);
            FastFont::printUtf8("深さ", 160, 106, WHITE, 1, BLACK);
            FastFont::printRom("Ping", 320 - 4 * 6, 15, WHITE, 1, BLACK);
            FastFont::printUtf8("最大震度", 320 - 13 * 4, 126, WHITE, 1, BLACK);
        }

        if (!IsUpdated)
        {
            IsUpdated = 1;
            IsRegionUpdate = false;
            if (json["ParseStatus"] == "Success")
            {
                if (json["Status"]["Code"] == "00")
                {
                    if (json["Warn"])
                    {
                        M5.lcd.fillRect(0, 15, 94, 24, RED);
                        IsRegionUpdate = true;
                        FastFont::printUtf8(" 警 報 ", 0, 15, WHITE, 2, RED);
                    }
                    else
                    {

                        M5.lcd.fillRect(0, 15, 94, 24, BLUE);
                        FastFont::printUtf8(" 予 報 ", 0, 15, WHITE, 2, BLUE);
                    }
                }
                else if (json["Status"]["Code"] == "01")
                {
                    M5.lcd.fillRect(0, 15, 94, 24, GREEN);
                    FastFont::printUtf8(" 取 消 ", 0, 15, WHITE, 2, GREEN);
                }
            }
            else
            {
                M5.lcd.fillRect(0, 15, 94, 24, BLACK);
                FastFont::printUtf8(" 不 明 ", 0, 15, WHITE, 2, BLACK);
            }
            if (1)
            { // json["ParseStatus"]=="Success"){
                char *text = new char[80];
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
                    FastFont::printUtf8("(最終報)", 113 + 6 * 3 * 2 + 1 + 12, 27, WHITE, 1, BLACK);
                if (json["Status"]["Code"] == "10")
                {
                    FastFont::printUtf8("訓練中", 113 + 6 * 3 * 2 + 1 + 12 + 55, 27, WHITE, 1, BLACK);
                }
                else if (json["Status"]["Code"] == "11")
                {
                    FastFont::printUtf8("訓練取消", 113 + 6 * 3 * 2 + 1 + 12 + 55, 27, RED, 1, BLACK);
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
                    FastFont::printUtf8("ごく浅い", 194, 94, WHITE, 2, BLACK);
                    break;
                case -1:
                    FastFont::printUtf8("不明", 194, 94, WHITE, 2, BLACK);
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
                regPos = 0;
                pg = 1;
                m5.Lcd.fillRect(268, 140, 51, 51, col);
                FastFont::printUtf8(str, 268 + (51 - x0 * 3) / 2 + 2, 140 + (42 - 24) / 2, textc, 3, col);
                int x1 = 2;
                int y1 = 142;
                M5.lcd.fillRect(0, 127, 267, 82, BLACK);
                if (IsRegionUpdate)
                {

                    M5.lcd.fillRect(0, 127, 267, 13, RED);
                    sprintf(text, "以下の地域は強い揺れに警戒      都道府県");
                    FastFont::printUtf8(text, 1, 128, WHITE, 1, RED);

                    M5.lcd.drawRect(0, 140, 267, 69, json["Warn"] ? RED : BLACK);
                    for (int i = 0;; i++)
                    {
                        String name = json["WarnForecast"]["LocalAreas"][i];
                        if (name == "null")
                            break;
                        int x = name.length() / 3 * 13 + 6;
                        if (x1 + x > 265)
                        {
                            x1 = 2;
                            y1 += 13;
                            if (y1 >= 207)
                                break;
                        }
                        FastFont::printUtf8(name, x1, y1, WHITE, 1, BLACK);
                        x1 += x;
                    }
                }
                delete[] text;
            }
        }
        if (!WarnRegionDisplay)
        {
            WarnRegionDisplay = true;
            char *text = new char[100];
            int x1 = 2;
            int y1 = 142;
            M5.lcd.fillRect(0, 127, 267, 82, BLACK);
            M5.lcd.drawRect(0, 140, 267, 69, json["Warn"] ? RED : BLACK);
            if (regPos == -1)
            {
                M5.lcd.fillRect(0, 127, 267, 13, RED);
                sprintf(text, "以下の地域は強い揺れに警戒      都道府県");
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
                sprintf(text, "以下の地域は強い揺れに警戒        詳細%2d", pg);
                FastFont::printUtf8(text, 1, 128, WHITE, 1, RED);
                for (int i = regPos;; i++)
                {
                    String name = json["WarnForecast"]["Regions"][i];
                    if (name == "null")
                    {
                        regPos = -1;
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
            delete[] text;
        }
        if (!IsPingUpdate)
        {
            IsPingUpdate = true;
            char *text = new char[12];
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
        break;
    case PingMode:
        if (!IsFirstDrawed)
        {
            IsFirstDrawed = 1;
            FastFont::printRom("10s", 0, 15, RED);
            FastFont::printRom(" 1s", 0, 65 - 4, RED);
            FastFont::printRom("500", 0, 115 - 4, YELLOW);
            FastFont::printRom("100", 0, 165 - 4, GREEN);

            FastFont::printRom("  0", 0, 215 - 8, GREEN);

            M5.Lcd.drawRect(19, 14, 302, 202, WHITE);
        }
        if (!IsPingUpdate)
        {
            IsPingUpdate = 1;

            for (int i = 0; i < 300; i++)
            {
                int val = PingValue[i];
                if (val > 9999)
                    val = 9999;
                if (PingValue[i] >= 1000)
                { // 10000まで 9000/180 50
                    val -= 1000;
                    val /= 180;
                    val += 150;

                    M5.Lcd.drawFastVLine(20 + i, 15 + (200 - val), val, RED);
                }
                else if (PingValue[i] >= 500)
                { // 1000まで 500/10 50
                    val -= 500;
                    val /= 10;
                    val += 100;

                    M5.Lcd.drawFastVLine(20 + i, 15 + (200 - val), val, RED);
                }
                else if (PingValue[i] >= 100)
                { // 500まで  400/8 50
                    val -= 100;
                    val /= 8;
                    val += 50;

                    M5.Lcd.drawFastVLine(20 + i, 15 + (200 - val), val, YELLOW);
                }
                else
                { // 100まで /2 +50
                    val /= 2;
                    M5.Lcd.drawFastVLine(20 + i, 15 + (200 - val), val, GREEN);
                }
                if (i % 60 == 0)
                {
                    M5.Lcd.drawFastVLine(20 + i, 15, 200 - val, GetColor(0x808080));
                }
                else if (i % 20 == 0)
                {
                    M5.Lcd.drawFastVLine(20 + i, 15, 200 - val, GetColor(0x404040));
                }
                else
                {
                    M5.Lcd.drawFastVLine(20 + i, 15, 200 - val, BLACK);
                }
                for (float pi = 0; pi < 50; pi += 10)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 15 + (199 - pi), GetColor(0x404040));
                for (float pi = 50; pi < 100; pi += 12.5)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 15 + (199 - pi), GetColor(0x404040));
                for (float pi = 100; pi < 150; pi += 10)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 15 + (199 - pi), GetColor(0x404040));
                for (float pi = 150; pi < 200; pi += 5.55555555555555555555556)
                    if (pi > val)
                        M5.Lcd.drawPixel(20 + i, 15 + (199 - pi), GetColor(0x404040));
                if (50 > val)
                    M5.Lcd.drawPixel(20 + i, 15 + (199 - 50), GetColor(0x208020));
                if (100 > val)
                    M5.Lcd.drawPixel(20 + i, 15 + (199 - 100), GetColor(0x808020));
                if (150 > val)
                    M5.Lcd.drawPixel(20 + i, 15 + (199 - 150), GetColor(0x802020));
            }
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
            for(int i=0;i<=ExitSetting;i++){
                switch(i){
                    case ForecastSoundPath:
                        sprintf(t, "予報鳴動時のサウンド : %s", config.ForecastSoundPath.c_str());
                    break;
                    case ForecastSoundPerSerial:
                        if(config.ForecastSoundPerSerial){
                            sprintf(t, "↑発表時毎回再生 : 有効");
                        }else{
                            sprintf(t, "↑発表時毎回再生 : 無効");
                        }
                    break;
                    case WarnSoundPath:
                        sprintf(t, "警報鳴動時のサウンド : %s", config.WarnSoundPath.c_str());
                        break;
                    break;
                    case WarnSoundPerSerial:
                        if(config.WarnSoundPerSerial){
                            sprintf(t, "↑発表時毎回再生 : 有効");
                        }else{
                            sprintf(t, "↑発表時毎回再生 : 無効");
                        }
                    break;
                    case OnlyListEvent:
                        sprintf(t, "鳴動地域の設定(警報のみ) : %s", config.OnlyListEvent.c_str());
                    break;
                    case LCDoffTimer:
                        if(config.LCDoffTimer==0){
                            sprintf(t, "LCD自動OFF : 無効");
                        }else{
                            sprintf(t, "LCD自動OFF : %5d秒後", config.LCDoffTimer);
                        }
                    break;
                    case RebootTimer:
                        if(config.RebootTimer==0){
                            sprintf(t, "端末自動再起動 : 無効");
                        }else{
                            sprintf(t, "端末自動再起動 : %5d秒後", config.RebootTimer);
                        }
                    break;
                    case ExitSetting:
                        sprintf(t, "設定を保存して終了");
                    break;
                }
                M5.lcd.fillRect(0, 20+12*i, 320, 12, settingSellect == i && IsNotCursorMode ? WHITE : BLACK);
                FastFont::printUtf8(t, 0, 20+12*i, settingSellect == i && IsNotCursorMode ? BLACK : WHITE, 1, INVISIBLE_COLOR);
            
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
                    FcName = "終了";
                    break;
                case EEWMode:
                    x += (100 - 46) / 2;
                    fillx = 46;
                    FcName = "EEW情報";
                    break;
                case PingMode:
                    x += (100 - 77) / 2;
                    fillx = 77;
                    FcName = "応答速度情報";
                    break;
                case SettingMode:
                    x += (100 - 25) / 2;
                    fillx = 25;
                    FcName = "設定";
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

            FastFont::printUtf8("↑", 19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);

            FastFont::printUtf8("決定", 19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);

            FastFont::printUtf8("↓", 19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        }
    }
}
void EEW::ModeEnter()
{
    if (mode == SettingMode && sellectMode == SettingMode)
    {
        IsNotCursorMode = true;
        IsButtonUIUpdate = false;
        settingSellect = 0;
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
    IsPingOpen = false;
    delete[] PingValue;
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
                deserializeJson(json, http[0].getString());
                break;
            default:
                LatestHttpError = httpcode;
                break;
            }
        }
        http[0].end();
        int t = millis();
        JsonReadTime = t - start;
        if (IsPingOpen)
        {
            for (int i = 1; i < PingData; i++)
                PingValue[i - 1] = PingValue[i];
            PingValue[PingData - 1] = JsonReadTime;
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
                //モード決定
                ModeEnter();
                break;
            case 3:
                if (sellectMode < 2)
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
                //設定画面移動
                SettingEnter();
                IsButtonUIUpdate = false;
                break;
            case 3:
                if (settingSellect < ExitSetting)
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
void EEW::SettingEnter()
{
    switch (settingSellect)
    {
    case ForecastSoundPath:
        cPath.Begin(&config.ForecastSoundPath, ".wav");
        sellectMode = SettingPath;
        cPath.SetTitle("予報時のサウンドを選択", "全ての緊急地震速報(予報)に適用されます。");
        ModeEnter();
        break;
    case ForecastSoundPerSerial:
        config.ForecastSoundPerSerial=!config.ForecastSoundPerSerial;
        IsUpdated=false;
        break;
    case WarnSoundPath:
        cPath.Begin(&config.WarnSoundPath, ".wav");
        sellectMode = SettingPath;
        cPath.SetTitle("警報時のサウンドを選択", "警報が発表された際にファイルを再生します。");
        ModeEnter();
        break;
    case WarnSoundPerSerial:
        config.WarnSoundPerSerial=!config.WarnSoundPerSerial;
        IsUpdated=false;
        break;
    case OnlyListEvent:
        cList.Begin(&config.OnlyListEvent, PrefList, 48);
        sellectMode = SettingList;
        cList.SetTitle("地域設定", "選択した場合、地域のみ警報の鳴動が有効になります");
        ModeEnter();
        break;
    case LCDoffTimer:
        cNum.Begin(&config.LCDoffTimer, 5);
        sellectMode = SettingNum;
        cNum.SetTitle("LCD自動消灯する時間(秒)", "鳴動後に液晶ディスプレイをオフにする時間を設定し\nます。(設定値=0で無効)");
        ModeEnter();
        break;
    case RebootTimer:
        cNum.Begin(&config.RebootTimer, 5);
        sellectMode = SettingNum;
        cNum.SetTitle("端末再起動する時間(秒)", "鳴動後に自動で再起動をスケジュールします。\nスピーカーのノイズが酷い場合にどうぞ\n(設定値=0で無効)");
        ModeEnter();
        break;
    case ExitSetting:
        SaveConfig();
        IsNotCursorMode = false;
        break;
        
    }
}
void EEW::SaveConfig(){
    SPIFFS.begin(true);
    
    fs::FS fs = SPIFFS;
    if(!fs.exists("/config"))fs.mkdir("/config");
    File configFile = fs.open("/config/MisakiEQ.cfg",FILE_WRITE);
    if(!configFile)return;
    configFile.println(config.ForecastSoundPath);
    configFile.println(config.ForecastSoundPerSerial?"true":"false");
    configFile.println(config.WarnSoundPath);
    configFile.println(config.WarnSoundPerSerial?"true":"false");
    configFile.println(config.OnlyListEvent);
    configFile.println(config.LCDoffTimer);
    configFile.println(config.RebootTimer);
    configFile.close();
    SPIFFS.end();
}
void EEW::ReadConfig(){
    SPIFFS.begin(true);
    fs::FS fs = SPIFFS;
    File configFile = fs.open("/config/MisakiEQ.cfg",FILE_READ);
    if(!configFile){
        config.OnlyListEvent="全都道府県";
        SPIFFS.end();
        return;
    }
    for(int i=0;i<ExitSetting;i++){
        String tmp=configFile.readStringUntil('\n');
        tmp.remove(tmp.length()-1,1);
        switch(i){
            case ForecastSoundPath:
                config.ForecastSoundPath=tmp;
                break;
            case ForecastSoundPerSerial:
                tmp=="true"?config.ForecastSoundPerSerial=1:config.ForecastSoundPerSerial=0;
                break;
            case WarnSoundPath:
                config.WarnSoundPath=tmp;
                break;
            case WarnSoundPerSerial:
                tmp=="true"?config.WarnSoundPerSerial=1:config.WarnSoundPerSerial=0;
                break;
            case OnlyListEvent:
                config.OnlyListEvent=tmp;
                break;
            case LCDoffTimer:
                config.LCDoffTimer=tmp.toInt();
                break;
            case RebootTimer:
                config.RebootTimer=tmp.toInt();
                break;
        }
    }
    
    configFile.close();
    SPIFFS.end();

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
bool EEW::IsPingOpen = false;
const String EEW::PrefList[] = {
    "全都道府県",
    "北海道",
    "青森",
    "岩手",
    "宮城",
    "秋田",
    "山形",
    "福島",
    "茨城",
    "栃木",
    "群馬",
    "埼玉",
    "千葉",
    "東京",
    "神奈川",
    "新潟",
    "富山",
    "石川",
    "福井",
    "山梨",
    "長野",
    "岐阜",
    "静岡",
    "愛知",
    "三重",
    "滋賀",
    "京都",
    "大阪",
    "兵庫",
    "奈良",
    "和歌山",
    "鳥取",
    "島根",
    "岡山",
    "広島",
    "山口",
    "徳島",
    "香川",
    "愛媛",
    "高知",
    "福岡",
    "佐賀",
    "長崎",
    "熊本",
    "大分",
    "宮崎",
    "鹿児島",
    "沖縄"};