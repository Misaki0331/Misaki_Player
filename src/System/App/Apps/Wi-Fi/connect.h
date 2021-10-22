#include<M5Stack.h>
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
            void SaveProfile();
            void ReadProfile();
            void ModeEnter();
            int mode=0;
            int sellectMode=0;
            bool IsFirstDraw;
            bool IsDraw;
            int startTimer=0;
            enum Function{
                Menu=-1,
                WiFi_Detail=0,
                WiFi_Test=1,
                WiFi_SetSSID=2,
                WiFi_SetPassword=3,
                WiFi_Return=4
            };
            String GetMacAddress();
            bool IsLargeCharMode;
            char GetSoftKeyboardChar(int value);
            int sellecting_char;
            String TempSetString;
            void EnterCharacter(int val);
            void DrawKeyBoardUI();
            int scroll;
            int testmode=0;
        };
    }
}