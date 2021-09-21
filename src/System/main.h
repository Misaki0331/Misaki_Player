#include <M5Stack.h>
#include "sound/sound.h"
namespace Core{
    class Main{
        public: 
        Main();
        void Begin();            //初期化
        void Loop();            //ループ
        static void ControlThread(void* arg);   //コントロール用スレッド
        static void SoundThread(void* arg);
        static int MainLPS;
        static int BatteryPercent;
        static bool DisableUI;
        private:
        static struct SystemData systemData;
        static struct SystemConfig systemConfig;
        static class DrawTaskUI drawUI;
        static class Sound::WavePlayer wavePlayer;
        static int TempMs;
        static int UpdateUI;
    };
    
    struct SystemData
    {
        public:
        int LoopCount;
        bool UpdateBatteryUI;
        int TempBatteryPercent;
    };
    struct SystemConfig
    {
        public:
        int BatteryPosX;
        int BatteryPosY;
        bool EnableALLUpdate;
    };

    
}
