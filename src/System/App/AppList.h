#include<M5Stack.h>
#include"Apps/StopWatch/stopwatch.h"
#include"Apps/Sleep/Sleep.h"
#include"Apps/FileManaager/FileManager.h"
#include"Apps/Wi-Fi/connect.h"
#define APPCOUNT 4
namespace App{
    class List{
        public:
        void Begin(int AppID);      //初期起動関数
        void Exit(int AppID);       //切り替えされる時に実行される関数
        void ButtonHold(int AppID,int Type, bool data);//ボタンタスク
        void ButtonPress(int AppID,int Type);//ボタンタスク
        void Loop(int AppID);       //処理関数
        void Draw(int AppID);       //描画関数
        void BackGround(int AppID);//バックグラウンドタスク
        bool GetRunBackGround(int AppID);//バックグラウンド情報
        void SetRunBackGround(int AppID,bool Run);//バックグラウンド設定
        bool GetUpdateDraw(int AppID);//図形の更新
        bool GetToActive(int AppID);//他のアプリからこのアプリに移動するフラグ
        bool GetGoToHome(int AppID);
        String GetAppName(int AppID);
        int GetAppCount();//合計アプリ数
        private:
        App::StopWatch::stopwatch list01;
        App::Sleep::sleep SleepLCD;
        App::FileManager::FileManager fileManager;
        App::Wi_Fi::Connect wifiConnect;
    };
}