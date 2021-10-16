#include<M5Stack.h>
#define RAMSIZE 327680
namespace Core{
    class SystemData
    {
        public:
        SystemData();
        static int LoopCount;
        static bool UpdateBatteryUI;
        static int TempBatteryPercent;
    }static systemData;
    class SystemConfig
    {
        public:
        SystemConfig();
        static int BatteryPosX;
        static int BatteryPosY;
        static bool EnableALLUpdate;
        static uint16_t UIUsageCPU_TextColor;
        static uint16_t UIUsageCPU_BackColor;
        static uint16_t UIUpTime_TextColor;
        static uint16_t UIUpTime_BackColor;
        
    }static systemConfig;
}