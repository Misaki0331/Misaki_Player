#include "Setting.h"
#include "../Fonts/FastFont.h"
#include "../Config.h"
using namespace Core::Draw;
using namespace Core::Setting;

bool Path::GetIsUpdate()
{
    if (IsUpdate)
    {
        IsUpdate = false;
        return 1;
    }
    return 0;
}
bool List::GetIsUpdate()
{
    if (IsUpdate)
    {
        IsUpdate = false;
        return 1;
    }
    return 0;
}
bool Num::GetIsUpdate()
{
    if (IsUpdate)
    {
        IsUpdate = false;
        return 1;
    }
    return 0;
}

bool Path::GetIsSetting()
{
    return IsSetting;
}
bool List::GetIsSetting()
{
    return IsSetting;
}
bool Num::GetIsSetting()
{
    return IsSetting;
}

void Path::Begin(String *path, String ext)
{
    IsSetting = true;
    IsFirstDraw = false;
    sellect = 0;
    scroll = 0;
    IsUpdate = true;
    ptr = path;
    if (ext.charAt(0) != '.')
        ext = "." + ext;
    ext.toLowerCase();
    fileExt = ext;
    openPath = "/";
    GetFileList();
}
void Path::Button(int type)
{
    //ここにボタン処理
    switch (type)
    {
    case 1:
        if (sellect > 0)
            sellect--;
        if (sellect - 1 < scroll + 1 && 0 < scroll)
            scroll = sellect - 1;
        break;
    case 2:
        if (FileLists[sellect].IsDirectry)
        {
            if (FileLists[sellect].IsCommandCancel)
            {
                IsSetting = false;
                break;
            }
            if (FileLists[sellect].IsCommandEmpty)
            {
                *ptr = "";
                IsSetting = false;
                break;
            }
            if (FileLists[sellect].IsCommnadBack)
            {
                openPath.remove(openPath.length() - 1, 1);
                while (openPath.charAt(openPath.length() - 1) != '/')
                    openPath.remove(openPath.length() - 1, 1);
                GetFileList();
                sellect = 0;
                scroll = 0;
                IsUpdate = true;
                IsPathUpdate = false;
                break;
            }
            openPath.concat(FileLists[sellect].Name);
            GetFileList();
            IsPathUpdate = false;
            sellect = 0;
            scroll = 0;
        }
        else
        {
            *ptr = openPath + FileLists[sellect].Name;
            IsSetting = false;
        }
        break;
    case 3:
        if (sellect < FileCount - 1)
            sellect++;
        if (sellect > scroll + 11 && FileCount > scroll + 13)
            scroll = sellect - 11;
        break;
    }
    IsUpdate = 1;
}
void Path::GetFileList()
{
    delete[] FileLists; //メモリのお掃除
    String PathStr = openPath;
    if (PathStr.length() > 1)
        PathStr.remove(PathStr.length() - 1, 1);
    File folder = SD.open(PathStr);
    int count = 1;
    FileCount = 1;
    File entry;
    //まずは中身の数を計算
    while (1)
    {
        entry = folder.openNextFile();
        if (!entry)
        {
            break; //以降ファイルなし
        }
        if (fileExt == ".*")
        {
            FileCount++;
        }
        else
        {
            if (entry.isDirectory())
            {
                FileCount++;
                continue;
            }
            String filename = entry.name();
            if (filename.length() < fileExt.length())
                continue;
            filename.toLowerCase();
            if (filename.endsWith(fileExt))
                FileCount++;
        }
    }
    // SDカードのルートに置いていればHomeに戻るフォルダーを入れる。
    //でなければ上のフォルダー階層に行くフォルダーを入れる

    folder = SD.open(PathStr);
    if (openPath != "/")
    {

        FileLists = new FileData[FileCount];
        FileLists[0].Name = "↑ 1つ前のディレクトリ";
        FileLists[0].IsDirectry = true;
        FileLists[0].Size = -1;
        FileLists[0].IsCommnadBack = true;
    }
    else
    {
        FileCount++;
        count = 2;
        FileLists = new FileData[FileCount];
        FileLists[0].Name = "キャンセル";
        FileLists[0].IsDirectry = true;
        FileLists[0].Size = -1;
        FileLists[0].IsCommandCancel = true;

        FileLists[1].Name = "(未設定)";
        FileLists[1].IsDirectry = true;
        FileLists[1].Size = -1;
        FileLists[1].IsCommandEmpty = true;
    }
    //ファイルの内容を取得
    while (1)
    {
        entry = folder.openNextFile();
        if (!entry)
        {
            break; //以降ファイルなし
        }
        String PathText = entry.name();
        PathText = PathText.substring(openPath.length(), PathText.length());

        if (entry.isDirectory())
        {
            FileLists[count].Name = PathText;
            FileLists[count].Name.concat("/");
            FileLists[count].IsDirectry = true;
            FileLists[count].Size = -1;
        }
        else
        {
            if (fileExt != ".*")
            {
                String filename = entry.name();
                if (filename.length() < fileExt.length())
                    continue;
                filename.toLowerCase();
                if (!filename.endsWith(fileExt))
                    continue;
            }

            FileLists[count].Name = PathText;
            FileLists[count].IsDirectry = false;
            FileLists[count].Size = entry.size();
        }
        count++;
    }
    FileCount = count;
    int start = 1;
    FileData Temp;
    //ファイナライズ
    for (int i = start + 1; i < FileCount; i++)
    {
        if (FileLists[i - 1].IsDirectry == false && FileLists[i].IsDirectry == true)
        {
            Temp = FileLists[i];
            int j = i;
            for (; FileLists[j - 1].IsDirectry == false && j > start; j--)
            {
                FileLists[j] = FileLists[j - 1];
            }
            FileLists[j] = Temp;
        }
    }
}
void Path::Draw()
{
    if (!IsFirstDraw)
    {
        IsFirstDraw = true;
        FastFont::printUtf8(title, 0, 15, YELLOW, 2, INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle, 0, 40, 0x7BEF, 1, INVISIBLE_COLOR);
        Core::SystemData::UpdateBatteryUI=1;
        Core::SystemData::UpdateSignalUI=1;
        //ここに画面初期化処理

        M5.Lcd.drawRect(300, 66, 20, 156, 0x7BEF);

        M5.Lcd.drawFastHLine(0, 224, 320, WHITE);
        FastFont::printUtf8("↑", 19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("決定", 19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("↓", 19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
    }
    if (!IsPathUpdate)
    {
        M5.lcd.fillRect(0, 53, 320, 12, BLACK);
        FastFont::printUtf8(openPath, 0, 53, WHITE, 1, INVISIBLE_COLOR);
    }
    //ここに描画処理
    for (int i = 0; i < 13; i++)
    {
        if (FileCount > scroll + i)
        {
            uint16_t col = WHITE;
            if (FileLists[scroll + i].IsDirectry)
                col = YELLOW;
            if (FileLists[scroll + i].IsCommandCancel)
                col = CYAN;
            if (FileLists[scroll + i].IsCommandEmpty)
                col = GREEN;

            if (sellect == scroll + i)
            {
                M5.Lcd.fillRect(0, 66 + 12 * i, 300, 12, col);
                FastFont::printUtf8(FileLists[scroll + i].Name, 0, 66 + 12 * i, BLACK, 1, INVISIBLE_COLOR);
            }
            else
            {
                M5.Lcd.fillRect(0, 66 + 12 * i, 300, 12, BLACK);
                FastFont::printUtf8(FileLists[scroll + i].Name, 0, 66 + 12 * i, col, 1, INVISIBLE_COLOR);
            }
        }
        else
        {
            M5.Lcd.fillRect(0, 66 + 12 * i, 300, 12, BLACK);
        }
    }
    M5.Lcd.fillRect(301, 67, 18, 154, BLACK);
    double cell = 154.0 / (FileCount);
    double start = (scroll)*cell;
    double end = (scroll + 13) * cell;
    if (end > 154)
        end = 154;
    end = end - start;
    M5.Lcd.fillRect(301, 67 + start, 18, end, WHITE);
}
void Path::Release()
{
    delete[] FileLists;
    FileLists = nullptr;
    ptr = nullptr;
    fileExt.clear();
    title.clear();
    subTitle.clear();
}
void Path::Cancel()
{
    IsSetting = false;
}

void List::Begin(String *name, const String *args, int argcount)
{
    mainstrcol=YELLOW;
    substrcol=0x7BEF;
    isCancel=false;
    IsSetting = true;
    scroll = -1;
    sellect = -1;
    IsUpdate = true;
    IsFirstDraw = false;
    ptr = name;
    listArgs = args;
    totalArgs = argcount;
}
void List::Button(int type)
{
    //ここにボタン処理
    switch (type)
    {
    case 1:
        if (sellect > -1)
            sellect--;
        if (sellect - 1 < scroll + 1 && -1 < scroll)
            scroll = sellect - 1;
        break;
    case 2:
        if (sellect == -1)
        {
            IsSetting = false;
            isCancel=true;
        }
        else
        {
            *ptr = listArgs[sellect];
            IsSetting = false;
        }
        break;
    case 3:
        if (sellect < totalArgs - 1)
            sellect++;
        if (sellect > scroll + 11 && totalArgs > scroll + 13)
            scroll = sellect - 11;
        break;
    }
    IsUpdate = 1;
}
void List::Draw()
{
    if (!IsFirstDraw)
    {
        IsFirstDraw = true;
        FastFont::printUtf8(title, 0, 15, mainstrcol, 2, INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle, 0, 40, substrcol, 1, INVISIBLE_COLOR);
        Core::SystemData::UpdateBatteryUI=1;
        Core::SystemData::UpdateSignalUI=1;

        M5.Lcd.drawRect(300, 61, 20, 157, 0x7BEF);
        //ここに画面初期化処理
        
        M5.Lcd.drawFastHLine(0, 224, 320, WHITE);
        FastFont::printUtf8("↑", 19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("決定", 19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("↓", 19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
    }
    for (int i = 0; i < 13; i++)
    {
        if (scroll + i == -1)
        {
            if (sellect == scroll + i)
            {
                M5.Lcd.fillRect(0, 61 + 12 * i, 300, 12, CYAN);
                FastFont::printUtf8("キャンセル", 0, 61 + 12 * i, BLACK, 1, INVISIBLE_COLOR);
            }
            else
            {
                M5.Lcd.fillRect(0, 61 + 12 * i, 300, 12, BLACK);
                FastFont::printUtf8("キャンセル", 0, 61 + 12 * i, CYAN, 1, INVISIBLE_COLOR);
            }
        }
        else if (scroll + i >= 0 && scroll + i < totalArgs)
        {
            if (sellect == scroll + i)
            {
                M5.Lcd.fillRect(0, 61 + 12 * i, 300, 12, WHITE);
                FastFont::printUtf8(listArgs[scroll + i], 0, 61 + 12 * i, BLACK, 1, INVISIBLE_COLOR);
            }
            else
            {
                M5.Lcd.fillRect(0, 61 + 12 * i, 300, 12, BLACK);
                FastFont::printUtf8(listArgs[scroll + i], 0, 61 + 12 * i, WHITE, 1, INVISIBLE_COLOR);
            }
        }
        else
        {

            M5.Lcd.fillRect(0, 61 + 12 * i, 300, 12, BLACK);
        }
    }
    M5.Lcd.fillRect(301, 62, 18, 155, BLACK);
    double cell = 155.0 / (totalArgs + 1);
    double start = (scroll + 1) * cell;
    double end = (scroll + 14) * cell;
    if (end > 155)
        end = 155;
    end = end - start;
    M5.Lcd.fillRect(301, 62 + start, 18, end, WHITE);

    //ここに描画処理
}
void List::Release()
{
    ptr = nullptr;
    listArgs = nullptr;
    title.clear();
    subTitle.clear();
}
void List::Cancel()
{
    isCancel=true;
    IsSetting = false;
}

void Num::Begin(int *value, uint8_t degit)
{
    isCancel=false;
    temp = 0;
    min = -2147483647;
    max = 2147483647;
    sellectDegit = 0;
    IsSetting = true;
    ptr = value;
    temp = *ptr;
    totalDegit = degit;
    IsFirstDraw = false;
    IsUpdate = true;
    if (totalDegit > 9)
        totalDegit = 9;
}
void Num::Button(int type)
{
    //ここにボタン処理
    switch (type)
    {
    case 1:
        if (IsDegitMode)
        {
            if (temp / powi(10, totalDegit - 1 - sellectDegit) % 10 == 0)
            {
                temp += powi(10, totalDegit - 1 - sellectDegit) * 9;
            }
            else
            {
                temp -= powi(10, totalDegit - 1 - sellectDegit);
            }
        }
        else
        {
            if (sellectDegit >= 0)
                sellectDegit--;
        }
        break;
    case 2:
        if (sellectDegit >= 0 && sellectDegit < totalDegit)
        {
            IsDegitMode = !IsDegitMode;
        }
        else
        {
            if (sellectDegit == -1)
            {
                IsSetting = false;
            }
            else
            {
                IsSetting = false;
                *ptr = temp;
            }
        }
        break;
    case 3:
        if (IsDegitMode)
        {
            if (temp / powi(10, totalDegit - 1 - sellectDegit) % 10 == 9)
            {
                temp -= powi(10, totalDegit - 1 - sellectDegit) * 9;
            }
            else
            {
                temp += powi(10, totalDegit - 1 - sellectDegit);
            }
        }
        else
        {
            if (sellectDegit < totalDegit)
                sellectDegit++;
        }
    }
    if (temp > max)
        temp = max;
    if (temp < min)
        temp = min;
    IsUpdate = 1;
}
void Num::Draw()
{
    if (!IsFirstDraw)
    {
        IsFirstDraw = true;
        FastFont::printUtf8(title, 0, 15, YELLOW, 2, INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle, 0, 40, 0x7BEF, 1, INVISIBLE_COLOR);
        Core::SystemData::UpdateBatteryUI=1;
        Core::SystemData::UpdateSignalUI=1;
        //ここに画面初期化処理
        for (int i = 0; i < totalDegit && i < 10; i++)
        {
            M5.lcd.drawRect(160 - 17 * totalDegit + 34 * i, 96, 5 * 6 + 4, 46, 0x7BEF);
        }
        M5.Lcd.drawFastHLine(0, 224, 320, WHITE);
        FastFont::printUtf8("←/－", -10+19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("変更/決定", -16+19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("→/＋", -10+19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
    }

    //ここに描画処理
    char *valchar = new char[2];
    for (int i = 0; i < totalDegit && i < 10; i++)
    {
        valchar[0]= '0' + temp / powi(10, totalDegit - i - 1) % 10;
        valchar[1]='\0';
        if (i == sellectDegit)
        {
            if (IsDegitMode)
            {
                FastFont::printRom(valchar, 160 - 17 * totalDegit + 34 * i + 2, 98, GREEN, 6, BLACK);
            }
            else
            {
                FastFont::printRom(valchar, 160 - 17 * totalDegit + 34 * i + 2, 98, YELLOW, 6, BLACK);
            }
        }
        else
        {
            FastFont::printRom(valchar, 160 - 17 * totalDegit + 34 * i + 2, 98, WHITE, 6, BLACK);
        }
    }
    if (sellectDegit == -1)
    {
        M5.lcd.fillRect(30, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, WHITE);
    }
    else
    {

        M5.lcd.fillRect(30, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, BLACK);
        M5.lcd.drawRect(30, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, WHITE);
    }
    FastFont::printRom("Cancel", 32, 172, sellectDegit == -1 ? BLACK : WHITE, 2);
    if (sellectDegit == totalDegit)
    {
        M5.lcd.fillRect(215, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, WHITE);
    }
    else
    {
        M5.lcd.fillRect(215, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, BLACK);
        M5.lcd.drawRect(215, 170, 6 * 6 * 2 + 3, 8 * 2 + 3, WHITE);
    }
    FastFont::printRom("  OK  ", 215, 172, sellectDegit == totalDegit ? BLACK : WHITE, 2);
    delete[] valchar;
}
void Num::Release()
{
    ptr = nullptr;
    totalDegit = 0;
    title.clear();
    subTitle.clear();
}
void Num::Cancel()
{
    isCancel=true;
    IsSetting = false;
}
long Num::powi(int x, int y)
{
    if (y < 0)
        return 0;
    if (y == 0)
        return 1;
    if (y >= 32)
        return -2147483648;
    int val = x;
    for (int i = 1; i < y; i++)
        val *= x;
    return val;
}
void Num::SetMin(int value)
{
    min = value;
    if (min > max)
        max = 2147483647;
}
void Num::SetMax(int value)
{
    max = value;
    if (max < min)
        min = -2147483647;
}

void TextBox::Begin(String *value, uint8_t max)
{
    isCancel=false;
    ptr = value;
    maxcount = max;
    //表示初期化
    IsSetting = true;
    IsTextUpdate = false;
    IsButtonUpdate = false;
    IsFirstDraw=false;
    //設定初期化
    sellectChar = 0;
    IsLargeCharMode = 0;
    tempStr = *ptr;
}
void TextBox::Release()
{
    ptr = nullptr;
    maxcount = 255;
    tempStr.clear();
    title.clear();
    subTitle.clear();
}
void TextBox::Cancel()
{
    isCancel=true;
    IsSetting = false;
}
bool TextBox::GetIsUpdate()
{
    return !IsTextUpdate || !IsButtonUpdate;
}
bool TextBox::GetIsSetting()
{
    return IsSetting;
}

// ToDo テキストボックスを作る。
void TextBox::Button(int type)
{
    switch (type)
    {
    case 1:
        sellectChar--;
        if (sellectChar < 0)
            sellectChar += 73;
        break;
    case 2:
        EnterCharacter(sellectChar);
        break;
    case 3:
        sellectChar++;
        if (sellectChar > 73)
            sellectChar -= 73;
    }
    IsButtonUpdate = false;
}
void TextBox::Draw()
{
    if (!IsFirstDraw)
    {
        IsFirstDraw = true;
        FastFont::printUtf8(title, 0, 15, YELLOW, 2, INVISIBLE_COLOR);
        FastFont::printUtf8(subTitle, 0, 40, 0x7BEF, 1, INVISIBLE_COLOR, true);M5.Lcd.drawFastHLine(0, 224, 320, WHITE);
        Core::SystemData::UpdateBatteryUI=1;
        Core::SystemData::UpdateSignalUI=1;
        FastFont::printUtf8("←", 19 + 94 * 0 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("決定", 19 + 94 * 1 + (100 - 25) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
        FastFont::printUtf8("→", 19 + 94 * 2 + (100 - 12) / 2, 226, WHITE, 1, INVISIBLE_COLOR);
    
    }
    if (!IsTextUpdate)
    {
        IsTextUpdate = true;
        short col=WHITE;
        char* text=new char[20];
        if (100 * tempStr.length() / maxcount >= 75)
            col = YELLOW;
        if (tempStr.length() == maxcount)
            col = RED;
        sprintf(text, "Length %3d/%3d", tempStr.length(), maxcount);
        FastFont::printRom(text, 320 - 6 * strlen(text), 54, col, 1, BLACK);
        delete[] text;
        M5.Lcd.fillRect(0, 62, 320, 16*(maxcount/26+1), BLACK);
        FastFont::printRom(tempStr, 0, 62, YELLOW, 2, BLACK,true);
    }
    if (!IsButtonUpdate)
    {
        IsButtonUpdate = true;
        M5.Lcd.fillRect(0, 160, 320, 60, BLACK);
        if (GetSoftKeyboardChar((sellectChar + 73) % 73) < 16)
        {
            switch (GetSoftKeyboardChar((sellectChar + 73) % 73))
            {
            case 0:
                FastFont::printRom("EXIT", (320 - 4 * 2 * 6) / 2+3, 220 - 2 * 8, WHITE, 2, BLACK);
                break;
            case 1:
                FastFont::printRom("SAVE", (320 - 4 * 2 * 6) / 2+3, 220 - 2 * 8, WHITE, 2, BLACK);
                break;
            case 2:
                FastFont::printRom("<-BS", (320 - 4 * 2 * 6) / 2+3, 220 - 2 * 8, WHITE, 2, BLACK);
                break;
            case 3:
                FastFont::printRom("A/a", (320 - 3 * 2 * 6) / 2+3, 220 - 2 * 8, WHITE, 2, BLACK);
                break;
            }
        }
        else
        {
            char *txt = new char[5];
            sprintf(txt, "%c", GetSoftKeyboardChar((sellectChar + 73) % 73));
            FastFont::printRom(txt, 148, 172, WHITE, 6, BLACK);
            delete[] txt;
        }
        for (int i = 0; i < 2; i++)
        {
            if (GetSoftKeyboardChar((sellectChar + 73 - 1 + i * 2) % 73) < 16)
            {
                switch (GetSoftKeyboardChar((sellectChar + 73 - 1 + i * 2) % 73))
                {
                case 0:
                    FastFont::printRom("EXIT", (140 - 4 * 1 * 6) / 2 + 186 * i, 220 - 1 * 8, WHITE, 1, BLACK);
                    break;
                case 1:
                    FastFont::printRom("SAVE", (140 - 4 * 1 * 6) / 2 + 186 * i, 220 - 1 * 8, WHITE, 1, BLACK);
                    break;
                case 2:
                    FastFont::printRom("<-BS", (140 - 4 * 1 * 6) / 2 + 186 * i, 220 - 1 * 8, WHITE, 1, BLACK);
                    break;
                case 3:
                    FastFont::printRom("A/a", (140 - 3 * 1 * 6) / 2 + 186 * i, 220 - 1 * 8, WHITE, 1, BLACK);
                    break;
                }
            }
            else
            {
                char *txt = new char[5];
                sprintf(txt, "%c", GetSoftKeyboardChar((sellectChar + 73 - 1 + i * 2) % 73));
                FastFont::printRom(txt, (140 - 3 * 5) / 2 + 186 * i, 220 - 3 * 8, WHITE, 3, BLACK);
                delete[] txt;
            }
        }
    }
}
void TextBox::EnterCharacter(uint8_t val)
{
    if (GetSoftKeyboardChar(val) >= 0 && GetSoftKeyboardChar(val) < 16)
    { //ここはコマンド
        switch (GetSoftKeyboardChar(val))
        {
        case 0: //キャンセル
            tempStr = "";
            Cancel();
            IsSetting=false;
            break;
        case 1: //保存
            *ptr = tempStr;
            IsSetting=false;
            break;
        case 2: // BackSpace
            if (tempStr.length() > 0)
                tempStr.remove(tempStr.length() - 1, 1);
            IsTextUpdate=false;
            break;
        case 3: //大文字小文字切り替え
            IsLargeCharMode = !IsLargeCharMode;
            break;
        }
    }
    else if (GetSoftKeyboardChar(val) > 15)
    { //文字の入力

        if (tempStr.length() < maxcount){
            tempStr += (char)GetSoftKeyboardChar(val);
            IsTextUpdate=false;
        }
    }
}

uint8_t TextBox::GetSoftKeyboardChar(uint8_t val)
{
    if (val == 0)
        return 0; // CANCEL
    if (val == 1)
        return 1; // SAVE
    if (val == 2)
        return 2; // BACKSPACE
    if (val >= 3 && val <= 12)
        return '0' + val - 3;
    if (val == 13)
        return ' '; //スペース
    if (val == 14)
        return 3; // Aa切替
    if (val >= 15 && val <= 40)
        return 'A' + val - 15 + 32 * !IsLargeCharMode;
    if (val >= 41 && val <= 55)
        return '!' + val - 41;
    if (val >= 56 && val <= 62)
        return ':' + val - 56;
    if (val >= 63 && val <= 68)
        return '[' + val - 63;
    if (val >= 69 && val <= 72)
        return '{' + val - 69;
    return 15; //範囲外
}