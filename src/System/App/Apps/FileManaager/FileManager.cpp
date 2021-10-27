#include"FileManager.h"
#include"../../../Fonts/FastFont.h"
#include"../../../config.h"
#include"../../../sound/sound.h"
using namespace App::FileManager;
using namespace Core::Sound;
using namespace Core::Draw;
FileManager::FileManager(){

}
void FileManager::Begin(){
    tohome=false;
    FileCount=0;
    IsFirstDraw=false;
    Path="/";
    GetFileList();
    //Serial.begin(115200);
    
}
/*bool FileManager::GetDirectry(File* dir,String PathName){
    if(PathName.charAt(0)!='/')return false;
    dir=SD.open("/");

    String Path2=PathName;
    Path2.remove(0,1);


}*/
void FileManager::GetFileList(){
    delete[] FileLists; //メモリのお掃除
    String PathStr=Path;
    if(PathStr.length()>1)PathStr.remove(PathStr.length()-1,1);
    File folder=SD.open(PathStr);
    int count=1;
    FileCount=1;
    File entry;
    //まずは中身の数を計算
    while(1){
        entry =  folder.openNextFile();
        if (! entry) {
            break; //以降ファイルなし
        }
        FileCount++;
    }
    //SDカードのルートに置いていればHomeに戻るフォルダーを入れる。
    //でなければ上のフォルダー階層に行くフォルダーを入れる

    folder=SD.open(PathStr);
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
void FileManager::Exit(){
    if(FileLists!=nullptr){
        delete[] FileLists;
        FileLists=nullptr;
    }
    //Serial.end();
}
void FileManager::ButtonHold(int Type,bool data){

}
void FileManager::FileEnter(){
    if(FileLists[sellectOffset].IsCommandHome){
        tohome=true;
    }else if(FileLists[sellectOffset].Name=="../"){
        Path.remove(Path.length()-1,1);
        while(Path.charAt(Path.length()-1)!='/')Path.remove(Path.length()-1,1);
        GetFileList();
    }else if(FileLists[sellectOffset].IsDirectry){
        Path.concat(FileLists[sellectOffset].Name);
        GetFileList();
    }else if(FileLists[sellectOffset].Name.length()<=64&&FileLists[sellectOffset].Name.length()>=5){
        
        String ext;
        ext=FileLists[sellectOffset].Name;
        int i=0;
        for(i=ext.length()-2;i>0&&ext.charAt(i)!='.';i--);
        int c=0;
        char ex[5];
        for(;i<ext.length()&&c<4;){
            ex[c]=ext.charAt(i);
            c++;
            i++;
        }
        ex[c]='\0';
        ext=ex;
        ext.toLowerCase();
        if(ext==".wav"){
            String SoundPath;
            SoundPath=Path;
            SoundPath.concat(FileLists[sellectOffset].Name);
            if(WavePlayer::GetFileName()!=SoundPath||!WavePlayer::GetIsPlaying())WavePlayer::SetFileName(SoundPath);
            ModeChange(AudioPlayer);
            
        }
    }   
    IsFirstDraw=false;
    sellectOffset=0;
    scrollOffset=0;
}
void FileManager::ButtonPress(int Type){
    switch(mode){
        case List:
            switch(Type){
                case 1:
                    if(sellectOffset>0)sellectOffset--;
                    IsUpdate=true;
                    UpdateDraw=true;
                    if(sellectOffset-scrollOffset<0)scrollOffset--;
                break;
                case 2:
                    FileEnter();
                break;
                case 3:
                    if(sellectOffset<FileCount-1)sellectOffset++;
                    IsUpdate=true;
                    UpdateDraw=true;
                    if(sellectOffset-scrollOffset>=26)scrollOffset++;
                break;
            }
            break;
        case AudioPlayer:
            switch(Type){
                case 1:
                    WavePlayer::Stop();
                    ModeChange(List);
                break;
                case 2:
                    WavePlayer::Pause(!WavePlayer::GetIsPlaying());
                    break;
                    case 3:
                    IsFirstDraw=false;
                    UpdateDraw=true;
                    mode=List;
            }
    }
}

void FileManager::Loop(){
    if(!IsFirstDraw){
        UpdateDraw=true;
        
    }
    if(IsUpdate){
        UpdateDraw=true;
    }
    switch(mode){
        case AudioPlayer:
        
            IsUpdate=1;
            UpdateDraw=1;
    }
}
void FileManager::ModeChange(int val){
    if(mode!=val){
        if(mode==AudioPlayer){
            DisplayWaveBarPos=0;
            WavePlayer::Stop();
        }
        IsFirstDraw=false;
        UpdateDraw=true;
        mode=val;
        
    }
}
void FileManager::Draw(){
    if(!IsFirstDraw){
        M5.Lcd.fillRect(0,8,320,232,BLACK);
        IsFirstDraw=true;
        IsUpdate=true;
        switch(mode){
            case List:
                M5.Lcd.fillRect(0,16,320,232,BLACK);
                FastFont::printRom("Path:",0,16,WHITE);
                FastFont::printRom(Path,30,16,WHITE);
                FastFont::printRom("FileName",0,24,WHITE);
                FastFont::printRom("Size",224,24,WHITE);
                break;
            case AudioPlayer:
                FastFont::printRom("Music Player",10,10,WHITE,2,BLACK);
                FastFont::printRom("File:",10,30,WHITE,1,BLACK);
                FastFont::printRom(WavePlayer::GetFileName(),10,38,WHITE,2,BLACK);
                M5.Lcd.fillRect(9,180,302,20,BLACK);
                M5.Lcd.drawRect(9,180,302,20,WHITE);
                DisplayWavePosString="                     ";
                break;
            break;
        }
    }
    if(IsUpdate){
        IsUpdate=false;
        switch(mode){
            case List:
                if(!IsSDFailed){
                    for(int list=0;list+scrollOffset<FileCount&&list<26;list++){

                        char text[20];
                        if(FileLists[list+scrollOffset].Size!=-1)
                        sprintf(text,"%10d Bytes",FileLists[list].Size);
                        if(list+scrollOffset==sellectOffset){
                            M5.Lcd.fillRect(0,32+8*list,320,8,WHITE);
                            FastFont::printRom(FileLists[list+scrollOffset].Name.substring(0,36).c_str(),0,32+8*list,BLACK,1,WHITE);
                            if(!FileLists[list].IsDirectry)FastFont::printRom(text,224,32+8*list,BLACK,1,WHITE);
                        }else{
                            M5.Lcd.fillRect(0,32+8*list,320,8,BLACK);
                            FastFont::printRom(FileLists[list+scrollOffset].Name.substring(0,36).c_str(),0,32+8*list,WHITE,1,BLACK);
                            if(!FileLists[list].IsDirectry)FastFont::printRom(text,224,32+8*list,WHITE,1,BLACK);
                        }
                    }
                }else{
                    FastFont::printRom("Exception Error",0,24,RED);
                }
            break;
            case AudioPlayer:
                int time = WavePlayer::GetSeek();
                int len =  WavePlayer::GetLength();
                int Bar=(float)time*300/len;
                if(Bar<0)Bar=0;
                if(Bar>300)Bar=300;
                if(Bar<DisplayWaveBarPos){
                    M5.Lcd.fillRect(10+Bar,181,DisplayWaveBarPos-Bar,18,BLACK);
                }else if(Bar>DisplayWaveBarPos){
                    M5.Lcd.fillRect(10+DisplayWaveBarPos,181,Bar-DisplayWaveBarPos,18,GREEN);
                }
                DisplayWaveBarPos=Bar;
                char text[20];
                sprintf(text,"%3d:%02d.%02d/%3d:%02d.%02d",time/60000,time%60000/1000,time%1000/10,len/60000,len%60000/1000,len%1000/10);
                FastFont::printFastRom(DisplayWavePosString,text,10,205,WHITE,2,BLACK);
                DisplayWavePosString=text;
            break;
        }
    }
}
void FileManager::Background(){

}
bool FileManager::GetBackGround(){return 0;}
void FileManager::SetBackGround(bool run){return;}
bool FileManager::GetUpDateDraw(){return UpdateDraw;}
bool FileManager::GetToActive(){return 0;}
String FileManager::GetAppName(){return "File Manager";}
bool FileManager::GetGoToHome(){
    return tohome;
}