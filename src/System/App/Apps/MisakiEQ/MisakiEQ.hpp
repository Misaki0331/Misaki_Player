#include <ArduinoJson.h>
#include <M5Stack.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "../../../ui/Setting.h"
#define PingData 300
using namespace Core::Setting;
namespace App
{
    namespace MisakiEQ
    {
        class Map{
            public:
            void Draw(double longitude ,double latitude,uint8_t size);
            private:
            void DrawPnt(short x, short y,uint8_t size);
            void DrawLeft(short x0, short y0,uint8_t size);

            //マップ配列(5サイズ)
            static const uint16_t map_2500_bin[];
            static const uint16_t map_2500_ptr[];
            static const uint16_t map_2000_bin[];
            static const uint16_t map_2000_ptr[];
            static const uint16_t map_1500_bin[];
            static const uint16_t map_1500_ptr[];
            static const uint16_t map_1000_bin[];
            static const uint16_t map_1000_ptr[];
            static const uint16_t map_500_bin[];
            static const uint16_t map_500_ptr[];
        };
        class EEW
        {
        public:
            EEW();
            ~EEW();
            void Begin();
            void Exit();
            void Loop();
            void Draw();
            void PressButton(int type);
            bool GetDrawUpdate();
            bool IsHome();
            bool GetActive();
            String GetAppName();
            bool GetBackGround();
            void BackGround();
            int LoadTime();
            void ModeEnter();

        private:
            bool IsActive;
            bool First;
            static void GetNetworkFile(void *args);
            int GetColor(unsigned int col);
            static DynamicJsonDocument json;
            static bool FinishedThread;
            static bool RunThread;
            static bool IsHttpError;
            static int LatestReadTime;
            static int JsonReadTime;
            bool IsButtonUIUpdate;
            bool IsFirstBegin;
            bool IsUpdated;
            static bool IsPingUpdate;
            bool IsFirstDrawed;
            bool toHome;
            bool IsGetChanged;
            int TempTime;
            int JsonState;
            int JsonOldState;
            int regionUpdate;
            bool IsRegionUpdate;
            bool WarnRegionDisplay;
            static bool IsCheck;
            bool FirstCheck;
            int pg;
            int regPos;
            int mode;
            int sellectMode;
            int tempoffmode;
            static short *PingValue;
            static short *PingValue12sec;
            static short *PingValue60sec;
            static int PingCount;
            static bool IsPingOpen;
            static String LatestHttpError;
            static HTTPClient *http;
            enum ModeType
            {
                ExitMode = -1,
                EEWMode = 0,
                MapMode =1,
                PingMode = 2,
                SettingMode = 3,
                SettingNum = -999,
                SettingList = -998,
                SettingPath = -997
            };
            int settingSellect;
            bool IsNotCursorMode;
            bool IsSettingUIUpdate;
            void SettingEnter();

            Num cNum;
            List cList;
            Path cPath;
            struct Configs
            {
                String ForecastSoundPath;
                bool ForecastSoundPerSerial;
                String WarnSoundPath;
                bool WarnSoundPerSerial;
                String OnlyListEvent;
                int LCDoffTimer;
                int LCDoffTimerBattery;
                int RebootTimer;
                int LCDLightLvSupply;
                int LCDLightLvBattery;
            } config;

            enum SettingVar
            {
                ForecastSoundPath,
                ForecastSoundPerSerial,
                WarnSoundPath,
                WarnSoundPerSerial,
                OnlyListEvent,
                LCDoffTimer,
                LCDLightLvSupply,
                LCDoffTimerBattery,
                LCDLightLvBattery,
                RebootTimer,
                
                ExitSetting,
                TestMode
            };

            enum PingGraphMode{
                Ping_60secondsMode,
                Ping_5minsMode,
                Ping_1hoursMode,
                Ping_5hoursMode
            };
            int pingGraphMode;
            void ReadConfig();
            void SaveConfig();
            int Reboottimer;
            bool IsnotLCDLight;
            int LCDTimer;
            bool IsFirstEEWForecast;
            bool IsFirstEEWWarn;
            void CallSoundForecast();
            void CallSoundWarn();
            void CallSoundReset();
            static const String PrefList[];
            static const String TestJson;
            static int TestTime;
            int Split(String data, char delimiter, String *dst);
            int GetConfigName(String str);
            void PingSave();
            void PingLoad();
            bool TempIsBatterySupply;
            String BoolToStr(bool val);
            void LCDLightUp();
            Map map;
            bool IsMapMode;
            int MapSize;

            
        };
        
    }

}