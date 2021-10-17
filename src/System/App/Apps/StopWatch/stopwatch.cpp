#include<M5Stack.h>
#include"stopwatch.h"
#include"../../../Fonts/FastFont.h"
#include"../../../config.h"
App::StopWatch::stopwatch::stopwatch(){
    time=0;
    IsStart=0;
    tohome=0;
    
}
void App::StopWatch::stopwatch::Begin(){;
    sprintf(tempText,"             ");
    sprintf(tempT,"     ");
    tohome=0;
}
void App::StopWatch::stopwatch::Exit(){}
void App::StopWatch::stopwatch::ButtonHold(int Type,bool data){}
void App::StopWatch::stopwatch::ButtonPress(int Type){
    switch (Type)
    {
    case 1:
        if(!IsStart)time=0;
        break;
    case 2:
    tohome=1;
        break;
    case 3:
        IsStart=!IsStart;
        break;
    }
}
void App::StopWatch::stopwatch::Loop(){
    int tmp=millis();
    int calc=tmp-temp;
    temp=tmp;
    if(IsStart)time+=calc;

}
void App::StopWatch::stopwatch::Draw(){
    char text[30];
    sprintf(text,"%4d:%02d:%02d",time/3600000,time/60000%60,time/1000%60);
    Core::Draw::FastFont::printFastRom(tempText,text,0,60,WHITE,5,BLACK);
    


    sprintf(tempText,text);
    sprintf(text,"%03d",time%1000);
    Core::Draw::FastFont::printFastRom(tempT,text,266,100,WHITE,3,BLACK);
    sprintf(tempT,text);
    //FastFont::printFastRom(DrawTemp,text,108,10,systemConfig.UIUpTime_TextColor,1,systemConfig.UIUpTime_BackColor);
    //tempText=text;
}
void App::StopWatch::stopwatch::Background(){}
bool App::StopWatch::stopwatch::GetBackGround(){return 0;}
void App::StopWatch::stopwatch::SetBackGround(bool run){return;}
bool App::StopWatch::stopwatch::GetUpDateDraw(){return 1;}
bool App::StopWatch::stopwatch::GetToActive(){return 0;}
char* App::StopWatch::stopwatch::GetAppName(){return "Stop Watch";}
bool App::StopWatch::stopwatch::GetGoToHome(){
    return tohome;
}