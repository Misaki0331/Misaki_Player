#include"sound.h"
#include "driver/i2s.h"
using namespace Core::Sound;
char WavePlayer::src_buf[]={0};
char WavePlayer::dac_buf[]={0};
struct LPF WavePlayer::lpf = {                   //// OSR Val.
#if   OSR >= 8                // fc = 0.05fs
  .k = +0.0208307252, .a1 = -1.552247273, .a2 = +0.635570174,
#elif OSR >= 6                // fc = 0.067fs
  .k = +0.0347860374, .a1 = -1.407505344, .a2 = +0.546649494,
#elif OSR >= 4                // fc = 0.1fs
  .k = +0.0697808942, .a1 = -1.126428906, .a2 = +0.405552483,
#elif OSR >= 2                // fc = 0.2fs
  .k = +0.2132071969, .a1 = -0.339151185, .a2 = +0.191979973,
#else // OSR < 2              // fc = 0.4fs (18kHz at 44k1, 20k at 48k)
  .k = +0.6632098902, .a1 = +1.209579277, .a2 = +0.443260284,
#endif
  .b1 = +2, .b2 = +1,         // Fixed (b0,b1,b2)=(+1,+2,+1)
};
struct FC WavePlayer::fc[]={
    { +1.000000000,   +128.00000 }, // Thru     1/1  +128
    { +0.333333333, +21973.33333 }, // Mid 1/3  1/3  +128       (1-1/3)*(2^15)
    { +0.333333333,   +213.33333 }, // Low 1/3  1/3  +128   
};
bool WavePlayer::isSpeak=false;
void WavePlayer::i2s_Init(){
  if(!isSpeak){
     i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_DAC_BUILT_IN | I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = OSR*srate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // 内部DACは上位8bitが再生対象
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // ステレオ。左右データ書込必要
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0,
    .dma_buf_count = 16,      // 現物合わせ
    .dma_buf_len = SPF,       // 1024以下。今回はサンプルフレーム幅に合わせた
    .use_apll = false         // 75kHz以上(OSR>=2)でtrue指定すると正常動作しない
  };
  i2s_driver_install( I2S_NUM_0, &i2s_config, 0, NULL );
  i2s_set_pin( I2S_NUM_0, NULL );
  isSpeak=true;
}
}
void WavePlayer::i2s_End(){
  if(isSpeak){
    isSpeak=false;
    delay(20);
  i2s_driver_uninstall(I2S_NUM_0);
  
  }
}
int WavePlayer::Pos=0;
bool WavePlayer::IsIOError=0;
size_t WavePlayer::fill_data(int type, float dB){
    static int32_t ct = 0, ct2 = 0;

  if ( type == Init ) {
    
  
    return 0;
  }
  
  int32_t *pto32 = (int32_t *)src_buf;
  dB = constrain( dB, -96, 0);                  // Clip to -96~0
  float f;
  float g  = 32767.0 * pow(10, dB / 20);        // for flat gain
  float gd = g * pow(0.5, (float)ct2);          // for decay gain
  int sz=sizeof(src_buf);
  size_t r_size = sz;
    
  if ( type == Wav ) {
    r_size = wav.readBytes(src_buf, sz);
    if(wav.size()-wav.position()>sizeof(src_buf)){
    if ( r_size != sz ) {
        IsIOError=1;
      
      wav.close();
      wav = SD.open(Filename,FILE_READ);
      wav.seek(Pos);
      while(IsPlaying&&wav.size()<=0){
        wav.close();
      wav = SD.open(Filename,FILE_READ);
      wav.seek(Pos);
      }
    }else{
      IsIOError=0;
      Pos=(int)wav.position();
    }
    }else{
      if ( r_size != sz ) {
      if(IsLooping){
            wav.seek(0x2C); // rewind to start point
        }else{
            wav.seek(0x2C);
            IsPlaying=false;
            i2s_End();
        }
      }
      Pos=(int)wav.position();
      IsIOError=0;
    }
  } else {
    
    for (int i = 0; i < (sz >> 2); i++) {
      switch ( type ) {
        case Zero :
        default   : f = 0;                      break;
      }
      if (++ct >= srate/4) {
        ct = 0;
        if (++ct2 >= DECAY_MAX) ct2 = 0;
        gd = g * pow(0.5, (float)ct2);          // update decay gain
      }
      int32_t id = (int32_t)(f + 0.5);          // f32->i32 & Round Up
      id = constrain( id, -32768, +32767);      // Clip to i16 Range
      *(pto32++) = (id << 16) | (id & 0xffff);  // L&R Dual Mono Data
    }
  }
  return r_size;
}
void WavePlayer::Filter_Process(size_t r_size){
    static float    z[3] = { 32768.0 };             // Biquad IIR LPF z work
  static int32_t  iqtout = 0;                     // int Quantizer Output
  static int32_t  isigma[DSOMAX] = {0};           // int Sigma[Order] Data;
  uint32_t *pti32 = (uint32_t *)src_buf;          // 演算と高速化都合でi16 x2をu32で読出
  uint32_t *pto32 = (uint32_t *)dac_buf;          // 高速化都合でu16 x2をu32で書込
  int dso = MODE2;                        // DeltaSigma Order(次数)取得

  for (int i = 0; i < (r_size >> 2); i++) { //////// Word Loop
    uint32_t ud = *(pti32++);                     // i16 stereo をu32で読出
    ud ^= 0x80008000;                             // i16 stereo-> u16 stereo 変換
    ud = ((ud & 0xffff) + (ud >> 16)) >> 1;       // Stereo->Mono (u16L+u16R)/2
    float fd = (float)ud, fin;                    // 後工程用float変換
    // 過増幅歪/DACノイズ対策処理
    fd    = (fc[MODE1].gain * fd             // Mul. Gain
            + fc[MODE1].offset)*((float)Volume/100);               // Add. Offset

    for (int j = 0; j < OSR; j ++) {          ////// xOSR Over Sampling Loop
      z[2] = z[1];                                // Biquad z2 Shift
      z[1] = z[0];                                // Biquad z1 Shift
      z[0] = lpf.k * fd - lpf.a1 * z[1] - lpf.a2 * z[2]; // Biquad z0 Update
      fin  = z[0]      + lpf.b1 * z[1] + lpf.b2 * z[2]; // Biquad result

      int32_t iin = (int32_t)(fin + 0.5);         // float -> i32 data
      for (int k = 0; k < dso; k++ ) {          //// xOrder DeltaSigma Loop(dso=0:Thru)
        isigma[k] += (iin - iqtout);              // Delta-Sigma Core
        iin = isigma[k];                          // Update Output (iin)
      }                                         //// Delta-Sigma Loop End
      iqtout = iin & 0xffffff00;                  // Quantize(8bDACで無視される下位8bitをMask)

      ud = (uint32_t)constrain(iqtout, 0, 65535); // Clip to u16 size
      ud |= (ud << 16);                           // u16 Mono → Dual u16 Mono
      *(pto32++) = ud;                            // DAC出力バッファに書込
    }                                         ////// OverSampling Loop End
  }                                         //////// Word Loop End

}
void WavePlayer::Begin(){
  M5.Speaker.begin();
    fill_data(Init,0);
    
}
String WavePlayer::Filename="";
bool WavePlayer::isEnd=0;
int WavePlayer::debug=0;
int WavePlayer::srate=0;
void WavePlayer::Loop(){
  if(srate>0){
    int sr=srate;
    isEnd=0;
    size_t r_size = 0;
    debug=163;
    if(isSpeak){
    for ( int i = 0; i < (sr / SPF); i++ ) {
        if(IsPlaying){
          debug=166;
            r_size = fill_data(Wav,     0); 
        }   else{
          debug=169;
            if (StopCount < 10) {r_size = fill_data(Zero,    0); // ゼロ出力
            }else{
              debug=171;
            }
        }
        if (IsPlaying)StopCount = 0;
        if (StopCount < 10) {
        StopCount++;
        if(isSpeak){
          debug=180;
        Filter_Process( r_size );
        debug=182;
        i2s_write_bytes( I2S_NUM_0, (char *)dac_buf, r_size * OSR, portMAX_DELAY );
        debug=184;
      }
        }
    }
    }else{
      vTaskDelay(10);
    }
  }else{
    isEnd=1;
    vTaskDelay(10);
  }
}
int WavePlayer::Volume=InitVolume;
bool WavePlayer::IsPlaying=false;
int WavePlayer::StopCount=0;
void WavePlayer::releaseMemory(){
  srate=0;
  
}
void WavePlayer::obtainMemory(){
  wav.seek(0x18);
  releaseMemory();
    byte* smrt=new byte[5];
    wav.read(smrt,4);
    int samplingrate=0;
    for(int c=3;c>=0;c--){
      samplingrate*=256;
      samplingrate+=(unsigned char)smrt[c];
    }
    delete[] smrt;
    srate=samplingrate;
}
int WavePlayer::Play(){
    if(!SD.exists(Filename))return 0;
    //wav = SD.open(Filename);
    wav.close();
    wav=SD.open(Filename,FILE_READ);
    obtainMemory();
    i2s_Init();
    wav.seek(0x2C);  // Skip wav header
    M5.Speaker.mute();
    IsPlaying=true;
    return 1;
}
int WavePlayer::Play(String str){
    if(!SD.exists(str))return 0;
    if(IsPlaying){
        IsPlaying=false;
    }
    
    Filename=str;
    if(!wav)wav.close();
    wav = SD.open(Filename);
    obtainMemory();
i2s_Init();
    wav.seek(0x2C);  // Skip wav header
    IsPlaying=true;
    M5.Speaker.setVolume(Volume);
    return 1;
}
int WavePlayer::Pause(){
  if(!SD.exists(Filename))return 0;
    IsPlaying=!IsPlaying;
    if(IsPlaying){
        i2s_Init();
      }else{
        i2s_End();
    }
    return 1;
}
int WavePlayer::Pause(bool flg){
  if(!SD.exists(Filename))return 0;
    IsPlaying=flg;
    if(IsPlaying){
        M5.Speaker.setVolume(Volume);
        i2s_Init();
      }else{
        i2s_End();
        M5.Speaker.mute();
    }
    return 1;
}
void WavePlayer::Stop(){
    IsPlaying=false;
    wav.seek(0x2C);
    wav.close();
    M5.Speaker.mute();
    i2s_End();
    releaseMemory();
}
void WavePlayer::SetFileName(String str){
    
    
    IsPlaying=false;
    if(str!=Filename){
      Stop();
    }
    Filename=str;
    if(!wav)wav.close();
    wav = SD.open(Filename);
    obtainMemory();
    wav.seek(0x2C);  // Skip wav header
    M5.Speaker.mute();
    Pos=0;
}
String WavePlayer::GetFileName(){
    return Filename;
}
int WavePlayer::SetSeek(int ms){

    unsigned long MaxTime=(0xFFFFFFFF-0x2C)/((int)srate/1000)/4;
    if(!wav.available())return -2;
    if(ms>MaxTime)return -1;
    if(((wav.size()-0x2C)/((int)srate/1000)/4)<ms)return 0;
    unsigned long SeekByte = ms*((int)srate/1000)*4;
    wav.seek(0x2C+SeekByte);
    return 1;
}
int WavePlayer::GetSeek(){
    if(!wav.available())return -2;
    return ((Pos-0x2C)/((int)srate/1000)/4);
}
int WavePlayer::GetLength(){
    if(!wav.available())return -2;
    return (wav.size()-0x2C)/((int)srate/1000)/4;
}
File WavePlayer::wav;
bool WavePlayer::IsLooping=false;
bool WavePlayer::GetIsPlaying(){
  return IsPlaying;
}
int WavePlayer::GetVolume(){
  return Volume;
}
int WavePlayer::SetVolume(int value){
  if(!(value>=0&&value<=100))return -1;
  Volume=value;
  if(IsPlaying)M5.Speaker.setVolume(Volume);
  return 0;
}