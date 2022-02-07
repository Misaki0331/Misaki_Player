#include<M5Stack.h>


#define SD_WAV  "/sample.wav" // SDCard上のWaveFile定義．44.1kHz 16bit Stereo専用
#define SRC_FS  48000         // Source Sampling Rate [Hz]
#define OSR     2             // Over Sampling Rate (1~8) 
#define DAC_FS  (OSR *SRC_FS) // DAC Sampling Rate [Hz]
#define SPF     300           // Sample per Frame (Buffer処理単位)
#define DSOMAX  6              // Max. of DeltaSigma Order
#define SIN_FRQ 1000.0        // 1kHz
#define SINTBLMAX (SRC_FS /4) // 44100/100*25 = 11025 (.25s分。半減単位)
#define DECAY_MAX 16          // Sin振幅を半減させる回数 (16*.25s = 4sec) 
#define MODE2 5
#define MODE1 2
#define Init -1
#define Zero 0
#define Sin 1
#define Decay 2
#define  Wav 5
#define InitVolume 50

namespace Core{
    namespace Sound
    {
        class WavePlayer{
            private:
            static char src_buf[4*SPF];
            static char dac_buf[4 * SPF * OSR];
            static struct LPF lpf; 
            static struct FC fc[3];
            static void i2s_Init();
            static void i2s_End();
            static File wav;
            static size_t fill_data(int type, float dB);
            static void Filter_Process(size_t r_size);
            static String Filename;
            static bool IsPlaying;
            static int StopCount;
            static bool IsLooping;
            static int Volume;
            static bool isSpeak;
            static int srate;
            static bool isEnd;
            static int debug;
            static void obtainMemory();
            static void releaseMemory();
            static int Pos;
            static bool IsIOError;
            public:
            static void Begin();
            static void Loop();
            static int Play();
            static int Play(String str);
            static int Pause();
            static int Pause(bool flg);
            static void Stop();
            static void SetFileName(String str);
            static String GetFileName();
            static int SetSeek(int ms);
            static int GetSeek();
            static int GetLength();
            static bool GetIsPlaying();
            static int GetVolume();
            static int SetVolume(int value);
        };
        struct LPF{                  ////// Biquad Filter Coef.
                const float k, a1, a2, b1, b2; // b0 is fixed to '1', b1 & b2 MUST BE normalized
            } ;
        struct FC {
            const float gain, offset;
        };
    } // namespace name
    
}