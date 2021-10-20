#include"ui.h"
using namespace Core;
void DrawTaskUI::Battery(int x,int y,int percent,bool isAllDraw){
    bool IsCharging=false;
    bool IsFull=false;
    short color=WHITE;
    if(percent>=200){
        IsCharging=1;
        percent-=200;
        color=ORANGE;
    }
    if(percent>=400){
        IsFull=1;
        percent-=400;
        color=GREEN;
    }
    
    if(isAllDraw){
        M5.Lcd.fillRect(x+2,y,21,12,BLACK);
        M5.Lcd.fillRect(x,y+3,3,6,BLACK);
        M5.Lcd.drawRect(x+3,y+1,19,10,color);
        M5.Lcd.drawRect(x+1,y+4,3,4,color);
        M5.Lcd.fillRect(x+2,y+5,2,2,BLACK);
    }
    int per=percent;
    if(per>50){
        color=GREEN;
    }else if(per>25){
        color=YELLOW;
    }else{
        color=RED;
    }
    for(int i=3;i>=0;i--){
        if(per>=25){
            M5.Lcd.fillRect(x+5+4*i,y+3,3,6,color);
        }else{
            M5.Lcd.fillRect(x+5+4*i,y+3,3,6,BLACK);
        }
        per-=25;
    }
}