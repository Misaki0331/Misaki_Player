#include"connect.h"
#include<EEPROM.h>
#include"../../../Fonts/FastFont.h"

#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <HTTPClient.h>
#define FORMAT_SPIFFS_IF_FAILED true
using namespace App::Wi_Fi;
using namespace Core::Draw;
Connect::Connect(){

}
void Connect::Exit(){
    TempSetString="";
}
String Connect::GetAppName(){
    return "Wi-Fi Connection Setting";
}
void Connect::Begin(){
    
    FastFont::printRom("Loading Profiles...",0,30,WHITE,2);
    FastFont::printRom("Please wait...",0,50,WHITE,2);
    ReadProfile();
    IsFirstDraw=1;
    IsDraw=1;
    sellecting_char=0;
    IsLargeCharMode=0;
    mode=Menu;
    TempSetString="";
    sellectMode=0;
    toHome=0;
    M5.lcd.fillRect(0,0,320,240,BLACK);
}
void Connect::SaveProfile(){
    
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    
    fs::FS fs = SPIFFS;
    if(!fs.exists("/config"))fs.mkdir("/config");
    File config = fs.open("/config/Wi-Fi_00.ini",FILE_WRITE);
    if(!config)return;
    config.println(ssid);
    config.println(password);
    config.close();
    SPIFFS.end();
}
void Connect::ReadProfile(){
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    File config = fs.open("/config/Wi-Fi_00.ini",FILE_READ);
    if(!config){
        SPIFFS.end();
        return;
    }
    ssid=config.readStringUntil('\n');
    password=config.readStringUntil('\n');
    ssid.remove(ssid.length()-1,1);
    password.remove(password.length()-1,1);
    config.close();
    SPIFFS.end();
}
void Connect::Draw(){
    if(IsFirstDraw){
        IsFirstDraw=0;
        switch(mode){
            case Menu:
                FastFont::printRom("Wi-Fi Setting",0,10,WHITE);
                break;
            case WiFi_Detail:
                FastFont::printRom("Wi-Fi Detail",0,10,WHITE);
                FastFont::printRom("MacAddress:",0,20,WHITE);
                FastFont::printRom(GetMacAddress(),0,30,WHITE,2);
                FastFont::printRom("SSID:",0,58,WHITE);
                FastFont::printRom(ssid,0,68,WHITE,2);
                FastFont::printRom("Password:",0,96,WHITE);
                FastFont::printRom(password,0,106,WHITE,2);
            break;
            case WiFi_SetSSID:
                FastFont::printRom("Set SSID",0,30,WHITE,1,BLACK);
                break;
            case WiFi_SetPassword:
                FastFont::printRom("Set Password",0,30,WHITE,1,BLACK);
                break;
                case WiFi_Test:
                FastFont::printRom("Wi-Fi Connection Tester",0,20,WHITE,1,BLACK);
                break;
        }
    }
    if(IsDraw){
        IsDraw=0;
        switch(mode){
            case Menu:
                for(int i=0;i<5;i++){
                    M5.Lcd.fillRect(0,20+8*i,90,8,sellectMode==i?WHITE:BLACK);
                    switch(i){
                        case WiFi_Detail:  
                            FastFont::printRom("Wi-Fi Detail",
                            0,20+8*i,sellectMode==i?BLACK:WHITE,1,sellectMode==i?WHITE:BLACK);
                        break;
                        case WiFi_Test:
                            FastFont::printRom("Connection Test",
                            0,20+8*i,sellectMode==i?BLACK:WHITE,1,sellectMode==i?WHITE:BLACK);
                        break;
                        case WiFi_SetSSID:
                            FastFont::printRom("Set SSID",
                            0,20+8*i,sellectMode==i?BLACK:WHITE,1,sellectMode==i?WHITE:BLACK);
                        break;
                        case WiFi_SetPassword:
                            FastFont::printRom("Set Password",
                            0,20+8*i,sellectMode==i?BLACK:WHITE,1,sellectMode==i?WHITE:BLACK);
                        break;
                        case WiFi_Return:
                            FastFont::printRom("Return to Home",
                            0,20+8*i,sellectMode==i?BLACK:WHITE,1,sellectMode==i?WHITE:BLACK);
                        break;
                    }
                    
                }
            
            break;
            case WiFi_SetSSID:
            case WiFi_SetPassword:
            {
                char* text=new char[48];
                short col=WHITE;
                int max=1;
                if(mode==WiFi_SetSSID){
                    max=WIFI_SSID_CHAR;
                }else if(mode==WiFi_SetPassword){
                    max=WIFI_PASSWORD_CHAR;
                }
                if(100*TempSetString.length()/max>=75)col=YELLOW;
                if(TempSetString.length()==max)col=RED;
                sprintf(text,"Length %2d/%2d",TempSetString.length(),WIFI_SSID_CHAR);
                FastFont::printRom(text,320-6*strlen(text),30,col,1,BLACK);
                M5.Lcd.fillRect(0,40,320,16,BLACK);
                FastFont::printRom(TempSetString,0,40,YELLOW,2,BLACK);
                DrawKeyBoardUI();
                delete[] text;
            }
            break;
            case WiFi_Test:
                if(tempmode!=testmode||testmode==1){
                    if(tempmode!=testmode){tempmode=testmode;scroll+=8;}
                    switch(testmode){
                        case 0:
                        FastFont::printConsole("Wi-Fi connecting...",0,20+scroll);
                        testmode++;
                        IsDraw=1;
                        break;
                        case 1:{
                        char* test = new char[40];
                        int tempt=millis()-connectingTime;
                        sprintf(test,"Connection time:%3d.%02ds",tempt/1000,tempt%1000/10);
                        if(tempt%10==0)FastFont::printRom(test,0,20+scroll,WHITE,1,BLACK);
                        delete[] test;}
                        break;
                        case 2:{
                        FastFont::printConsole("|*aConnected!",0,20+scroll);
                        scroll+=8;
                        char* tes = new char[40];
                        //int n= WiFi.scanNetworks();
                        int level=0;
                        /*for(int i=0;i<n;i++){
                            if(WiFi.SSID(i)==ssid){
                                level=WiFi.RSSI(i);
                                break;
                            }
                        }*/
                        level=WiFi.RSSI();
                        Serial.println(WiFi.RSSI());
                        if(WiFi.status()!=WL_CONNECTED)Serial.println("Connection Lost");
                        sprintf(tes,"Wi-Fi Signal Strength : |*a%d|*fdB",level);
                        FastFont::printConsole(tes,0,20+scroll);
                        scroll+=8;
                        IPAddress ip = (uint32_t) WiFi.localIP();
                        sprintf(tes,"Local IP : |*a%u.%u.%u.%u", ip[0],ip[1],ip[2],ip[3]);
                        FastFont::printConsole(tes,0,20+scroll);
                        scroll+=8;
                        ip = (uint32_t) WiFi.dnsIP();
                        sprintf(tes,"DNS IP : |*a%u.%u.%u.%u", ip[0],ip[1],ip[2],ip[3]);
                        FastFont::printConsole(tes,0,20+scroll);
                        scroll+=8;
                        delete[]tes;
                        testmode++;}
                        case 3:{
                            HTTPClient http;
                            //HTTPS
                            FastFont::printConsole("Testing http client...",0,20+scroll);
                            scroll+=8;
                            http.begin("https://api.iedred7584.com/eew/json/");
                            char* ts=new char[64];
                            // start connection and send HTTP header
                            int httpCode = http.GET();
                            sprintf(ts,"HTTP Code : %d",httpCode);
                            FastFont::printConsole(ts,0,20+scroll);
                            scroll+=8;
                            if(httpCode>0){
                               String Data = http.getString();
                               sprintf(ts,"Data Size : %d Bytes",http.getSize());
                               FastFont::printConsole(ts,0,20+scroll);
                                scroll+=8;
                               
                               FastFont::printUtf8(Data,0,20+scroll,GREEN,1,BLACK,true);
                               
                               //for(int i=0;i<Data.length();i+=32)Serial.print(Data.substring(i,i+31).c_str());
                                Data.clear();
                            }else{
                                FastFont::printConsole("|*cHTTP GET ERROR!",0,20+scroll);
                                scroll+=8;
                            
                                FastFont::printConsole("Massage : |*c"+http.errorToString(httpCode),0,20+scroll);
                                scroll+=8;
                            }
                            http.end();
                            delete[] ts;
                            
                            testmode=65535;
                            }
                            break;
                        break;
                        case 1000:
                        FastFont::printConsole("|*cWi-Fi connection timeout!",0,20+scroll);
                        testmode=65535;
                        break;
                        
                        default:
                        break;
                    }
                }
            break;
          
                

        }
    }
}
Connect::DateTime Connect::UnixToDateTime(long value){
    Connect::DateTime tm;
    tm.year  = 1970;
    tm.month = 1;
    tm.day   = 1;
    tm.hour  = 0;
    tm.min   = 0;
    tm.sec   = 0;
    
    unsigned long yearindate;
    unsigned long seconds_year;
    unsigned long seconds_month;
    byte daysinmonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
    value += 9 * 3600;
    while( value > 0 ) {
      if( (tm.year%4==0&&(tm.year%100!=0||tm.year%400==0)) ) {
        yearindate = 366;
        daysinmonth[1] = 29;
      } else {
        yearindate = 365;
        daysinmonth[1] = 28;
      }
      seconds_year  = yearindate * 86400;
      seconds_month = daysinmonth[tm.month - 1] * 86400;
      if( value >= seconds_year ) {
        tm.year++;
        value -= seconds_year;
      } else if( value >= seconds_month ) {
        tm.month++;
        value -= seconds_month;
      } else if( value >= 86400 ) {
        tm.day++;
        value -= 86400;  
      } else if( value >= 3600 ) {
        tm.hour++;
        value -= 3600; 
      } else if( value >= 60 ) {
        tm.min++;
        value -= 60;
      } else {
        tm.sec = (byte)value;
        value= 0;
      }
    }
    return tm;
}
void Connect::DrawKeyBoardUI(){
    M5.Lcd.fillRect(0,160,320,60,BLACK);
    if(GetSoftKeyboardChar((sellecting_char+73)%73)<16){
        switch(GetSoftKeyboardChar((sellecting_char+73)%73)){
            case 0:
                FastFont::printRom("EXIT",(320-4*2*6)/2,220-2*8,WHITE,2,BLACK);
            break;
            case 1:
                FastFont::printRom("SAVE",(320-4*2*6)/2,220-2*8,WHITE,2,BLACK);
            break;
            case 2:
                FastFont::printRom("<-BS",(320-4*2*6)/2,220-2*8,WHITE,2,BLACK);
            break;
            case 3:
                FastFont::printRom("A/a",(320-3*2*6)/2,220-2*8,WHITE,2,BLACK);
            break;
        }
    }else{
        char* txt=new char[5];
        sprintf(txt,"%c",GetSoftKeyboardChar((sellecting_char+73)%73));
        FastFont::printRom(txt,145,172,WHITE,6,BLACK);
        delete[] txt;
    }
    for(int i=0;i<2;i++)
    {
        if(GetSoftKeyboardChar((sellecting_char+73-1+i*2)%73)<16){
            switch(GetSoftKeyboardChar((sellecting_char+73-1+i*2)%73)){
                case 0:
                    FastFont::printRom("EXIT",(160-4*1*6)/2+160*i,220-1*8,WHITE,1,BLACK);
                break;
                case 1:
                    FastFont::printRom("SAVE",(160-4*1*6)/2+160*i,220-1*8,WHITE,1,BLACK);
                break;
                case 2:
                    FastFont::printRom("<-BS",(160-4*1*6)/2+160*i,220-1*8,WHITE,1,BLACK);
                break;
                case 3:
                    FastFont::printRom("A/a",(160-3*1*6)/2+160*i,220-1*8,WHITE,1,BLACK);
                break;
            }
        }else{
            char* txt=new char[5];
            sprintf(txt,"%c",GetSoftKeyboardChar((sellecting_char+73-1+i*2)%73));
            FastFont::printRom(txt,(160-3*5)/2+160*i,220-3*8,WHITE,3,BLACK);
            delete[] txt;
        }
    }
    
}
void Connect::ButtonPress(int Type){
    switch(mode){
        case Menu:
            switch(Type){
                case 1:
                    if(sellectMode>0)sellectMode--;
                    break;
                case 2:
                    ModeEnter();
                    //決定時の動作
                    break;
                case 3:
                    if(sellectMode<4)sellectMode++;
                    break;
            }
            IsDraw=true;
            break;
        case WiFi_Detail:
            sellectMode=-1;
            ModeEnter();
            break;
        case WiFi_Test:
            //WiFi.disconnect(true);
            sellectMode=-1;
            ModeEnter();
            break;
        case WiFi_SetSSID:
        case WiFi_SetPassword:
            
            switch(Type){
                case 1:
                    sellecting_char--;
                    if(sellecting_char<0)sellecting_char+=73;
                    break;
                case 2:
                    EnterCharacter(sellecting_char);
                    break;
                case 3:
                    sellecting_char++;
                    if(sellecting_char>73)sellecting_char-=73;
            }
            IsDraw=true;
            
            break;
        
        default:
            sellectMode=-1;
            ModeEnter();
            
    }
}
void Connect::EnterCharacter(int val){
    if(GetSoftKeyboardChar(val)>=0&&GetSoftKeyboardChar(val)<16){ //ここはコマンド
        switch(GetSoftKeyboardChar(val)){
            case 0://キャンセル
            TempSetString="";
            sellectMode=-1;
            ModeEnter();
            break;
            case 1://保存
            switch(mode){
                case WiFi_SetSSID:
                    ssid=TempSetString;
                break;
                case WiFi_SetPassword:
                    password=TempSetString;
                break;
            }
            SaveProfile();
            sellectMode=-1;
            ModeEnter();
            break;
            case 2://BackSpace
            if(TempSetString.length()>0)TempSetString.remove(TempSetString.length()-1,1);
            break;
            case 3://大文字小文字切り替え
            IsLargeCharMode=!IsLargeCharMode;
            break;
        }
    }else if(GetSoftKeyboardChar(val)>15){   //文字の入力
        int maxsize=1;
        switch(mode){
            case WiFi_SetSSID:
            maxsize=WIFI_SSID_CHAR;
            break;
            case WiFi_SetPassword:
            maxsize=WIFI_PASSWORD_CHAR;
            break;

        }
        if(TempSetString.length()<maxsize)TempSetString+=GetSoftKeyboardChar(val);
    }
}

