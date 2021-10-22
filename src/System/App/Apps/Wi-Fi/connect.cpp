#include"connect.h"
#include<EEPROM.h>
#include"../../../Fonts/FastFont.h"

#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
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
    ReadProfile();
    IsFirstDraw=1;
    IsDraw=1;
    sellecting_char=0;
    IsLargeCharMode=0;
    mode=Menu;
    TempSetString="";
    sellectMode=0;
    toHome=0;
}
void Connect::SaveProfile(){
    
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    File config = fs.open("/Wi-Fi_00.ini",FILE_WRITE);
    if(!config)return;
    config.println(ssid);
    config.println(password);
    config.close();
    SPIFFS.end();
}
void Connect::ReadProfile(){
    SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
    fs::FS fs = SPIFFS;
    File config = fs.open("/Wi-Fi_00.ini",FILE_READ);
    if(!config)return;
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
            break;
            /*
            {
                switch(testmode){
                    case -1:{
                    FastFont::printConsole("|*9Connection Failed.",0,20+scrollX);
                    testmode=-32768;}
                    break;
                    case 0:{
                    char *text2=new char[64];
                    int t=millis()-startTimer;
                    sprintf(text2,"Wi-Fi Connecting... |*f[%d:%02d:%03d]",t/60000,t/1000%60,t%1000);
                    FastFont::printConsole(text2,0,20+scrollX);
                    delete[] text2;}
                    break;
                    case 1:{
                    FastFont::printConsole("|*aConnected!",0,20+scrollX);
                    testmode++;
                    scrollX+=8;}
                    break;
                    
                }

                
                break;
            }*/

        }
    }
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
            //WiFi.disconnect();
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
    return IsDraw;
}
bool Connect::GetGoToHome(){
    return toHome;
}
void Connect::Loop(){
    switch(mode){
        case WiFi_Test:
        /*
        IsDraw=1;
            switch(testmode){
                case 0:
                    if(WiFi.status()==WL_CONNECTED){
                        scrollX+=8;
                        testmode=1;
                    }
                    if(WiFi.status()==WL_CONNECT_FAILED){

                        scrollX+=8;
                        testmode=-1;
                    }
                break;
            }*/
        break;
    }
}