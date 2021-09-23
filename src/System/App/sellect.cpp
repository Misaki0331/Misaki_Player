#include"sellect.h"
#include"../Config.h"
namespace AppSelect = Core::App::System;
namespace Config = Core;
void AppSelect::Select::SetButtonStatus(unsigned int value){
    if ((value & (1<<0))!=(ButtonStatus & (1<<0)))PressButtonA();
    if ((value & (1<<1))!=(ButtonStatus & (1<<1)))PressButtonB();
    if ((value & (1<<2))!=(ButtonStatus & (1<<2)))PressButtonC();
    if ((value & (1<<3))!=(ButtonStatus & (1<<3)))HoldButtonA();
    if ((value & (1<<4))!=(ButtonStatus & (1<<4)))HoldButtonB();
    if ((value & (1<<5))!=(ButtonStatus & (1<<5)))HoldButtonC();
    ButtonStatus=value;
}
void AppSelect::Select::Loop(){}
void AppSelect::Select::Draw(){
    if(IsDrawUpdate){
    M5.Lcd.fillRect(0,0,320,240,UIBGColor);
    M5.Lcd.fillRect(0,20,320,230,MenuBGColor);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.setTextColor(WHITE,UIBGColor);
    M5.Lcd.print("Application Menu");
    Config::systemConfig.EnableALLUpdate=1;
    IsDrawUpdate=false;
    }
}
void AppSelect::Select::Update(){
    IsDrawUpdate=true;
}
void AppSelect::Select::PressButtonA(){}
void AppSelect::Select::PressButtonB(){}
void AppSelect::Select::PressButtonC(){}
void AppSelect::Select::HoldButtonA(){}
void AppSelect::Select::HoldButtonB(){}
void AppSelect::Select::HoldButtonC(){}
//unsigned int AppSelect::Select::ButtonStatus=0;
//bool AppSelect::Select::IsDrawUpdate=false;