#include "main.h"
#include "ui/ui.h"
#include "config.h"
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
    appSelecter.Update();
}
int Main::UpdateUI=0;
int Main::TempMs=0;
void Main::Draw(){
    drawUI.Battery(systemConfig.BatteryPosX,systemConfig.BatteryPosY,
                BatteryPercent,systemConfig.EnableALLUpdate);
}
void Main::Loop(){
    //主な処理
    M5.update(); 
    unsigned int Buttons=0;
    if(M5.BtnA.wasPressed())Buttons |= (1<<0);
    if(M5.BtnB.wasPressed())Buttons |= (1<<1);
    if(M5.BtnC.wasPressed())Buttons |= (1<<2);
    if(M5.BtnA.isPressed()) Buttons |= (1<<3);
    if(M5.BtnB.isPressed()) Buttons |= (1<<4);
    if(M5.BtnC.isPressed()) Buttons |= (1<<5);
    appSelecter.SetButtonStatus(Buttons);
    appSelecter.Loop();
    appSelecter.Draw();

    int MilliSecounds=millis();
    if(MilliSecounds/1000>UpdateUI){
       UpdateUI=MilliSecounds/1000;
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(0, 0);
        //M5.Lcd.fillRect(0,0,200,10,BLACK);
         M5.Lcd.setTextColor(WHITE,BLACK);
         int MaxLPS=-1;
         switch(getCpuFrequencyMhz()){
             case 240: MaxLPS=851000; break;
             case 160: MaxLPS=581800; break;
             case 80: MaxLPS=294200; break;
             case 40: MaxLPS=140700; break;
             case 20: MaxLPS=62500; break;
             case 10: MaxLPS=23550; break;
             default: MaxLPS=842000; break;

         }
         int CPULoad =(int)(((sqrt(MaxLPS)-sqrt(MainLPS))/sqrt(MaxLPS))*10000);
        if(CPULoad<0)CPULoad=0;
        if(CPULoad>99999)CPULoad=99999;
        M5.Lcd.printf("CPU:%3d.%02d%%",CPULoad/100,CPULoad%100);
    }
    
           /* M5.Lcd.setCursor(0, 20);
            M5.Lcd.setTextColor(WHITE,BLACK);
            //M5.Lcd.printf("%7d.%ds",millis()/1000,millis()/100%10);
        
            if(M5.BtnA.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                wavePlayer.SetFileName("/oruga.wav");
                M5.Lcd.printf("Play:%d                 ",wavePlayer.Play());
            }
            if(M5.BtnB.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                //wavePlayer.Stop();
                //M5.Lcd.printf("Stop:%d             ",wavePlayer.GetSeek());
                wavePlayer.SetVolume(wavePlayer.GetVolume()-1);
                M5.Lcd.printf("Volume:%3d",wavePlayer.GetVolume());
            }
            if(M5.BtnC.wasReleased()){
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.setTextColor(WHITE,BLACK);
                //wavePlayer.SetFileName("/oruga.wav");
                //M5.Lcd.printf("File Setted             ");
                wavePlayer.SetVolume(wavePlayer.GetVolume()+1);
                M5.Lcd.printf("Volume:%3d",wavePlayer.GetVolume());
            
            
        }*/
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
            
        }
        vTaskDelay(1000);
    }
}

int Main::MainLPS=0;
int Main::BatteryPercent=0;
bool Main::DisableUI=false;

void Main::SoundThread(void* arg){
    while(1){
        
        wavePlayer.Loop();
    }
}
int SystemData::LoopCount=0;
bool SystemData::UpdateBatteryUI=false;
int SystemData::TempBatteryPercent=0;
int SystemConfig::BatteryPosX=0;
int SystemConfig::BatteryPosY=0;
bool SystemConfig::EnableALLUpdate=0;
App::System::Select Main::appSelecter;
