#include"Config.h"
using namespace Core;
SystemData::SystemData(){
    LoopCount=0;
    UpdateBatteryUI=true;
    TempBatteryPercent=0;
}
SystemConfig::SystemConfig(){
    BatteryPosX=297;
    BatteryPosY=0;
    EnableALLUpdate=false;
    UIUsageCPU_BackColor=0x0000;
    UIUsageCPU_TextColor=0xFFFF;
    UIUpTime_BackColor=0x0000;
    UIUpTime_TextColor=0xFFFF;
}

uint16_t SystemConfig::UIUpTime_BackColor=0x0000;
uint16_t SystemConfig::UIUpTime_TextColor=0xFFFF;

uint16_t SystemConfig::UIUsageCPU_BackColor=0x0000;
uint16_t SystemConfig::UIUsageCPU_TextColor=0xFFFF;