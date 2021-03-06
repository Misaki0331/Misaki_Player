#include<M5Stack.h>
#include"../../../ui/Setting.h"
#include "../../../config.h"
using namespace Core::Setting;
#define WIFI_SSID_CHAR 26
#define WIFI_PASSWORD_CHAR 26
namespace App{
    namespace Wi_Fi{
        class Connect{
            public:
            Connect();
            void Begin();
            void Exit();
            void ButtonHold(int Type,bool data);
            void ButtonPress(int Type);
            void Loop();
            void Draw();
            void Background();
            bool GetBackGround();
            void SetBackGround(bool run);
            bool GetUpDateDraw();
            bool GetToActive();
            bool GetGoToHome();
            String GetAppName();
            private:
            bool toHome;
            String ssid;
            String password;
            void SaveProfile(uint8_t profile=Core::SystemAPI::WiFiCurrentProfile);
            void ReadProfile(uint8_t profile=Core::SystemAPI::WiFiCurrentProfile);
            void DelProfile(uint8_t profile);
            void ModeEnter();
            int mode=0;
            int sellectMode=0;
            bool IsFirstDraw;
            bool IsDraw;
            int startTimer=0;
            bool IsFileExists;
            enum Function{
                Menu=-1,
                WiFi_Detail=0,
                SettingProfileID=1,
                DeleteProfile=2,
                WiFi_Test=3,
                WiFi_EasySetting=4,
                WiFi_SetSSID=5,
                WiFi_SetPassword=6,
                WiFi_Return=7,
                SettingList=-997,
                SettingNum=-998,
                SettingTextBox=-999
            };
            String GetMacAddress();
            String* AccessPoints;
            int CurrentProfileID;
            int scroll;
            int testmode=0;
            int tempmode=-1;
            int connectingTime=0;
            bool isEasySetting;
            struct DateTime {
                int  year;
                byte month;
                byte day;
                byte hour;
                byte min;
                byte sec;
            };
            DateTime UnixToDateTime(long value);
            TextBox textbox;
            Num numbox;
            List listbox;
            
        };
    }
}