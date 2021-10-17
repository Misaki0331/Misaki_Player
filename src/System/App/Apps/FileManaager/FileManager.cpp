#include"FileManager.h"
#include"../../../Fonts/FastFont.h"
#include"../../../config.h"
#include"../../../sound/sound.h"

App::FileManager::FileManager::FileManager(){

}
void App::FileManager::FileManager::Begin(){
    tohome=false;
    FileCount=0;
    IsFirstDraw=false;
    Path="/";
    GetFileList();
    
}
/*bool App::FileManager::FileManager::GetDirectry(File* dir,String PathName){
    if(PathName.charAt(0)!='/')return false;
    dir=SD.open("/");

    String Path2=PathName;
    Path2.remove(0,1);


}*/
void App::FileManager::FileManager::GetFileList(){
    delete[] FileLists;
    String PathStr=Path;
    if(PathStr.length()>1)PathStr.remove(PathStr.length()-1,1);
    File folder=SD.open(PathStr);
    int count=1;
    FileCount=1;
    File entry;
    while(1){
        entry =  folder.openNextFile();
        if (! entry) {
            break; //以降ファイルなし
        }
        FileCount++;
    }
    
    folder=SD.open(PathStr.c_str());
    FileLists=new FileData[FileCount];
    if(Path!="/"){
        FileLists[0].Name="../";
        FileLists[0].IsDirectry=true;
        FileLists[0].Size=-1;
    }else{
        FileLists[0].Name="->Home";
        FileLists[0].IsDirectry=true;
        FileLists[0].Size=-1;
        FileLists[0].IsCommandHome=true;
    }
    while(1){
        entry =  folder.openNextFile();
        if (! entry) {
            break; //以降ファイルなし
        }
        String PathText=entry.name();
        PathText=PathText.substring(Path.length(),PathText.length());
        FileLists[count].Name=PathText;
        if(entry.isDirectory()){
            FileLists[count].Name.concat("/");
            FileLists[count].IsDirectry=true;
            FileLists[count].Size=-1;
        }else{
            
            FileLists[count].IsDirectry=false;
            FileLists[count].Size=entry.size();
        }
        count++;
    }
    FileCount=count;
    int start=1;
    FileData Temp;
    for(int i=start+1;i<FileCount;i++){
        if(FileLists[i-1].IsDirectry==false
        && FileLists[i  ].IsDirectry==true){
            Temp=FileLists[i];
            int j=i;
            for(;FileLists[j-1].IsDirectry==false&&j>start;j--){
                FileLists[j]=FileLists[j-1];
            }
            FileLists[j]=Temp;
        }
    }
}
void App::FileManager::FileManager::Exit(){
    if(FileLists!=nullptr){
        delete[] FileLists;
        FileLists=nullptr;
    }
}
void App::FileManager::FileManager::ButtonHold(int Type,bool data){

}
void App::FileManager::FileManager::FileEnter(){
    if(FileLists[sellectOffset].IsCommandHome){
        tohome=true;
    }else if(FileLists[sellectOffset].Name=="../"){
        Path.remove(Path.length()-1,1);
        while(Path.charAt(Path.length()-1)!='/')Path.remove(Path.length()-1,1);
        GetFileList();
    }else if(FileLists[sellectOffset].IsDirectry){
        Path.concat(FileLists[sellectOffset].Name);
        GetFileList();
    }else{
    }   
    IsFirstDraw=false;
    sellectOffset=0;
}
void App::FileManager::FileManager::ButtonPress(int Type){
    switch(Type){
        case 1:
            if(sellectOffset>0)sellectOffset--;
            IsUpdate=true;
            UpdateDraw=true;
        break;
        case 2:
            FileEnter();
        break;
        case 3:
            if(sellectOffset<FileCount-1)sellectOffset++;
            IsUpdate=true;
            UpdateDraw=true;
        break;
    }
}
void App::FileManager::FileManager::Loop(){
    if(!IsFirstDraw){
        UpdateDraw=true;
        
    }
    if(IsUpdate){
        UpdateDraw=true;
    }
}
void App::FileManager::FileManager::Draw(){
    if(!IsFirstDraw){
        M5.Lcd.fillRect(0,8,29*6,232,BLACK);
        IsFirstDraw=true;
        IsUpdate=true;
        Core::Draw::FastFont::printRom("Path:",0,16,WHITE);
        Core::Draw::FastFont::printRom(Path,30,16,WHITE);
        Core::Draw::FastFont::printRom("FileName",0,24,WHITE);
        Core::Draw::FastFont::printRom("Size",78,24,WHITE);
        
    }
    if(IsUpdate){
        IsUpdate=false;
       
        if(!IsSDFailed){
            for(int list=0;list<FileCount;list++){
                char text[20];
                if(FileLists[list].Size!=-1)
                sprintf(text,"%10d Bytes",FileLists[list].Size);
                if(list==sellectOffset){
                    M5.Lcd.fillRect(0,32+8*list,29*6,8,WHITE);
                    Core::Draw::FastFont::printRom(FileLists[list].Name.substring(0,12).c_str(),0,32+8*list,BLACK,1,WHITE);
                    if(!FileLists[list].IsDirectry)Core::Draw::FastFont::printRom(text,78,32+8*list,BLACK,1,WHITE);
                }else{
                    M5.Lcd.fillRect(0,32+8*list,29*6,8,BLACK);
                    Core::Draw::FastFont::printRom(FileLists[list].Name.substring(0,12).c_str(),0,32+8*list,WHITE,1,BLACK);
                    if(!FileLists[list].IsDirectry)Core::Draw::FastFont::printRom(text,78,32+8*list,WHITE,1,BLACK);
                }

                
            }
        }else{
            Core::Draw::FastFont::printRom("Exception Error",0,24,RED);
        }
    }
}
void App::FileManager::FileManager::Background(){

}
bool App::FileManager::FileManager::GetBackGround(){return 0;}
void App::FileManager::FileManager::SetBackGround(bool run){return;}
bool App::FileManager::FileManager::GetUpDateDraw(){return UpdateDraw;}
bool App::FileManager::FileManager::GetToActive(){return 0;}
char* App::FileManager::FileManager::GetAppName(){return "File Manager";}
bool App::FileManager::FileManager::GetGoToHome(){
    return tohome;
}