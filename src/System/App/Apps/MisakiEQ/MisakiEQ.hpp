#include<ArduinoJson.h>
#include<M5Stack.h>
#include<HTTPClient.h>
#include<WiFi.h>
#define PingData 300
namespace App{
    namespace MisakiEQ{
        class EEW{
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
            bool IsFirstBegin;
            bool IsUpdated;
            bool IsFirstDrawed;
            bool toHome;
            bool IsGetChanged;
            int TempTime;
            int JsonState;
            int JsonOldState;
            int regionUpdate;
            bool IsRegionUpdate;
            bool WarnRegionDisplay;
            int pg;
            int regPos;
            int mode;
            int sellectMode;
            static short *PingValue;
            static bool IsPingOpen;
            static String LatestHttpError;
            static HTTPClient* http;
            enum ModeType{
                EEWMode=0,
                PingMode=1,
                SettingMode=2
            };
        };
    }
}