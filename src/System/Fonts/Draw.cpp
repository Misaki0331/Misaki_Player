#include"FastFont.h"

using namespace Core::Draw;
void FastFont::displayASCII(int x, int y, uint8_t chara, uint8_t siz, long color) {
  if (chara < 16)return;
  if(1){//siz==1){
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
          if(siz==1){
            M5.Lcd.drawFastVLine(x+tm%8,y+tm/8+mx,len,color);
          }else{
            M5.Lcd.fillRect(x+(tm%8)*siz,y+(tm/8+mx)*siz,siz,len*siz,color);
          }
          
        }else{
          if(IsM)tm++;
          if(siz==1){
            M5.Lcd.drawFastHLine(x+tm%8+mx,y+tm/8,len,color);
          }else{
            M5.Lcd.fillRect(x+(tm%8+mx)*siz,y+(tm/8)*siz,len*siz,siz,color);
          }
          
        }
        ptr++;
      }else if(tm<0x40){
        if(siz==1){
        M5.Lcd.drawPixel(x+tm%8,y+tm/8,color);
        }else{
          M5.Lcd.fillRect(x+(tm%8)*siz,y+(tm/8)*siz,siz,siz,color);
        }
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
      if(text[j]==0)break;
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
      if(text[j]==0)break;
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
  for (int len = 0; text[len] != 0 || len < ctext.length(); len++) {
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
      if (text[len ] == 0 || len >= ctext.length())break;
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
int FastFont::GetSjisPtr(int value){
  int ptr=value;
  
  if(0xA000<=value){
    ptr-=0x0000E040-(0x0B00);
    //ptr-=(value/256)-0xE0*3;
    ptr-=0x0040*(value/256-0xE0);
  }else{
    
    
    ptr-=0x00008140;
    //ptr-=(value/256)-0x81*3;
    ptr-=0x0040*((value/256)-0x81);
    
  }
  //if(value%256>=0x7F)ptr+=1;
  if(ptr==-1)return 5;
  if(value>0xEFFF)return 0;
  return SjisFontPtr[ptr];
}
void FastFont::displaySjis(int x,int y,short ptr,uint8_t siz,long color){
  if(siz==0)return;
  
  int i=0;
  int len=0;
  if(siz==1){
    for(int dy=0;dy<12;dy++){
      for(int dx=0;dx<12;dx++){
        //if(SjisFontBin[ptr*18+i/8]&(1<<(7-(i%8))))M5.Lcd.drawPixel(x+dx,y+dy,color);
        
        if(SjisFontBin[ptr*18+i/8]&(1<<(7-(i%8)))){
          len++;
        }else{
          M5.Lcd.drawFastHLine(x+dx-len,y+dy,len,color);
          len=0;
        }
        i++;
        

      }
      if(len>0){
        M5.Lcd.drawFastHLine(x+12-len,y+dy,len,color);
        len=0;
      }
    }
  }else{
    for(int dy=0;dy<12;dy++){
      for(int dx=0;dx<12;dx++){
        if(SjisFontBin[ptr*18+i/8]&(1<<(7-(i%8)))){
          len++;
        }else{
          M5.Lcd.fillRect(x+(dx-len)*siz,y+dy*siz,siz*len,siz,color);
          len=0;
        }
        i++;
      }
      if(len>0){
        M5.Lcd.fillRect(x+(12-len)*siz,y+dy*siz,siz*len,siz,color);
        len=0;
      }
    }
  }

}
void FastFont::displayHSjis(int x,int y,short ptr,uint8_t siz,long color){
  if(siz==0)return;
  
  int i=0;
  int len=0;
  if(siz==1){
    for(int dy=0;dy<12;dy++){
      for(int dx=0;dx<6;dx++){
        if(SjisHFontBin[ptr*9+i/8]&(1<<(7-(i%8)))){
          len++;
        }else{
          M5.Lcd.drawFastHLine(x+dx-len,y+dy,len,color);
          len=0;
        }
        i++;
        
      }
      if(len>0){
        M5.Lcd.drawFastHLine(x+6-len,y+dy,len,color);
        len=0;
      }
    }
  }else{
    for(int dy=0;dy<12;dy++){
      for(int dx=0;dx<6;dx++){
        if(SjisHFontBin[ptr*9+i/8]&(1<<(7-(i%8)))){
          len++;
        }else{
          
          M5.Lcd.fillRect(x+(dx-len)*siz,y+dy*siz,siz*len,siz,color);
          len=0;
        }
        i++;
      }
      if(len>0)M5.Lcd.fillRect(x+(6-len)*siz,y+dy*siz,siz*len,siz,color);
    }
  }

}
void FastFont::printSjis(String t,int x,int y,uint16_t color,uint8_t size,long bg){
  

  uint8_t* text;
 
    text=new uint8_t[t.length()+1];
    for(int i=0;i<t.length();i++)text[i]=t.charAt(i);
    text[t.length()]=0;
  
  int dx=0;
  for(int i=0;i<t.length();i++)text[i]=t.charAt(i);
 
  for(int i=0;i<t.length();i++){
    if((text[i]>=0x81&&text[i]<=0x9F)||(text[i]>=0xE0&&text[i]<=0xEF)){
      unsigned short chr=(text[i]+256)%256;
      i++;
      chr*=256;
      chr+=(text[i]+256)%256;
      
      if(bg>=0)M5.Lcd.fillRect(x+dx,y,12,12,bg);
      displaySjis(x+dx,y,GetSjisPtr(chr),size,color);
      dx+=13;
    }else{
      unsigned short chr=(text[i]+256)%256;
      if(bg>=0)M5.Lcd.fillRect(x+dx,y,6,12,bg);
      displayHSjis(x+dx,y,chr-0x20,size,color);
      dx+=7;
    }
  }
  delete[] text;
}
void FastFont::begin(){
    
    
}


void FastFont::printUtf8(String t,int x,int y,uint16_t color,uint8_t size,long bg,bool AutoBR){
  

  uint8_t* text;
  
  text=new uint8_t[t.length()+1];
  for(int i=0;i<t.length();i++)text[i]=t.charAt(i);
  text[t.length()]=0;
  
  int dx=0;
  for(int i=0;i<t.length();i++)text[i]=t.charAt(i);
  
  for(int i=0;i<t.length();){
    int ptr=-1;
    if(text[i]>=0x20&&text[i]<=0x7F){
      Serial.printf("%02x\n",text[1]);
      int chr=(text[i]+256)%256;
      if(bg>=0)M5.Lcd.fillRect(x+dx,y,6*size,12*size,bg);
      displayHSjis(x+dx,y,chr-0x20,size,color);
      dx+=7*size;
      i++;
    }else if(text[i]>=0xc0&&text[i]<=0xdf){
      int chr=(text[i]+256)%256;
      chr*=256;
      chr+=(text[i+1]+256)%256;
      for(ptr=0;ptr<17;ptr++){
        if(Utf8FontPtr2[ptr]<=chr&&Utf8FontPtr2[ptr]+15>=chr)break;
        if(ptr==16)ptr=9999;
      }
      if(ptr==10000)ptr=-1;
      if(ptr==-1){i++;continue;}
      if(bg>=0)M5.Lcd.fillRect(x+dx,y,12*size,12*size,bg);
      displaySjis(x+dx,y,Utf8FontPtr[Utf8FontPtr2[ptr]*16+chr%16],size,color);
      dx+=13*size;
      i+=2;
    }else if(text[i]>=0xe0&&text[i]<=0xef){
      int chr=(text[i]+256)%256;
      chr*=256;
      chr+=(text[i+1]+256)%256;
      chr*=256;
      chr+=(text[i+2]+256)%256;
      
      for(ptr=17;ptr<1383;ptr++){
        if(Utf8FontPtr2[ptr]<=chr&&Utf8FontPtr2[ptr]+15>=chr)break;
        if(ptr==1382)ptr=9999;
      }
      if(ptr==10000)ptr=-1;
      if(ptr==-1){i++;continue;}
      if(bg>=0)M5.Lcd.fillRect(x+dx,y,12*size,12*size,bg);
      displaySjis(x+dx,y,Utf8FontPtr[ptr*16+chr%16],size,color);
      i+=3;
      dx+=13*size;
    }else{
      i++;
    }
    if(dx+x>308&&AutoBR){
      dx=0;
      y+=13*size;
    }
    
  }
  delete[] text;
}