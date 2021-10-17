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
            char* GetAppName();
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

            void GetFileList();
            void FileEnter();
        };
    }
}