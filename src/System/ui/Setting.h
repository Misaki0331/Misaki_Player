#include <M5Stack.h>
#include <HTTPClient.h>
#include <WiFi.h>
#ifndef SETTING_H
#define SETTING_H
namespace Core
{
    namespace Setting
    {
        class Path
        {
        public:
            void SetTitle(String t, String t2)
            {
                title = t;
                subTitle = t2;
            }
            void Begin(String *path, String ext);
            void Draw();
            void Button(int type);
            void Cancel();
            void Release();
            bool GetIsUpdate();
            bool GetIsSetting();

        private:
            bool IsSetting;
            bool IsUpdate;
            String title;
            String subTitle;
            bool IsFirstDraw;
            bool IsPathUpdate;
            String *ptr;
            String fileExt;
            int sellectFile;
            String openPath;
            void GetFileList();
            typedef struct filedata
            {
                String Name = "";
                int Size = -1;
                bool IsDirectry = false;
                bool IsCommandCancel = false;
                bool IsCommandEmpty = false;
                bool IsCommnadBack = false;
            } FileData;
            FileData *FileLists;
            int FileCount;
            int scroll;
            int sellect;
        };
        class List
        {
        public:
            void SetTitle(String t, String t2)
            {
                title = t;
                subTitle = t2;
            }
            void Begin(String *name, const String *args, int argcount);
            void Draw();
            void Button(int type);
            void Release();
            void Cancel();
            bool GetIsUpdate();
            bool GetIsSetting();

        private:
            bool IsSetting;
            bool IsUpdate;
            String title;
            String subTitle;
            bool IsFirstDraw;
            String *ptr;
            const String *listArgs;
            int totalArgs;

            int scroll;
            int sellect;
        };
        class Num
        {
        public:
            void SetTitle(String t, String t2)
            {
                title = t;
                subTitle = t2;
            }
            void Begin(int *value, uint8_t degit);
            void SetMin(int val);
            void SetMax(int val);
            void Draw();
            void Button(int type);
            void Release();
            void Cancel();
            bool GetIsUpdate();
            bool GetIsSetting();

        private:
            bool IsSetting;
            bool IsUpdate;
            String title;
            String subTitle;
            bool IsFirstDraw;
            int *ptr;
            int temp;
            uint8_t totalDegit;
            int sellectDegit;
            bool IsDegitMode;
            long powi(int x, int y); // X^Y
            int min;
            int max;
        };
        class TextBox{
            public:
            void SetTitle(String t, String t2)
            {
                title = t;
                subTitle = t2;
            }
            void Begin(String *value, uint8_t max);
            void Draw();
            void Button(int type);
            void Release();
            void Cancel();
            bool GetIsUpdate();
            bool GetIsSetting();
            private:
            bool IsSetting;
            bool IsTextUpdate;
            bool IsButtonUpdate;
            String title;
            String subTitle;
            bool IsFirstDraw;
            String *ptr;
            uint8_t maxcount;

            String tempStr;
            int sellectChar;
            bool IsLargeCharMode;
            void EnterCharacter(uint8_t val);
            uint8_t GetSoftKeyboardChar(uint8_t val);
        };
    }

}
#endif