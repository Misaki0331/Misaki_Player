#include<M5Stack.h>
namespace Core{
    class DrawTaskUI{
        public:
        static void Battery(int x,int y,int percent,bool isAllDraw);
        static void RSSI(int x,int y,int rssi,bool isConnected,bool isAllDraw);
    };
}