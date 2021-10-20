#include<M5Stack.h>
#define WIFI_SSID_CHAR 32
#define WIFI_PASSWORD_CHAR 32
#define WIFI_OFFSET 64
namespace App{
    namespace WiFi{
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
            char* GetAppName();
            private:
            bool toHome;
            String ssid;
            String password;
            void SaveProfile();
            void ReadProfile();
            int mode=0;
            int sellectMode=0;
            bool IsFirstDraw;
            bool IsChangedMenu;
        };
    }
}