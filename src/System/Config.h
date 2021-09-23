#include<M5Stack.h>
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
    }static systemConfig;
}