#include<M5Stack.h>
#include"sleep.h"
#include"../../../Fonts/FastFont.h"
#include"../../../config.h"
App::Sleep::sleep::sleep(){

    
}
void App::Sleep::sleep::Begin(){
    tohome=0;
    M5.Lcd.setBrightness(0);
}
void App::Sleep::sleep::Exit(){
}
void App::Sleep::sleep::ButtonHold(int Type,bool data){}
void App::Sleep::sleep::ButtonPress(int Type){
    M5.Lcd.setBrightness(255);
    tohome=1;
}
void App::Sleep::sleep::Loop(){
}
void App::Sleep::sleep::Draw(){
}
void App::Sleep::sleep::Background(){}
bool App::Sleep::sleep::GetBackGround(){return 0;}
void App::Sleep::sleep::SetBackGround(bool run){return;}
bool App::Sleep::sleep::GetUpDateDraw(){return 1;}
bool App::Sleep::sleep::GetToActive(){return 0;}
char* App::Sleep::sleep::GetAppName(){return "LCD Sleep";}
bool App::Sleep::sleep::GetGoToHome(){
    return tohome;
}