void Connect::ModeEnter(){
    IsFirstDraw=true;
    IsDraw=true;
    mode=sellectMode;
    M5.lcd.fillRect(0,0,320,240,BLACK);
    switch(mode){
        case WiFi_Return:
        toHome=1;
        break;
        case WiFi_SetSSID:
            TempSetString=ssid;
        break;
        case WiFi_SetPassword:
            TempSetString=password;
            break;
        case Menu:
            sellectMode=0;
            break;
        case WiFi_Test:
            scroll=0;
            testmode=0;
            tempmode=-1;
            connectingTime=millis();
            WiFi.begin(ssid.c_str(),password.c_str());
        /*
            
            scrollX=0;
            testmode=0;
            WiFi.begin(ssid.c_str(), password.c_str());
            startTimer=millis();*/
            break;
    }
}
String Connect::GetMacAddress(){
    uint8_t* baseMac=new uint8_t[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char* baseMacChr = new char[18];
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", 
    baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    String Result=baseMacChr;
    delete[] baseMac;
    delete[] baseMacChr;
    return Result;
}
char Connect::GetSoftKeyboardChar(int value){
    if(value==0)return 0;   //CANCEL
    if(value==1)return 1;   //SAVE
    if(value==2)return 2;   //BACKSPACE
    if(value>=3&&value<=12)return '0'+value-3;
    if(value==13)return ' ';//スペース
    if(value==14)return 3;  //Aa切替
    if(value>=15&&value<=40)return 'A'+value-15+32*!IsLargeCharMode;
    if(value>=41&&value<=55)return '!'+value-41;
    if(value>=56&&value<=62)return ':'+value-56;
    if(value>=63&&value<=68)return '['+value-63;
    if(value>=69&&value<=72)return '{'+value-69;
    return 15; //範囲外

}
bool Connect::GetUpDateDraw(){
    if(testmode==1&&mode==WiFi_Test)return 1;
    return IsDraw;
}
bool Connect::GetGoToHome(){
    return toHome;
}
void Connect::Loop(){
    switch(mode){
        case WiFi_Test:
        if(testmode==1){
            if(WiFi.status()==WL_CONNECTED){testmode++;}
            if(millis()-connectingTime>20000){
                testmode=1000;
                
            }
            IsDraw=true;
        }
        if(testmode==65535){
            //WiFi.disconnect(true);
            testmode=65536;
        }
        break;
    }
}