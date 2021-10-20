#include<M5Stack.h>
namespace App{
    namespace FileManager{
        class FileManager{
            public:
            FileManager();
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
            typedef struct filedata{
                String Name="";
                int Size=-1;
                bool IsDirectry=false;
                bool IsCommandHome=false;
            }FileData;
            FileData* FileLists;
            bool IsSDFailed;
            int scrollOffset;
            int sellectOffset;
            String Path;
            bool IsFirstDraw;
            bool UpdateDraw;
            int FileCount;
            bool IsUpdate;
            int mode=0;
            enum cModeName{
                List=0,
                AudioPlayer=1,
                TextViewer=2,
                BinViewer=3
            };
            void GetFileList();
            void FileEnter();
            void ModeChange(int val);
            int WavUpdate;
            int DisplayWaveBarPos;
            String DisplayWavePosString;
        };
    }
}