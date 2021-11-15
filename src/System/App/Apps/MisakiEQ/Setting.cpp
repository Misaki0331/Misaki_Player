#include"MisakiEQ.hpp"
#include"../../../Fonts/FastFont.h"
using namespace Core::Draw;
using namespace App::MisakiEQ::Setting;

bool Path::GetIsUpdate(){
    if(IsUpdate){
        IsUpdate=false;
        return 1;
    }
    return 0;
}
bool List::GetIsUpdate(){
    if(IsUpdate){
        IsUpdate=false;
        return 1;
    }
    return 0;
}
bool Num::GetIsUpdate(){
    if(IsUpdate){
        IsUpdate=false;
        return 1;
    }
    return 0;
}

bool Path::GetIsSetting(){
    return IsSetting;
}
bool List::GetIsSetting(){
    return IsSetting;
}
bool Num::GetIsSetting(){
    return IsSetting;
}

void Path::Begin(String* path){
    IsSetting=true;
    ptr=path;
}
void Path::Button(int type){
    //ここにボタン処理
    IsUpdate=1;
}
void Path::Draw(){
    if(!IsFirstDraw){
        IsFirstDraw=true;
        FastFont::printUtf8(title,0,15,YELLOW,2,INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle,0,48,0x7BEF,1,INVISIBLE_COLOR);
        //ここに画面初期化処理
    }
    //ここに描画処理
}
void Path::Release(){
    ptr=nullptr;
    title.clear();
    subTitle.clear();
}
void Path::Cancel(){
    IsSetting=false;
}

void List::Begin(String* name,String* args){
    ptr=name;
    listArgs=args;
}
void List::Button(int type){
    //ここにボタン処理
    IsUpdate=1;
}
void List::Draw(){
    if(!IsFirstDraw){
        IsFirstDraw=true;
        FastFont::printUtf8(title,0,15,YELLOW,2,INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle,0,48,0x7BEF,1,INVISIBLE_COLOR);
        //ここに画面初期化処理
    }
    //ここに描画処理
}
void List::Release(){
    ptr=nullptr;
    title.clear();
    subTitle.clear();
}
void List::Cancel(){
    IsSetting=false;
}

void Num::Begin(int* value,uint8_t degit){
    temp=0;
    sellectDegit=0;
    IsSetting=true;
    ptr=value;
    temp=*ptr;
    totalDegit=degit;
    IsFirstDraw=false;
    IsUpdate=true;
    if(totalDegit>9)totalDegit=9;
    
}
void Num::Button(int type){
    //ここにボタン処理
    switch(type){
        case 1:
            if(IsDegitMode){
                if(temp/powi(10,totalDegit-1-sellectDegit)%10==0){
                    temp+=powi(10,totalDegit-1-sellectDegit)*9;
                }else{
                    temp-=powi(10,totalDegit-1-sellectDegit);
                }
            }else{
            if(sellectDegit>=0)sellectDegit--;
            }
            break;
        case 2:
            if(sellectDegit>=0&&sellectDegit<totalDegit){
                IsDegitMode=!IsDegitMode;
            }else{
                if(sellectDegit==-1){
                    IsSetting=false;
                }else{
                    IsSetting=false;
                    *ptr=temp;
                }
            }
            break;
        case 3:
            if(IsDegitMode){
                if(temp/powi(10,totalDegit-1-sellectDegit)%10==9){
                    temp-=powi(10,totalDegit-1-sellectDegit)*9;
                }else{
                    temp+=powi(10,totalDegit-1-sellectDegit);
                }
            }else{
            if(sellectDegit<totalDegit)sellectDegit++;
            }
    }
    IsUpdate=1;
}
void Num::Draw(){
    if(!IsFirstDraw){
        IsFirstDraw=true;
        FastFont::printUtf8(title,0,15,YELLOW,2,INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle,0,48,0x7BEF,1,INVISIBLE_COLOR);
        for(int i=0;i<totalDegit&&i<10;i++){
            M5.lcd.drawRect(160-17*totalDegit+34*i,96,5*6+4,46,0x7BEF);
        }
        //ここに画面初期化処理
        
    }
    
    //ここに描画処理
    char* valchar=new char[5];
    for(int i=0;i<totalDegit&&i<10;i++){
        sprintf(valchar,"%c",'0'+temp/powi(10,totalDegit-i-1)%10);
        if(i==sellectDegit){  
            if(IsDegitMode){
                FastFont::printRom(valchar,160-17*totalDegit+34*i+2,98,GREEN,6,BLACK);
            }else{
                FastFont::printRom(valchar,160-17*totalDegit+34*i+2,98,YELLOW,6,BLACK); 
            }  
        }else{
            FastFont::printRom(valchar,160-17*totalDegit+34*i+2,98,WHITE,6,BLACK);   
        }
        
    }
    if(sellectDegit==-1){
            M5.lcd.fillRect(30,170,6*6*2+3,8*2+3,WHITE);
        }else{
            
            M5.lcd.fillRect(30,170,6*6*2+3,8*2+3,BLACK);
            M5.lcd.drawRect(30,170,6*6*2+3,8*2+3,WHITE);
        }
        FastFont::printRom("Cancel",32,172,sellectDegit==-1?BLACK:WHITE,2);
        if(sellectDegit==totalDegit){
            M5.lcd.fillRect(215,170,6*6*2+3,8*2+3,WHITE);
        }else{
            M5.lcd.fillRect(215,170,6*6*2+3,8*2+3,BLACK);
            M5.lcd.drawRect(215,170,6*6*2+3,8*2+3,WHITE);
        }
        FastFont::printRom("  OK  ",215,172,sellectDegit==totalDegit?BLACK:WHITE,2);
    delete[] valchar;
}
void Num::Release(){
    ptr=nullptr;
    totalDegit=0;
    title.clear();
    subTitle.clear();
}
void Num::Cancel(){
    IsSetting=false;
}
long Num::powi(int x,int y){
    if(y<0)return 0;
    if(y==0)return 1;
    if(y>=32)return -2147483648;
    int val=x;
    for(int i=1;i<y;i++)val*=x;
    return val;
}