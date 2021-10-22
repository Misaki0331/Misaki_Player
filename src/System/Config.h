#include<M5Stack.h>
#define RAMSIZE 327680
namespace Core{
    class SystemData
    {
        public:
        SystemData();
        static int LoopCount;
        static int LPS;
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
    class SystemAPI{
        public:
        static int LPS;//毎秒あたりのループカウント
        static int FLPS; //毎秒当たりの処理余裕カウント
        static int FreeRAM; //RAMの空き容量
        static int FreeROM; //ROMの空き容量
        static int BatteryLeft; //残りバッテリー値
        static int WiFiLevel; //Wi-Fi強度
        static bool WiFiIsConnected; //Wi-Fiの接続状態
    }static systemAPI;
}