
#include<M5Stack.h>
#include"AppList.h"

#define MenuBGColor 0xAD55
#define UIBGColor BLUE

namespace Core{
    namespace Apps{
        namespace System{
            class Select{
                private:
                    App::List applist;


                    void DrawUI();
                    
                    int SelectingAppID;
                    int PlayingAppID;
                    unsigned int ButtonStatus;
                    void PressButtonA();
                    void PressButtonB();
                    void PressButtonC();
                    void HoldButtonA();
                    void HoldButtonB();
                    void HoldButtonC();
                    bool IsDrawUpdate;
                    int sellectAppID;
                    bool IsFirstDrawFlg;
                    void InSellectDraw();
                    int RunAppValue=-1;
                    void SellectInit();
                public:
                    void Begin();
                    void Loop();
                    void Draw();
                    void SetButtonStatus(unsigned int value);
                    void Update();
                    int drawDelay;
                    
                    

            };
        }
    }
}