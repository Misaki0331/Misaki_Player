#include"MisakiEQ.hpp"
using namespace App::MisakiEQ;
void Map::Draw(short x0, short y0, uint8_t size){
    //1,2,4と4の倍数以外は弾く
    if(size==0)return;
    if(size==3)return;
    if(size>4&&size%4!=0)return;
    M5.Lcd.fillRect(0, 14, 320, 210, BLUE);
    switch(size){
        case 4:
            for(int y1=0;y1<210;y1++){
                if(y1+y0>=1280&&y1+y0<0)continue;
                int x1=0;
                for(int ptr=Japan_map_ptr[y1+y0];Japan_map_bin[ptr]!=0xFFFF;ptr++){
                    uint8_t coltype=Japan_map_bin[ptr]/0x1000;
                    if(x0<=x1+Japan_map_bin[ptr]%0x1000){
                        uint16_t color=WHITE;
                        switch(coltype){
                            case 1:
                                color=GREEN;
                                break;
                            case 2:
                                color=DARKGREEN;
                                break;
                            case 3:
                                color=LIGHTGREY;
                                break;
                            case 4:
                                color=0x7BEF;
                                break;
                        }
                        if(coltype!=0)M5.lcd.drawFastHLine(x1-x0,y1+14,Japan_map_bin[ptr]%0x1000,color);
                    }
                    x1+=Japan_map_bin[ptr]%0x1000;
                }
            }
            break;
    }

}