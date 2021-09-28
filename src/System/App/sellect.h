
#include<M5Stack.h>


#define MenuBGColor 0xAD55
#define UIBGColor BLUE

namespace Core{
    namespace App{
        namespace System{
            class Select{
                private:

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
                public:
                    void Begin();
                    void Loop();
                    void Draw();
                    void SetButtonStatus(unsigned int value);
                    void Update();
                    

            };
        }
    }
}