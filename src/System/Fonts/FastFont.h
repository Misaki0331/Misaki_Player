#include<M5Stack.h>
#define INVISIBLE_COLOR -1
namespace Core{
    namespace Draw{
        
        class FastFont{
            public:
            static void setColor(uint16_t color,long bg);
            static void setPos(int x,int y);
            static void printf(const char* format, ...);
            static void printRom(String t, int x, int y, uint16_t color);
            static void printRom(String t, int x, int y, uint16_t color, uint8_t siz);
            static void printRom(String t, int x, int y, uint16_t color, uint8_t siz, long bgc);
            static void printConsole(String ctext,int x,int y);
            static void printSjis(String t,int x,int y,uint16_t color,uint8_t siz,long bg);
            static uint16_t Color16[];
            static void printFastRom(String t1,String t2,int x,int y,uint16_t color, uint8_t siz,long bgc);
            static void printUtf8(String t,int x,int y,uint16_t color,uint8_t siz,long bg,bool AutoBR=0);
            static void begin();
            
            private:
            static const unsigned char AsciiFontBin[];
            static const unsigned short AsciiFontPtr[];
            static const unsigned char SjisFontBin[];
            static const unsigned short SjisFontPtr[];
            static const unsigned char SjisHFontBin[];
            static const short Utf8FontPtr[];
            static const int Utf8FontPtr2[];
            static int GetSjisPtr(int value);
            static void displaySjis(int x,int y, short ptr,uint8_t siz,long color);
            static void displayHSjis(int x,int y, short ptr,uint8_t siz,long color);
            static void displayASCII(int x,int y, uint8_t chara,uint8_t siz,long color);
            static int PosX;
            static int PosY;
            static uint16_t TextColor;
            static long BackColor;

        };
        
    }
}