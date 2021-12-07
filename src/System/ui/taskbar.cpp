#include"ui.h"
#include"../Fonts/FastFont.h"
using namespace Core::Draw;
using namespace Core;
void DrawTaskUI::Battery(int x,int y,int percent,bool isAllDraw){
    short color=WHITE;
    if(percent>=200){
        percent-=200;
        color=ORANGE;
    }
    if(percent>=400){
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
void DrawTaskUI::RSSI(int x,int y,int rssi,int profileID,bool isConnected,bool isAllDraw){
    /*if(isAllDraw){
        M5.Lcd.drawFastHLine(x+1,y+1,7,WHITE);
        M5.Lcd.drawFastVLine(x+4,y+1,10,WHITE);
        M5.Lcd.drawLine(x+1,y+1,x+1+3,y+1+3,WHITE);
        M5.Lcd.drawLine(x+7,y+1,x+1+3,y+1+3,WHITE);
    }*/
    uint16_t col;
    char* txt=new char[5];
    sprintf(txt,"%02d",profileID);
    FastFont::printSmall(txt,x,y+1,GREEN,1,BLACK);
    delete[] txt;
    if(isConnected){
    if(rssi>=-60){
        col=GREEN;
    }else if(rssi>=-70){
        col=YELLOW;
    }else{
        col=RED;
    }
    for(int i=0;i<20;i++){
        if(-80+i*2<=rssi){
            M5.Lcd.drawFastVLine(x+i,y+1+9-i/2,i/2+1,col);
        }else{
            M5.Lcd.drawFastVLine(x+i,y+1+9-i/2,i/2+1,0x4208);
        }
        M5.Lcd.drawPixel(x+i,y+1+9-i/2-1,BLACK);
        M5.Lcd.drawPixel(x+i,y+1+9+1,BLACK);
    }
    }else{
        for(int i=0;i<20;i++){
        
            M5.Lcd.drawFastVLine(x+i,y+1+9-i/2,i/2+1,0x4208);
        M5.Lcd.drawPixel(x+i,y+1+9-i/2-1,BLACK);
        M5.Lcd.drawPixel(x+i,y+1+9+1,BLACK);
        }
        M5.Lcd.drawLine(x+15-1,y+7-1,x+15+4-1,y+7+4-1,RED);
        M5.Lcd.drawLine(x+15-1,y+11-1,x+15+4-1,y+11-4-1,RED);
        

    }
    M5.Lcd.drawFastVLine(x+20,y,12,BLACK);
    M5.Lcd.drawFastVLine(x-1,y+10,2,BLACK);
    M5.Lcd.drawFastVLine(x-2,y+10,2,BLACK);
    //M5.Lcd.drawFastVLine(x+10-3,y+11,1,BLACK);
    //M5.Lcd.drawFastVLine(x+10-4,y+11,1,BLACK);
}