#include"AppList.h"

//開始関数
void App::List::Begin(int AppID){
    switch(AppID){
        case 0:
        list01.Begin();
        break;
        case 1:
        SleepLCD.Begin();
        break;
        case 2:
        fileManager.Begin();
        break;
        case 3:
        wifiConnect.Begin();
        break;
    }
}

//終了関数
void App::List::Exit(int AppID){
    switch(AppID){
        case 0:
        list01.Exit();
        break;
        case 1:
        SleepLCD.Exit();
        break;
        case 2:
        fileManager.Exit();
        break;
        case 3:
        wifiConnect.Exit();
        break;
    }
}

//入力関数
void App::List::ButtonHold(int AppID,int Type, bool data){
    switch(AppID){
        case 0:
        list01.ButtonHold(Type,data);
        break;
    }
}
//入力関数
void App::List::ButtonPress(int AppID,int Type){
    switch(AppID){
        case 0:
        list01.ButtonPress(Type);
        break;
        case 1:
        SleepLCD.ButtonPress(Type);
        break;
        case 2:
        fileManager.ButtonPress(Type);
        break;
        case 3:
        wifiConnect.ButtonPress(Type);
    }
}

//処理関数
void App::List::Loop(int AppID){
    switch(AppID){
        case 0:
        list01.Loop();
        break;
        case 2:
        fileManager.Loop();
        break;
        case 3:
        wifiConnect.Loop();
    }
}

//描画関数
void App::List::Draw(int AppID){
    switch(AppID){
        case 0:
        list01.Draw();
        break;
        case 2:
        fileManager.Draw();
        break;
        case 3:
        wifiConnect.Draw();
        break;
    }
}

//バックグラウンド処理
void App::List::BackGround(int AppID){
    if(GetRunBackGround(AppID)){
        switch(AppID){
        case 0:
        list01.Background();
        break;
    }
    }
}

//バックグラウンド動作フラグの取得
bool App::List::GetRunBackGround(int AppID){
    switch(AppID){
        case 0:
        list01.GetBackGround();
        break;
    }
    return 0;
    
}

//バックグラウンド動作フラグの設定
void App::List::SetRunBackGround(int AppID, bool Run){
    switch(AppID){
        case 0:
        list01.SetBackGround(Run);
        break;
    }
    
}

//描画の更新
bool App::List::GetUpdateDraw(int AppID){
    switch(AppID){
        case 0:
        return list01.GetUpDateDraw();
        break;
        case 2:
        return fileManager.GetUpDateDraw();
        case 3:
        return wifiConnect.GetUpDateDraw();
    }
    return 0;
}

bool App::List::GetToActive(int AppID){
    switch(AppID){
        case 0:
        list01.GetToActive();
        break;
    }
    return 0;
}

String App::List::GetAppName(int AppID){
    switch(AppID){
        case 0:
        return list01.GetAppName();
        case 1:
        return SleepLCD.GetAppName();
        case 2:
        return fileManager.GetAppName();
        case 3:
        return wifiConnect.GetAppName();
        default:
        return "Unknown";
    }
}

bool App::List::GetGoToHome(int AppID){
    switch(AppID){
        case 0:
        return list01.GetGoToHome();
        case 1:
        return SleepLCD.GetGoToHome();
        case 2:
        return fileManager.GetGoToHome();
        case 3:
        return wifiConnect.GetGoToHome();
    }
    return 0;
}

int App::List::GetAppCount(){
    return APPCOUNT;
}