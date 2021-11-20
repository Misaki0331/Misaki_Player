#include <ArduinoJson.h>
#include <M5Stack.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "Setting.h"
#define PingData 300
namespace App
{
    namespace MisakiEQ
    {

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
                PingMode = 1,
                SettingMode = 2,
                SettingNum = -999,
                SettingList = -998,
                SettingPath = -997
            };
            int settingSellect;
            bool IsNotCursorMode;
            bool IsSettingUIUpdate;
            void SettingEnter();

            MisakiEQ::Setting::Num cNum;
            MisakiEQ::Setting::List cList;
            MisakiEQ::Setting::Path cPath;
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
            } config;

            enum SettingVar
            {
                ForecastSoundPath,
                ForecastSoundPerSerial,
                WarnSoundPath,
                WarnSoundPerSerial,
                OnlyListEvent,
                LCDoffTimer,
                LCDoffTimerBattery,
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
        };

    }

}