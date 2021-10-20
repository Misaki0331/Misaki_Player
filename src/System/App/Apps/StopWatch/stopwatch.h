namespace App{
    namespace StopWatch{
        class stopwatch{
            public:
            stopwatch();
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
            bool tohome;
            int time;
            bool IsStart;
            int temp;
            char tempText[30];
            char tempT[10];
        };
    }
}