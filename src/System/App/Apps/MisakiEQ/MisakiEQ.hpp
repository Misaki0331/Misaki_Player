#include<ArduinoJson.h>
#include<M5Stack.h>
#include<HTTPClient.h>
#include<WiFi.h>
#define PingData 300
namespace App{
    namespace MisakiEQ{
        namespace Setting{
            class Path{
                public:
                void SetTitle(String t,String t2){
                    title=t;
                    subTitle=t2;
                }
                void Begin(String* path);
                void Draw();
                void Button(int type);
                void Cancel();
                void Release();
                bool GetIsUpdate();
                bool GetIsSetting();
                private:
                bool IsSetting;
                bool IsUpdate;
                String title;
                String subTitle;
                bool IsFirstDraw;
                String* ptr;

            };
            class List{
                public:
                void SetTitle(String t,String t2){
                    title=t;
                    subTitle=t2;
                }
                void Begin(String* name,const String* args, int argcount);
                void Draw();
                void Button(int type);
                void Release();
                void Cancel();
                bool GetIsUpdate();
                bool GetIsSetting();
                private:
                bool IsSetting;
                bool IsUpdate;
                String title;
                String subTitle;
                bool IsFirstDraw;
                String* ptr;
                const String* listArgs;
                int totalArgs;

                int scroll;
                int sellect;

            };
            class Num{
                public:
                void SetTitle(String t,String t2){
                    title=t;
                    subTitle=t2;
                }
                void Begin(int* value,uint8_t degit);
                void Draw();
                void Button(int type);
                void Release();
                void Cancel();
                bool GetIsUpdate();
                bool GetIsSetting();
                private:
                bool IsSetting;
                bool IsUpdate;
                String title;
                String subTitle;
                bool IsFirstDraw;
                int* ptr;
                int temp;
                uint8_t totalDegit;
                int sellectDegit;
                bool IsDegitMode;
                long powi(int x,int y);//X^Y

            };
        }
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
            int pg;
            int regPos;
            int mode;
            int sellectMode;
            static short *PingValue;
            static bool IsPingOpen;
            static String LatestHttpError;
            static HTTPClient* http;
            enum ModeType{
                ExitMode=-1,
                EEWMode=0,
                PingMode=1,
                SettingMode=2,
                SettingNum=-999,
                SettingList=-998,
                SettingPath=-997
            };
            int settingSellect;
            bool IsNotCursorMode;
            bool IsSettingUIUpdate;
            void SettingEnter();
            MisakiEQ::Setting::Num cNum;
            MisakiEQ::Setting::List cList;
            struct Configs{
                int value;
                String str;
            } config;
            static const String PrefList[];
        };
        
        
        
    }
}