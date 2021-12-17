#include <M5Stack.h>
#include "sound/sound.h"
#include "App/sellect.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "utility/MPU9250.h"
#include "utility/quaternionFilters.h"
namespace Core{
    class Main{
        public: 
        Main();
        static void Begin();            //初期化
        static void Loop();            //ループ
        static void ControlThread(void* arg);   //コントロール用スレッド
        static void SoundThread(void* arg);
        static void HTTPInit();
        static int MainLPS;
        static int BatteryPercent;
        static int FreeHeapMemory;
        static bool WiFiError;
        static bool DisableUI;
        private:
        static uint16_t bitFlip16(uint16_t x);
        static void Draw();
        static bool FirstWiFiConnect();
        static class DrawTaskUI drawUI;
        static class Sound::WavePlayer wavePlayer;
        static int TempMs;
        static int UpdateUI;
        static int DrawUpdate;
        static int LatestConnection;
        static String DrawTemp;
        static Core::Apps::System::Select appSelecter;
        static int ButtonACount;
        static int ButtonBCount;
        static int ButtonCCount;
        const static String index_html;
        const static String index_html_sc;
        const static String AccelGraph;
        const static uint8_t BMP_Header[];
        static AsyncWebServer *server;
        static int ScreenshotRequest;

        static int TempMillis;
        static void AddClock(int t);
        static void GetClock();
        static bool GotTime;

        static MPU9250 IMU;
    };
    
    
    
}
