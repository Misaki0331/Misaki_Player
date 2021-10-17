namespace App{
    namespace Sleep{
        class sleep{
            public:
            sleep();
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
            bool tohome;
            
        };
    }
}