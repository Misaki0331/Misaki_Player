#include <M5Stack.h>
#include "sound/sound.h"
#include "App/sellect.h"
namespace Core{
    class Main{
        public: 
        Main();
        static void Begin();            //初期化
        static void Loop();            //ループ
        static void ControlThread(void* arg);   //コントロール用スレッド
        static void SoundThread(void* arg);
        static int MainLPS;
        static int BatteryPercent;
        static int FreeHeapMemory;
        
        static bool DisableUI;
        private:
        static void Draw();
        static class DrawTaskUI drawUI;
        static class Sound::WavePlayer wavePlayer;
        static int TempMs;
        static int UpdateUI;
        static int DrawUpdate;
        static String DrawTemp;
        static Core::Apps::System::Select appSelecter;
        static int ButtonACount;
        static int ButtonBCount;
        static int ButtonCCount;
    };
    
    
    
}
