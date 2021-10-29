#include<M5Stack.h>
namespace App{
    namespace Info{
        class InfoData{
            public:
            void Begin();
            void Loop();
            void ButtonType(int Type);
            void Exit();
            void Draw();
            bool IsDraw();
            bool IsToHome();
            private:
            bool tohome;
        };
    }
}