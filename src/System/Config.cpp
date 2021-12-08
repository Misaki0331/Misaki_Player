#include"Config.h"
using namespace Core;
SystemData::SystemData(){
    LoopCount=0;
    UpdateBatteryUI=true;
    TempBatteryPercent=0;
    UpdateSignalUI=true;
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

int SystemAPI::LPS=0;
int SystemAPI::FLPS=0;
int SystemAPI::FreeRAM=0;
int SystemAPI::FreeROM=0;
int SystemAPI::BatteryLeft=0;
bool SystemAPI::BatteryIsCharging=0;
bool SystemAPI::BatteryIsFull=0;
bool SystemAPI::BatteryIsSupply=0;
int SystemAPI::WiFiLevel=0;
bool SystemAPI::WiFiIsConnected=false;
int SystemData::LPS=0;
bool SystemData::UpdateSignalUI=0;
uint8_t SystemAPI::WiFiCurrentProfile=0;
bool SystemData::IsHttpPressA=false;
bool SystemData::IsHttpPressB=false;
bool SystemData::IsHttpPressC=false;
bool SystemData::IsCaptureAvailable=true;