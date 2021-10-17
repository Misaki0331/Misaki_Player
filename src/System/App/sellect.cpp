#include"sellect.h"
#include"../Config.h"
#include"../Fonts/FastFont.h"
#define DRAW_UPDATE 16
using namespace Core::Draw;
namespace AppSelect = Core::Apps::System;
namespace Config = Core;
void AppSelect::Select::SetButtonStatus(unsigned int value){
    if ((value & (1<<0))&&(value & (1<<0))!=(ButtonStatus & (1<<0)))PressButtonA();
    if ((value & (1<<1))&&(value & (1<<1))!=(ButtonStatus & (1<<1)))PressButtonB();
    if ((value & (1<<2))&&(value & (1<<2))!=(ButtonStatus & (1<<2)))PressButtonC();
    if ((value & (1<<3))!=(ButtonStatus & (1<<3)))HoldButtonA();
    if ((value & (1<<4))!=(ButtonStatus & (1<<4)))HoldButtonB();
    if ((value & (1<<5))!=(ButtonStatus & (1<<5)))HoldButtonC();
    if(ButtonStatus!=value){//Home
        if((value & (1<<3))&&(value & (1<<4))&&(value & (1<<5))){
            
            SellectInit();
        }
    }
    ButtonStatus=value;
}
void AppSelect::Select::Loop(){
    
    for(int i=0;i<applist.GetAppCount();i++){
        applist.BackGround(i);
    }
    if(RunAppValue>=0){
        if(applist.GetGoToHome(RunAppValue)){
            SellectInit();
        }
        applist.Loop(RunAppValue);
    }
}
void AppSelect::Select::InSellectDraw(){
    
        if(!IsFirstDrawFlg){
            M5.Lcd.fillRect(0,0,320,240,UIBGColor);
            M5.Lcd.fillRect(0,20,320,230,MenuBGColor);
            FastFont::printRom("Application Menu",0,10,WHITE,1,UIBGColor);
            Config::systemConfig.EnableALLUpdate=1;
            IsFirstDrawFlg=true;
        }
    for(int i=0;i<applist.GetAppCount();i++){
        char AppName[64];
        sprintf(AppName,"%d : %s",i,applist.GetAppName(i));
        if(i==sellectAppID){
            M5.Lcd.fillRect(0,20+i*10-1,strlen(AppName)*6,10,WHITE);
            FastFont::printRom(AppName,0,20+i*10,BLACK,1,WHITE);
        }else{
            M5.Lcd.fillRect(0,20+i*10-1,strlen(AppName)*6,10,MenuBGColor);
            FastFont::printRom(AppName,0,20+i*10,WHITE,1,MenuBGColor);
        }
    }
    
}
void AppSelect::Select::Draw(){
    int t=millis();
    if(drawDelay!= t/DRAW_UPDATE){
        drawDelay=t/DRAW_UPDATE ;
    
    if(RunAppValue>=0)IsDrawUpdate=applist.GetUpdateDraw(RunAppValue);
    if(IsDrawUpdate){
    switch(RunAppValue){
        case -1:
        InSellectDraw();
        break;
        default:
        applist.Draw(RunAppValue);
    }
    IsDrawUpdate=false;
    }
    }
}
void AppSelect::Select::Update(){
    IsDrawUpdate=true;
}
void AppSelect::Select::Begin(){
    SellectInit();
    
}
void AppSelect::Select::SellectInit(){
    if(RunAppValue>=0)applist.Exit(RunAppValue);
    RunAppValue=-1;
    systemConfig.UIUpTime_BackColor=UIBGColor;
    systemConfig.UIUsageCPU_BackColor=UIBGColor;
    systemConfig.UIUpTime_TextColor=WHITE;
    systemConfig.UIUsageCPU_TextColor=WHITE;
    systemData.UpdateBatteryUI=true;
    IsFirstDrawFlg=0;
    IsDrawUpdate=true;
}
void AppSelect::Select::PressButtonA(){
    switch(RunAppValue){
        case -1:
            if(sellectAppID>0)sellectAppID--;
            IsDrawUpdate=1;
        break;
        default:
        applist.ButtonPress(RunAppValue,1);
    }
}
void AppSelect::Select::PressButtonB(){
    switch(RunAppValue){
        case -1:
            RunAppValue=sellectAppID;
            systemConfig.UIUpTime_BackColor=BLACK;
            systemConfig.UIUsageCPU_BackColor=BLACK;
            systemConfig.UIUpTime_TextColor=WHITE;
            systemConfig.UIUsageCPU_TextColor=WHITE;
            M5.Lcd.fillRect(0,0,320,240,BLACK);
            applist.Begin(RunAppValue);
            break;
        default:
        applist.ButtonPress(RunAppValue,2);
    }

}
void AppSelect::Select::PressButtonC(){
    switch(RunAppValue){
        case -1:
            if(sellectAppID<applist.GetAppCount()-1)sellectAppID++;
            IsDrawUpdate=1;
        break;
        default:
        applist.ButtonPress(RunAppValue,3);
    }
}
void AppSelect::Select::HoldButtonA(){
    if(RunAppValue>=0)applist.ButtonHold(RunAppValue,1,ButtonStatus&(1<<3));
}
void AppSelect::Select::HoldButtonB(){
    if(RunAppValue>=0)applist.ButtonHold(RunAppValue,2,ButtonStatus&(1<<4));
}
void AppSelect::Select::HoldButtonC(){
    if(RunAppValue>=0)applist.ButtonHold(RunAppValue,3,ButtonStatus&(1<<5));

}
//unsigned int AppSelect::Select::ButtonStatus=0;
//bool AppSelect::Select::IsDrawUpdate=false;