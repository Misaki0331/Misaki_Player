#include "connect.h"
#include <EEPROM.h>
#include "../../../Fonts/FastFont.h"

#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#define FORMAT_SPIFFS_IF_FAILED true
using namespace App::Wi_Fi;
using namespace Core::Draw;
Connect::Connect()
{
}
void Connect::Exit()
{
    textbox.Release();
    numbox.Release();
    listbox.Release();
    if (AccessPoints)
    {
        delete[] AccessPoints;
        AccessPoints = nullptr;
    }
}
String Connect::GetAppName()
{
    return "Wi-Fi Connection Setting";
}
void Connect::Begin()
{

    FastFont::printRom("Loading Profiles...", 0, 30, WHITE, 2);
    FastFont::printRom("Please wait...", 0, 50, WHITE, 2);
    IsFileExists = false;
    ReadProfile();
    isEasySetting = false;
    IsFirstDraw = 1;
    IsDraw = 1;
    mode = Menu;
    sellectMode = 0;
    CurrentProfileID = 0;
    toHome = 0;
    M5.lcd.fillRect(0, 0, 320, 240, BLACK);
}
void Connect::SaveProfile(uint8_t profile)
{
    if (profile > 99)
        return;
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    if (!fs.exists("/config"))
        fs.mkdir("/config");
    char *filename = new char[32];
    sprintf(filename, "/config/Wi-Fi_%02d.ini", profile);
    File config = fs.open(filename, FILE_WRITE);
    delete[] filename;
    if (!config)
    {
        return;
    }
    else
    {
        IsFileExists = true;
    }
    config.println(ssid);
    config.println(password);
    config.close();
    SPIFFS.end();
}
void Connect::ReadProfile(uint8_t profile)
{
    ssid.clear();
    password.clear();
    if (profile > 99)
        return;
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    char *filename = new char[32];

    sprintf(filename, "/config/Wi-Fi_%02d.ini", profile);
    if (fs.exists(filename))
    {
        IsFileExists = true;
    }
    else
    {
        IsFileExists = false;
        SPIFFS.end();
        return;
    }
    File config = fs.open(filename, FILE_READ);
    delete[] filename;

    if (!config)
    {
        IsFileExists = false;
        SPIFFS.end();
        return;
    }
    ssid = config.readStringUntil('\n');
    password = config.readStringUntil('\n');
    ssid.remove(ssid.length() - 1, 1);
    password.remove(password.length() - 1, 1);
    config.close();
    SPIFFS.end();
}
void Connect::DelProfile(uint8_t profile)
{
    ssid.clear();
    password.clear();
    if (profile > 99)
        return;
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    char *filename = new char[32];
    sprintf(filename, "/config/Wi-Fi_%02d.ini", profile);
    if (fs.exists(filename))
        fs.remove(filename);
    delete[] filename;
    SPIFFS.end();
    IsFileExists = false;
}
void Connect::Draw()
{
    if (IsFirstDraw)
    {
        IsFirstDraw = 0;
        switch (mode)
        {
        case Menu:
        {
            FastFont::printRom("Wi-Fi Setting", 0, 10, WHITE);
            M5.Lcd.fillRect(0, 200, 320, 27, BLACK);
            char *txt = new char[128];
            String st;
            if (IsFileExists)
            {
                st = "あり";
            }
            else
            {
                st = "なし";
            }
            sprintf(txt, "現在のプロファイル番号 : %02d (ファイル : %s)", CurrentProfileID, st.c_str());
            st.clear();
            FastFont::printUtf8(txt, 0, 180, WHITE, 1, INVISIBLE_COLOR);
            sprintf(txt, "SSID : %s", ssid.c_str());
            FastFont::printUtf8(txt, 0, 193, WHITE, 1, INVISIBLE_COLOR);
            String pas;
            for (int i = 0; i < password.length(); i++)
                pas += "*";
            sprintf(txt, "PASS : %s", pas.c_str());
            pas.clear();
            FastFont::printUtf8(txt, 0, 206, WHITE, 1, INVISIBLE_COLOR);
            delete[] txt;
        }
        break;
        case WiFi_Detail:
            FastFont::printRom("Wi-Fi Detail", 0, 10, WHITE);
            FastFont::printRom("MacAddress:", 0, 20, WHITE);
            FastFont::printRom(GetMacAddress(), 0, 30, WHITE, 2);
            FastFont::printRom("SSID:", 0, 58, WHITE);
            FastFont::printRom(ssid, 0, 68, WHITE, 2);
            FastFont::printRom("Password:", 0, 96, WHITE);
            FastFont::printRom(password, 0, 106, WHITE, 2);
            break;

        case WiFi_Test:
            FastFont::printRom("Wi-Fi Connection Tester", 0, 20, WHITE, 1, BLACK);
            break;
        }
    }
    if (IsDraw)
    {
        IsDraw = 0;
        switch (mode)
        {
        case Menu:
            for (int i = 0; i < 8; i++)
            {
                M5.Lcd.fillRect(0, 20 + 12 * i, 198, 12, sellectMode == i ? WHITE : BLACK);
                switch (i)
                {
                case WiFi_Detail:
                    FastFont::printUtf8("Wi-Fi詳細情報の表示",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : WHITE, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case WiFi_Test:
                    FastFont::printUtf8("接続テストを実行",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : GREEN, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case WiFi_SetSSID:
                    FastFont::printUtf8("Wi-Fi SSIDの手動設定",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : WHITE, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case WiFi_SetPassword:
                    FastFont::printUtf8("Wi-Fi パスワードの設定",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : WHITE, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case WiFi_Return:
                    FastFont::printUtf8("Launcherに戻る",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : WHITE, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case SettingProfileID:
                    FastFont::printUtf8("プロファイルの変更",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : WHITE, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case DeleteProfile:
                    FastFont::printUtf8("プロファイルの削除",
                                        0, 20 + 12 * i, RED, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                case WiFi_EasySetting:
                    FastFont::printUtf8("近くのWi-Fiから設定",
                                        0, 20 + 12 * i, sellectMode == i ? BLACK : YELLOW, 1, sellectMode == i ? WHITE : BLACK);
                    break;
                }
            }

            break;
        case WiFi_Test:
            if (tempmode != testmode || testmode == 1)
            {
                if (tempmode != testmode)
                {
                    tempmode = testmode;
                    scroll += 8;
                }
                switch (testmode)
                {
                case 0:
                    FastFont::printConsole("Wi-Fi connecting...", 0, 20 + scroll);
                    testmode++;
                    IsDraw = 1;
                    break;
                case 1:
                {
                    char *test = new char[40];
                    int tempt = millis() - connectingTime;
                    sprintf(test, "Connection time:%3d.%02ds", tempt / 1000, tempt % 1000 / 10);
                    if (tempt % 10 == 0)
                        FastFont::printRom(test, 0, 20 + scroll, WHITE, 1, BLACK);
                    delete[] test;
                }
                break;
                case 2:
                {
                    FastFont::printConsole("|*aConnected!", 0, 20 + scroll);
                    scroll += 8;
                    char *tes = new char[40];
                    // int n= WiFi.scanNetworks();
                    int level = 0;
                    /*for(int i=0;i<n;i++){
                        if(WiFi.SSID(i)==ssid){
                            level=WiFi.RSSI(i);
                            break;
                        }
                    }*/
                    level = WiFi.RSSI();
                    Serial.println(WiFi.RSSI());
                    if (WiFi.status() != WL_CONNECTED)
                        Serial.println("Connection Lost");
                    sprintf(tes, "Wi-Fi Signal Strength : |*a%d|*fdB", level);
                    FastFont::printConsole(tes, 0, 20 + scroll);
                    scroll += 8;
                    IPAddress ip = (uint32_t)WiFi.localIP();
                    sprintf(tes, "Local IP : |*a%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
                    FastFont::printConsole(tes, 0, 20 + scroll);
                    scroll += 8;
                    ip = (uint32_t)WiFi.dnsIP();
                    sprintf(tes, "DNS IP : |*a%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
                    FastFont::printConsole(tes, 0, 20 + scroll);
                    scroll += 8;
                    delete[] tes;
                    testmode++;
                }
                case 3:
                {
                    HTTPClient http;
                    // HTTPS
                    FastFont::printConsole("Testing http client...", 0, 20 + scroll);
                    scroll += 8;
                    http.begin("https://api.iedred7584.com/eew/json/");
                    char *ts = new char[64];
                    // start connection and send HTTP header
                    int httpCode = http.GET();
                    sprintf(ts, "HTTP Code : %d", httpCode);
                    FastFont::printConsole(ts, 0, 20 + scroll);
                    scroll += 8;
                    if (httpCode > 0)
                    {
                        String Data = http.getString();
                        sprintf(ts, "Data Size : %d Bytes", http.getSize());
                        FastFont::printConsole(ts, 0, 20 + scroll);
                        scroll += 8;

                        //FastFont::printUtf8(Data, 0, 20 + scroll, GREEN, 1, BLACK, true);

                        // for(int i=0;i<Data.length();i+=32)Serial.print(Data.substring(i,i+31).c_str());
                        Data.clear();
                    }
                    else
                    {
                        FastFont::printConsole("|*cHTTP GET ERROR!", 0, 20 + scroll);
                        scroll += 8;

                        FastFont::printConsole("Massage : |*c" + http.errorToString(httpCode), 0, 20 + scroll);
                        scroll += 8;
                    }
                    http.end();
                    delete[] ts;

                    testmode = 65535;
                }
                break;
                    break;
                case 1000:
                    FastFont::printConsole("|*cWi-Fi connection timeout!", 0, 20 + scroll);
                    testmode = 65535;
                    break;

                default:
                    break;
                }
            }
            break;
        }
    }
    if (mode == SettingTextBox || mode == SettingNum || mode == SettingList)
    {
        switch (mode)
        {

        case SettingNum:
            numbox.Draw();
            break;
        case SettingTextBox:
            textbox.Draw();
            break;
        case SettingList:
            listbox.Draw();
            break;
        }
    }
}
Connect::DateTime Connect::UnixToDateTime(long value)
{
    Connect::DateTime tm;
    tm.year = 1970;
    tm.month = 1;
    tm.day = 1;
    tm.hour = 0;
    tm.min = 0;
    tm.sec = 0;

    unsigned long yearindate;
    unsigned long seconds_year;
    unsigned long seconds_month;
    byte daysinmonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    value += 9 * 3600;
    while (value > 0)
    {
        if ((tm.year % 4 == 0 && (tm.year % 100 != 0 || tm.year % 400 == 0)))
        {
            yearindate = 366;
            daysinmonth[1] = 29;
        }
        else
        {
            yearindate = 365;
            daysinmonth[1] = 28;
        }
        seconds_year = yearindate * 86400;
        seconds_month = daysinmonth[tm.month - 1] * 86400;
        if (value >= seconds_year)
        {
            tm.year++;
            value -= seconds_year;
        }
        else if (value >= seconds_month)
        {
            tm.month++;
            value -= seconds_month;
        }
        else if (value >= 86400)
        {
            tm.day++;
            value -= 86400;
        }
        else if (value >= 3600)
        {
            tm.hour++;
            value -= 3600;
        }
        else if (value >= 60)
        {
            tm.min++;
            value -= 60;
        }
        else
        {
            tm.sec = (byte)value;
            value = 0;
        }
    }
    return tm;
}
void Connect::ButtonPress(int Type)
{
    switch (mode)
    {
    case Menu:
        switch (Type)
        {
        case 1:
            if (sellectMode > 0)
                sellectMode--;
            break;
        case 2:
            ModeEnter();
            //決定時の動作
            break;
        case 3:
            if (sellectMode < WiFi_Return)
                sellectMode++;
            break;
        }
        IsDraw = true;
        break;
    case WiFi_Detail:
        sellectMode = -1;
        ModeEnter();
        break;
    case WiFi_Test:
        // WiFi.disconnect(true);
        sellectMode = -1;
        ModeEnter();
        break;
    case SettingNum:
        numbox.Button(Type);
        break;
    case SettingTextBox:
        textbox.Button(Type);
        break;
    case SettingList:
        listbox.Button(Type);
        break;
    default:
        sellectMode = -1;
        ModeEnter();
    }
}

void Connect::ModeEnter()
{
    IsFirstDraw = true;
    IsDraw = true;
    mode = sellectMode;
    M5.lcd.fillRect(0, 0, 320, 240, BLACK);
    switch (mode)
    {
    case WiFi_Return:
        toHome = 1;
        break;
    case WiFi_SetSSID:
        textbox.Begin(&ssid, 64);
        textbox.SetTitle("Wi-Fi SSID", "アクセスポイント名を入力してください。");
        mode = SettingTextBox;
        break;
    case WiFi_SetPassword:
        textbox.Begin(&password, 64);
        textbox.SetTitle("Wi-Fi Password", "パスワードを入力してください。");
        mode = SettingTextBox;
        break;
    case Menu:
        sellectMode = 0;
        break;
    case WiFi_Test:
        if(ssid==""){
            mode=Menu;
        }else{
        Core::SystemAPI::WiFiCurrentProfile=CurrentProfileID;
        scroll = 0;
        testmode = 0;
        tempmode = -1;
        connectingTime = millis();
        WiFi.begin(ssid.c_str(), password.c_str());
        }
        /*

            scrollX=0;
            testmode=0;
            WiFi.begin(ssid.c_str(), password.c_str());
            startTimer=millis();*/
        break;
    case SettingProfileID:
        numbox.Begin(&CurrentProfileID, 2);
        numbox.SetTitle("プロファイルの選択", "IDを指定してください");
        mode = SettingNum;
        break;
    case DeleteProfile:
        DelProfile(CurrentProfileID);
        IsFirstDraw = true;
        mode = Menu;
        break;
    case WiFi_EasySetting:
        // |No.001 100% [AUTH] ACCESS Point
        if(WiFi.status()!=WL_CONNECTED)WiFi.disconnect();
        isEasySetting = true;
        M5.Lcd.clear();
        FastFont::printUtf8("Wi-Fiスポットを検索中\nしばらくお待ちください…",0,15,YELLOW,2,INVISIBLE_COLOR);
        int n = WiFi.scanNetworks();
        M5.lcd.clear();
        AccessPoints = new String[n];
        char*text=new char[256];
        for(int i=0;i<n&&i<99;i++){
            int percent=WiFi.RSSI(i);
            percent+=100;
            float per=percent/80.0*100.0;
            percent=(int)per;
            if(percent<0)per=0;
            if(percent>100)percent=100;
            if(percent==100){
                sprintf(text,"No.%02d MAX %c %s",i+1,(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? ' ' : '*',WiFi.SSID(i).c_str());
           
            }else{
                sprintf(text,"No.%02d %2d%% %c %s",i+1,percent,(WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? ' ' : '*',WiFi.SSID(i).c_str());
           
            }
            AccessPoints[i]=text;
        }
        listbox.Begin(&ssid, AccessPoints, n>99?99:n);
        sprintf(text,"%d 個のアクセスポイントが見つかりました。",n);
        listbox.SetTitle("Wi-Fiの選択",text);
        listbox.SetColor(YELLOW,GREEN);
        delete[] text;
        mode = SettingList;
        break;
    }
}
String Connect::GetMacAddress()
{
    uint8_t *baseMac = new uint8_t[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char *baseMacChr = new char[18];
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X",
            baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    String Result = baseMacChr;
    delete[] baseMac;
    delete[] baseMacChr;
    return Result;
}
bool Connect::GetUpDateDraw()
{
    switch (mode)
    {
    case SettingNum:
        return numbox.GetIsUpdate();
        break;
    case SettingTextBox:
        return textbox.GetIsUpdate();
        break;
    case SettingList:
        return listbox.GetIsUpdate();
        break;
    case WiFi_Test:
        return tempmode != testmode || testmode == 1;
        break;
    default:
        return IsDraw;
    }
}
bool Connect::GetGoToHome()
{
    return toHome;
}
void Connect::Loop()
{
    switch (mode)
    {
    case WiFi_Test:
        if (testmode == 1)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                testmode++;
            }
            if (millis() - connectingTime > 20000)
            {
                testmode = 1000;
            }
            IsDraw = true;
        }
        if (testmode == 65535)
        {
            // WiFi.disconnect(true);
            testmode = 65536;
        }
        break;
    case SettingNum:
        if (!numbox.GetIsSetting())
        {
            numbox.Release();
            sellectMode = Menu;
            ModeEnter();
            ReadProfile(CurrentProfileID);
        }
        break;
    case SettingTextBox:
        if (!textbox.GetIsSetting())
        {
            textbox.Release();
            if (isEasySetting)
            {
                if (textbox.IsCanceled()){
                    ReadProfile(CurrentProfileID);
                }else{
                    SaveProfile(CurrentProfileID);
                }
                sellectMode = Menu;
                ModeEnter();
            }
            else
            {
                sellectMode = Menu;
                ModeEnter();
                if (!textbox.IsCanceled())
                    SaveProfile(CurrentProfileID);
            }
        }
        break;
    case SettingList:
        if (!listbox.GetIsSetting())
        {
            listbox.Release();
            if (!listbox.IsCancel())
            {
                ssid = ssid.substring(12, ssid.length()); // |No.001 100% ACCESS Point
                sellectMode = SettingTextBox;
                ModeEnter();
                textbox.Begin(&password, 64);
                textbox.SetTitle("Wi-Fi Password", "パスワードを入力してください。");
            }
            else
            {
                sellectMode = Menu;
                ModeEnter();
            }
            if (AccessPoints)
            {
                delete[] AccessPoints;
                AccessPoints = nullptr;
            }
        }
        break;
    }
}
