#include "main.h"
#include "ui/ui.h"
using namespace Core;
Main::Main(){


}
void Main::Begin(){
    M5.begin();                       //M5Stackを初期化
    M5.Power.begin();                 //M5Stackのバッテリ初期化
    setCpuFrequencyMhz(240);
    M5.Lcd.clear(BLACK);              //表示リセット
    wavePlayer.Begin();
    Serial.begin(115200);
    xTaskCreatePinnedToCore(ControlThread, "ControlThread", 1024, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(SoundThread, "SoundThread", 8192, NULL, 1, NULL, 1);
    Serial.println("Welcome");
    wavePlayer.SetFileName("/sample.wav");
}
int Main::parapara=0;
int Main::TempMs=0;
void Main::Loop(){
    int MilliSecounds=millis();
    if(MilliSecounds%1000==0){
       
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(0, 0);
        //M5.Lcd.fillRect(0,0,200,10,BLACK);
         M5.Lcd.setTextColor(WHITE,BLACK);
         int MaxLPS=-1;
         switch(getCpuFrequencyMhz()){
             case 240: MaxLPS=842000; break;
             case 160: MaxLPS=581800; break;
             case 80: MaxLPS=294200; break;
             case 40: MaxLPS=140700; break;
             case 20: MaxLPS=62500; break;
             case 10: MaxLPS=23550; break;
             default: MaxLPS=842000; break;

         }
         int CPULoad =(int)(((log(MaxLPS)-log(MainLPS))/log(MaxLPS))*10000);
        if(CPULoad<0)CPULoad=0;
        if(CPULoad>99999)CPULoad=99999;
        M5.Lcd.printf("CPU:%3d.%02d%%(%8d)",CPULoad/100,CPULoad%100,MainLPS);
    }
    M5.update(); 
    if(wavePlayer.GetIsPlaying()){
        
        
    }
        
            M5.Lcd.setCursor(0, 20);
            M5.Lcd.setTextColor(WHITE,BLACK);
            //M5.Lcd.printf("%7d.%ds",millis()/1000,millis()/100%10);
        
            if(M5.BtnA.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                M5.Lcd.printf("Play:%d                 ",wavePlayer.Play());
            }
            if(M5.BtnB.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                wavePlayer.Stop();
                M5.Lcd.printf("Stop:%d             ",wavePlayer.GetSeek());
            }
            if(M5.BtnC.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                wavePlayer.SetFileName("/oruga.wav");
                M5.Lcd.printf("File Setted             ");
            
            
            
        }
        while(MilliSecounds==TempMs){
            MilliSecounds=millis();
            systemData.LoopCount++;
        }
        TempMs=MilliSecounds;
}
void Main::ControlThread(void* arg){
    drawUI.Battery(297,0,BatteryPercent,true);
    while(1){
        BatteryPercent = M5.Power.getBatteryLevel();
        MainLPS=systemData.LoopCount;
        systemData.LoopCount=0;
        if(systemData.TempBatteryPercent!=BatteryPercent){
            systemData.TempBatteryPercent=BatteryPercent;
            drawUI.Battery(systemConfig.BatteryPosX,systemConfig.BatteryPosY,
                BatteryPercent,systemConfig.EnableALLUpdate);
        }
        vTaskDelay(1000);
    }
}

int Main::MainLPS=0;
int Main::BatteryPercent=0;
bool Main::DisableUI=false;
SystemData Main::systemData={0,true,0};
SystemConfig Main::systemConfig={297,0,false};

void Main::SoundThread(void* arg){
    while(1){
        
        wavePlayer.Loop();
    }
}