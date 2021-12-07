#include<M5Stack.h>
#define RAMSIZE 327680
#define CHATTERING_DELAY 12
#ifndef API_H
#define API_H
namespace Core{
    class SystemData
    {
        public:
        SystemData();
        static int LoopCount;
        static int LPS;
        static bool UpdateBatteryUI;
        static int TempBatteryPercent;
        static bool UpdateSignalUI;
        static bool IsHttpPressA;
        static bool IsHttpPressB;
        static bool IsHttpPressC;
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
        static bool BatteryIsCharging;
        static bool BatteryIsFull;
        static bool BatteryIsSupply;
        static int WiFiLevel; //Wi-Fi強度
        static bool WiFiIsConnected; //Wi-Fiの接続状態
        static uint8_t WiFiCurrentProfile;
        
    }static systemAPI;
}
#endif