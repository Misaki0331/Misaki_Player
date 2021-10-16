#include"FastFont.h"
using namespace Core::Draw;
void FastFont::displayASCII(int x, int y, uint8_t chara, uint8_t siz, long color) {
  if (chara < 16)return;
  if (siz > 1) {
    for (int p = 0; p < 5; p++) {
      uint8_t bin = AsciiOldFont[(chara - 16) * 5 + p];
      bool ba = 0;
      int bc = 0;
      
        for (int q = 0; q < 8; q++)
        {
          if (bin & (1 << q))M5.Lcd.fillRect(x + p * siz, y + q * siz,siz,siz,color);
        }
      
    }
  }else if(siz==1){
    //char tex[256];
    //sprintf(tex,"%c:",chara);
    int ptr=AsciiFontPtr[chara];
    while(1){
      uint8_t tm=AsciiFontBin[ptr];
      //sprintf(tex,"%s%|02X ",tex,tm);
      if(tm>=0x80)break;
      if(0x40<=tm&&tm<0x80){
        tm-=0x40;
        
        uint8_t tm2=AsciiFontBin[ptr+1];
        
        //sprintf(tex,"%s%02X |",tex,tm2);
        if(tm2>=0x80)break;
        bool IsH=false;
        if(tm2>=64){
          tm2-=64;
          IsH=true;
        }
        bool IsM=false;
        if(tm2>=32){
            tm2-=32;
            IsM=true;
          
        }else{
          
        }
        int len=tm2;
        len++;
        int mx=0;
        if(IsM){
            mx=-len;
        }
        if(IsH){
          if(IsM)tm+=8;
          M5.Lcd.drawFastVLine(x+tm%8,y+tm/8+mx,len,color);
          
        }else{
          if(IsM)tm++;
          M5.Lcd.drawFastHLine(x+tm%8+mx,y+tm/8,len,color);
          
        }
        ptr++;
      }else if(tm<0x40){
        M5.Lcd.drawPixel(x+tm%8,y+tm/8,color);
      }
      
      ptr++;
    }
    //sprintf(tex,"%s(Ptr:%4d) ",tex,pgm_read_word(ASCII_Font_ptr+chara));
    //Serial.println(tex);
    
  }
}
void FastFont::printRom(String t, int x, int y, uint16_t color) {
  printRom(t, x, y, color, 1, -1);
}
void FastFont::printRom(String t, int x, int y, uint16_t color, uint8_t siz) {
  printRom(t, x, y, color, siz, -1);
}
void FastFont::printRom(String t, int x, int y, uint16_t color, uint8_t siz, long bgc) {
  if (siz == 0)return;
  char *text;
  text = new char[t.length() + 1];
  t.toCharArray(text, t.length() + 1);
  for (int j = 0; j < t.length(); j++) {
      if(text[j]==NULL)break;
    if (bgc >= 0 && bgc < 65536) {
      M5.Lcd.fillRect(x + 6 * siz * j, y, 5 * siz , 7 * siz,bgc);
    }
    displayASCII(x + 6 * siz * j, y, text[j], siz,color);
  }
  delete[] text;
}
void FastFont::printFastRom(String from,String to,int x,int y,uint16_t color, uint8_t siz,long bgc){
    if (siz == 0)return;
  char *text;
  char *text2;
  text = new char[to.length() + 1];
  int Ssiz=from.length();
  if(from.length()<to.length()){
      Ssiz=to.length();
  }

  text2 = new char[Ssiz+1];
  to.toCharArray(text, to.length() + 1);
  from.toCharArray(text2,from.length()+1);
  for (int j = 0; j < to.length(); j++) {
      if(text[j]==NULL)break;
      if(text[j]!=text2[j]){
    if (bgc >= 0 && bgc < 65536) {
      M5.Lcd.fillRect(x + 6 * siz * j, y, 5 * siz , 7 * siz,bgc);
    }
    displayASCII(x + 6 * siz * j, y, text[j], siz,color);
      }
  }
  delete[] text;
  delete[] text2;
}
void FastFont::printConsole(String ctext,int x,int y) {
  char *text;
  text = new char[ctext.length() + 1];
  ctext.toCharArray(text, ctext.length() + 1);
  uint16_t cid = 0xffff;
  char buckup[64];
  int col = 0;
  int skip = 0;
  int zx = 0;
  int fx = 0;
  char he[2];
  for (int len = 0; text[len] != NULL || len < ctext.length(); len++) {
    if (text[len] == '|' && text[len + 1] == '*') {

      he[0] = text[len + 2]; he[1] = 0;
      sscanf(he, "%x", &col);
      buckup[skip] = 0;

      printRom(buckup, x+fx * 6, y, cid);
      fx += zx;
      zx = 0;
      skip = 0;
      //sprintf(buckup,"color is %4x get %x",convert_color(pgm_read_word(char_col + 3 * col), pgm_read_word(char_col + 3 * col + 1), pgm_read_word(char_col + 3 * col + 2)),col)
      //;//mylcd.Set_Text_colour(WHITE);
      //mylcd.Print_String(buckup, fx * 6, (i ) % 480);
      cid = Color16[col];
      //mylcd.Set_Text_colour(pgm_read_word(char_col +  col));
      len += 2;
      if (text[len ] == NULL || len >= ctext.length())break;
    } else {
      buckup[skip] = text[len];
      skip++;
      zx++;
    }


  }
  buckup[skip] = 0;

  printRom(buckup, x+fx * 6, y , cid);
  delete[] text;
}
int FastFont::PosX=0;
int FastFont::PosY=0;
uint16_t FastFont::TextColor=0xFFFF;
long FastFont::BackColor=0x0000;
void FastFont::printf(const char* format, ...){
    char*buffer="";
    va_list ap;
    va_start(ap, format);
    vsprintf(buffer, format, ap);
    va_end(ap);
    printRom(buffer,PosX,PosY,TextColor,1,BackColor);
    PosX+=(int)(sizeof(buffer)-1)*6;
    delete[] buffer;
}
void FastFont::setPos(int x,int y){
    PosX=x;
    PosY=y;
}
void FastFont::setColor(uint16_t color,long bg){
    TextColor=color;
    BackColor=bg;
}