#include<ArduinoJson.h>
#include<M5Stack.h>
#include<HTTPClient.h>
#include<WiFi.h>
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
            String GetAppName();
            
            int LoadTime();
            private:
            static void GetNetworkFile(void *args);
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
            static String LatestHttpError;
            static HTTPClient* http;
        };
    }
}