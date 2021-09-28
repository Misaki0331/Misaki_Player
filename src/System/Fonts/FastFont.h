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
            static uint16_t Color16[];
            static void printFastRom(String t1,String t2,int x,int y,uint16_t color, uint8_t siz,long bgc);
            private:
            static const unsigned char AsciiFontBin[];
            static const unsigned short AsciiFontPtr[];
            static const unsigned char AsciiOldFont[];
            static void displayASCII(int x,int y, uint8_t chara,uint8_t siz,long color);
            static int PosX;
            static int PosY;
            static uint16_t TextColor;
            static long BackColor;
        };
        
    }
}