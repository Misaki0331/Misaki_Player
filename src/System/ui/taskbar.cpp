#include"ui.h"
using namespace Core;
void DrawTaskUI::Battery(int x,int y,int percent,bool isAllDraw){
    if(isAllDraw){
        M5.Lcd.fillRect(x+2,y,21,12,BLACK);
        M5.Lcd.fillRect(x,y+3,3,6,BLACK);
        M5.Lcd.drawRect(x+3,y+1,19,10,WHITE);
        M5.Lcd.drawRect(x+1,y+4,3,4,WHITE);
        M5.Lcd.fillRect(x+2,y+5,2,2,BLACK);
    }
    int per=percent;
    short color;
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