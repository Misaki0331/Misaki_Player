#include"AppList.h"
#define APPCOUNT 1
//開始関数
void App::List::Begin(int AppID){
    switch(AppID){
        case 0:
        break;
    }
}

//終了関数
void App::List::Exit(int AppID){
    switch(AppID){
        case 0:
        break;
    }
}

//入力関数
void App::List::ButtonHold(int AppID,int Type, bool data){
    switch(AppID){
        case 0:
        break;
    }
}
//入力関数
void App::List::ButtonPress(int AppID,int Type){
    switch(AppID){
        case 0:
        break;
    }
}

//処理関数
void App::List::Loop(int AppID){
    switch(AppID){
        case 0:
        break;
    }
}

//描画関数
void App::List::Draw(int AppID){
    switch(AppID){
        case 0:
        break;
    }
}

//バックグラウンド処理
void App::List::BackGround(int AppID){
    if(GetRunBackGround(AppID)){
        switch(AppID){
        case 0:
        break;
    }
    }
}

//バックグラウンド動作フラグの取得
bool App::List::GetRunBackGround(int AppID){
    switch(AppID){
        case 0:
        break;
    }
    return 0;
    
}

//バックグラウンド動作フラグの設定
void App::List::SetRunBackGround(int AppID, bool Run){
    switch(AppID){
        case 0:
        break;
    }
    
}

//描画の更新
bool App::List::GetUpdateDraw(int AppID){
    switch(AppID){
        case 0:
        break;
    }
    return 0;
}

bool App::List::GetToActive(int AppID){
    switch(AppID){
        case 0:
        break;
    }
    return 0;
}

char* App::List::GetAppName(int AppID){
    switch(AppID){
        case 0:
        break;
        default:
        return "Unknown";
    }
}

int App::List::GetAppCount(){
    return APPCOUNT;
}