#include"MisakiEQ.hpp"
#include"../../../Fonts/FastFont.h"
using namespace App::MisakiEQ;
using namespace Core::Draw;
/*
M5.begin();

    
    }*/
EEW::EEW(){
    First=0;
}
EEW::~EEW(){
    RunThread=false;
}
void EEW::Begin(){
    IsActive=false;
    toHome=0;
    mode=EEWMode;
    sellectMode=EEWMode;
    IsFirstDrawed=0;
    IsUpdated=0;
    FinishedThread=0;
    RunThread=1;
    WarnRegionDisplay=1;
    pg=1;
    http=new HTTPClient();
    PingValue=new short[PingData];
    IsPingOpen=true;
    xTaskCreatePinnedToCore(GetNetworkFile, "MisakiEQ_EEW", 18000, NULL, 5, NULL, 1);
    
    if(SPIFFS.begin(0)){
    
    fs::FS fs = SPIFFS;
    File fp=fs.open("/MisakiEQ.flg",FILE_WRITE);
    if(fp){
        fp.close();
    }else{
        Serial.println("Error! Cannot created!");
    }
    SPIFFS.end();
    }
    
}
void EEW::Loop(){
    int a=json["Status"]["Code"];
    int b=json["Serial"];
    JsonState=a*10000+b;
    
    if(JsonState!=JsonOldState){
        JsonOldState=JsonState;
        IsUpdated=0;
    }
    if(TempTime!=JsonReadTime){
        TempTime=JsonReadTime;
        char* text=new char[12];
        sprintf(text,"%d",JsonReadTime>9999?9999:JsonReadTime);
        int col=0;
        if(JsonReadTime>500){
            col=RED;
        }else if(JsonReadTime>100){
            col=YELLOW;
        }else{
            col=GREEN;
        }
        m5.Lcd.fillRect(320-4*6,23,24,8,BLACK);
        FastFont::printRom(text,320-4*6+3*(4-strlen(text)),23,col,1,-1);
        delete[] text;
    }
    if(millis()>=regionUpdate+5000&&IsRegionUpdate){
        WarnRegionDisplay=0;
        regionUpdate=millis();
    }

}
void EEW::BackGround(){
    if(!First){
        First=true;
        if(SPIFFS.begin(0)){
    
        fs::FS fs = SPIFFS;
        if(fs.exists("/MisakiEQ.flg")){
            IsActive=true;
            Serial.println("MisakiEQ Found!");
        }else{
            Serial.println("MisakiEQ Not Found!");

        }
        SPIFFS.end();
        }else{
            Serial.println("SPIFFS Error!");
        }
    }
}
bool EEW::GetBackGround(){
    return !First;
}
int EEW::GetColor(unsigned int col){
    int R=col/65536%256;
    int G=col/256%256;
    int B=col%256;
    R/=8;
    G/=4;
    B/=8;
    int rgb=R*64;
    rgb+=G;
    rgb*=32;
    rgb+=B;
    return rgb;
}
void EEW::Draw(){
    switch(mode){
        case EEWMode:
        if(!IsFirstDrawed){
        IsFirstDrawed=1;
        
        FastFont::printUtf8("第",100,27,WHITE,1,BLACK);
        FastFont::printUtf8("報",113+6*3*2,27,WHITE,1,BLACK);
        FastFont::printUtf8("発生時刻",0,50,WHITE,1,BLACK);
        FastFont::printUtf8("震源地",13,78,WHITE,1,BLACK);
        FastFont::printUtf8("規模",26,106,WHITE,1,BLACK);
        FastFont::printUtf8("深さ",160,106,WHITE,1,BLACK);
        FastFont::printRom("Ping",320-4*6,15,WHITE,1,BLACK);
        FastFont::printUtf8("最大震度",320-13*4,126,WHITE,1,BLACK);

    }
    
    if(!IsUpdated){
        IsUpdated=1;
        IsRegionUpdate=false;
        if(json["ParseStatus"]=="Success"){
            if(json["Status"]["Code"]=="00"){
                if(json["Warn"]){
                    M5.lcd.fillRect(0,15,94,24,RED);
                    IsRegionUpdate=true;
                    FastFont::printUtf8(" 警 報 ",0,15,WHITE,2,RED);
                }else{

                    M5.lcd.fillRect(0,15,94,24,BLUE);
                    FastFont::printUtf8(" 予 報 ",0,15,WHITE,2,BLUE);
                }
            }else if(json["Status"]["Code"]=="01"){
                M5.lcd.fillRect(0,15,94,24,GREEN);
            FastFont::printUtf8(" 取 消 ",0,15,WHITE,2,GREEN);
            }

        }else{
            M5.lcd.fillRect(0,15,94,24,BLACK);
            FastFont::printUtf8(" 不 明 ",0,15,WHITE,2,BLACK);
        }
        if(1){//json["ParseStatus"]=="Success"){
        char* text=new char[80];
        int num=json["Serial"];
        if(num>999)num=999;
        if(num<-99)num=-99;
        sprintf(text,"%d",num);
        M5.Lcd.fillRect(113,23,36,16,BLACK);
        FastFont::printRom(text,113+(3-(strlen(text)))*6,25,WHITE,2,BLACK);
        
        M5.Lcd.fillRect(113+6*3*2+1+12,27,110,12,BLACK);
        if(json["Type"]["Code"]==9)FastFont::printUtf8("(最終報)",113+6*3*2+1+12,27,WHITE,1,BLACK);
        if(json["Status"]["Code"]=="10"){
            FastFont::printUtf8("訓練中",113+6*3*2+1+12+55,27,WHITE,1,BLACK);
        }else if(json["Status"]["Code"]=="11"){
            FastFont::printUtf8("訓練取消",113+6*3*2+1+12+55,27,RED,1,BLACK);
        }
        FastFont::printRom(json["OriginTime"]["String"],56,46,WHITE,2,BLACK);
        M5.Lcd.fillRect(56,66,264,24,BLACK);
        FastFont::printUtf8(json["Hypocenter"]["Name"],56,66,WHITE,2,-1);
        FastFont::printRom(json["Hypocenter"]["Magnitude"]["String"],56,97,WHITE,3,BLACK);
        num=json["Hypocenter"]["Location"]["Depth"]["Int"];
        m5.lcd.fillRect(218,94,102,24,BLACK);
        if(num!=-1){
            sprintf(text,"%3d km",num);
            FastFont::printRom(text,194,97,WHITE,3,BLACK);
        }else{ 
            FastFont::printUtf8("ごく浅い",194,94,WHITE,2,BLACK);
        }
        int col;
        int textc=WHITE;
        String str=json["MaxIntensity"]["To"];
        int x0=6;
        if(str=="1"){
            col=GetColor(0x808080);
        }else if(str=="2"){
            col=GetColor(0x4169E1);
        }else if(str=="3"){
            col=GetColor(0x2E8B57);
        }else if(str=="4"){
            col=GetColor(0xbfbf0f);
        }else if(str=="5-"){
            x0=13;
            col=GetColor(0xFF4500);
        }else if(str=="5+"){
            x0=13;
            col=GetColor(0xFF4500);
        }else if(str=="6-"){
            x0=13;
            x0=42-(13*3)/2;
            col=GetColor(0xFFC0CB);
        }else if(str=="6+"){
            textc=RED;
            x0=13;
            col=GetColor(0xFFC0CB);
        }else if(str=="7"){
            x0=13;
            col=GetColor(0x800080);
        }else{
            x0=6;
            str="-";
            col=GetColor(0x808080);
        }
        regionUpdate=millis();
        regPos=0;
        pg=1;
        m5.Lcd.fillRect(268,140,51,51,col);
        FastFont::printUtf8(str,268+(51-x0*3)/2+2,140+(42-24)/2,textc,3,col);
        int x1=2;
        int y1=142;
            M5.lcd.fillRect(0,127,267,82,BLACK);
        if(IsRegionUpdate){
            
            
                M5.lcd.fillRect(0,127,267,13,RED);
        sprintf(text,"以下の地域は強い揺れに警戒      都道府県");
        FastFont::printUtf8(text,1,128,WHITE,1,RED);
        
        M5.lcd.drawRect(0,140,267,69,json["Warn"]?RED:BLACK);
        for(int i=0;;i++){
            String name=json["WarnForecast"]["LocalAreas"][i];
            if(name=="null")break;
            int x=name.length()/3*13+6;
            if(x1+x>265){
                x1=2;
                y1+=13;
                if(y1>=207)break;
            }
            FastFont::printUtf8(name,x1,y1,WHITE,1,BLACK);
            x1+=x;


        }
        }
        delete[] text;
        }
        
    }
    if(!WarnRegionDisplay){
            WarnRegionDisplay=true;
            char* text=new char[100];
            int x1=2;
            int y1=142;
            M5.lcd.fillRect(0,127,267,82,BLACK);
            M5.lcd.drawRect(0,140,267,69,json["Warn"]?RED:BLACK);
            if(regPos==-1){
                M5.lcd.fillRect(0,127,267,13,RED);
            sprintf(text,"以下の地域は強い揺れに警戒      都道府県");
            FastFont::printUtf8(text,1,128,WHITE,1,RED);
                for(int i=0;;i++){
                    String name=json["WarnForecast"]["LocalAreas"][i];
                    if(name=="null"){
                        break;
                    }
                    int x=name.length()/3*13+6;
                    if(x1+x>265){
                        x1=2;
                        y1+=13;
                        if(y1>=207){
                            break;
                        }
                    }
                    FastFont::printUtf8(name,x1,y1,WHITE,1,BLACK);
                    x1+=x;
                }
                pg=1;
                regPos=0;
            }else{
                 M5.lcd.fillRect(0,127,267,13,RED);
                sprintf(text,"以下の地域は強い揺れに警戒        詳細%2d",pg);
                FastFont::printUtf8(text,1,128,WHITE,1,RED);
                for(int i=regPos;;i++){
                    String name=json["WarnForecast"]["Regions"][i];
                    if(name=="null"){
                        regPos=-1;
                        break;
                    }
                    int x=name.length()/3*13+6;
                    if(x1+x>265){
                        x1=2;
                        y1+=13;
                        if(y1>=207){
                            regPos=i;
                            pg++;
                            break;
                        }
                    }
                    FastFont::printUtf8(name,x1,y1,WHITE,1,BLACK);
                    x1+=x;
                }
            }
            delete[] text;
        }
        break;
        case PingMode:
            if(!IsFirstDrawed){
                IsFirstDrawed=1;
            }
            if(!IsUpdated){
                IsUpdated=1;
            }
        break;
    }
    
}
void EEW::Exit(){
    RunThread=0;
    while(!FinishedThread)delay(1);
    json.clear();
    http[0].end();
    http[0].~HTTPClient();
    delete http;
    if(SPIFFS.begin(0)){
    
    fs::FS fs = SPIFFS;
    if(fs.remove("/MisakiEQ.flg")){

    }else{
        Serial.println("Error! Cannot remove!");
    }
    SPIFFS.end();
    }
    IsPingOpen=false;
    delete[] PingValue;
}
bool EEW::GetActive(){
    return IsActive;
}
void EEW::GetNetworkFile(void* args){
    while(RunThread){
        int start=millis();
        http[0].begin("https://api.iedred7584.com/eew/json/");
        
        int httpcode=http[0].GET();
        if(httpcode<0){
                IsHttpError=true;
                LatestHttpError=http[0].errorToString(httpcode);
        }else{
            switch(httpcode){
                case 200:
                    deserializeJson(json, http[0].getString());
                    break;
                default:
                    LatestHttpError=httpcode;
                    break;

            }
        }
        http[0].end();
        int t=millis();
        JsonReadTime=t-start;
        if(IsPingOpen){
        for(int i=1;i<PingData;i++)PingValue[i-1]=PingValue[i];
        PingValue[PingData-1]=JsonReadTime;
        }
        LatestReadTime=t;
        while(millis()-start<1000)vTaskDelay(1);
    }
    FinishedThread=1;
    vTaskDelete(NULL);
    vTaskDelay(250);
    
}
int EEW::LoadTime(){
    return JsonReadTime;
}
bool EEW::GetDrawUpdate(){
    return !IsUpdated||!IsFirstDrawed||!WarnRegionDisplay;
}
String EEW::GetAppName(){
    return "MisakiEQ";
}
bool EEW::IsHome(){
    return toHome;
}
void EEW::PressButton(int type){
    toHome=1;
}
DynamicJsonDocument EEW::json(15000);
bool EEW::FinishedThread=false;
bool EEW::RunThread=false;
bool EEW::IsHttpError=false;
int EEW::LatestReadTime=0;
int EEW::JsonReadTime=0;
String EEW::LatestHttpError="";
HTTPClient* EEW::http;
short* EEW::PingValue;
bool EEW::IsPingOpen=false;
