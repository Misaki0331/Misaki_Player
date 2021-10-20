#include"connect.h"
#include<EEPROM.h>
using namespace App::WiFi;
Connect::Connect(){

}
char* Connect::GetAppName(){
    return "Wi-Fi Connection Setting";
}
void Connect::Begin(){
    ReadProfile();
}
void Connect::SaveProfile(){
    for(int i=0;i<WIFI_SSID_CHAR;i++){
        if(ssid.length()>=i){
            EEPROM.writeByte(i+WIFI_OFFSET,0);
        }else{
            EEPROM.writeByte(i+WIFI_OFFSET,ssid.charAt(i));
        }
    }
    for(int i=0;i<WIFI_PASSWORD_CHAR;i++){
        if(password.length()>=i){
            EEPROM.writeByte(i+WIFI_SSID_CHAR+WIFI_OFFSET,0);
        }else{
            
            EEPROM.writeByte(i+WIFI_SSID_CHAR+WIFI_OFFSET,password.charAt(i));
        }
    }
}
void Connect::ReadProfile(){
    char* Temp;
    Temp=new char[WIFI_SSID_CHAR+1];
    for(int i=0;i<=WIFI_SSID_CHAR;i++){
        Temp[i]=EEPROM.readByte(i+WIFI_OFFSET);
        if(Temp[i]==NULL)break;
    }
    Temp[WIFI_SSID_CHAR]=NULL;
    ssid=Temp;
    delete[] Temp;
    Temp=new char[WIFI_PASSWORD_CHAR+1];
    for(int i=0;i<=WIFI_SSID_CHAR;i++){
        Temp[i]=EEPROM.readByte(i+WIFI_SSID_CHAR+WIFI_OFFSET);
        if(Temp[i]==NULL)break;
    }
    Temp[WIFI_PASSWORD_CHAR]=NULL;
    password=Temp;
    delete[] Temp;
}