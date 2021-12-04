#include "MisakiEQ.hpp"
#include "../../../Fonts/FastFont.h"
using namespace Core::Draw;
using namespace App::MisakiEQ;
void Map::Draw(double longitude, double latitude, uint8_t size)
{
    double x = (91.096262 * longitude - 11121.6491) / 2500.0;
    double y = (4322.4854 - 60.13934 * latitude - 0.7343239 * pow(latitude, 2)) / 2500;
    int zoom = 0;
    switch (size)
    {
    case 1:
        zoom = 500;
        break;
    case 2:
        zoom = 1000;
        break;
    case 3:
        zoom = 1500;
        break;
    case 4:
        zoom = 2000;
        break;
    case 5:
        zoom = 2500;
        break;
    default:
        return;
    }
    x *= zoom;
    y *= zoom;
    DrawPnt((short)x, (short)y, size);
}
void Map::DrawPnt(short x, short y, uint8_t size)
{
    DrawLeft(x - 320 / 2, y - 210 / 2, size);
    int ix = 160;
    int iy = 14 + 105;
}
void Map::DrawLeft(short x0, short y0, uint8_t size)
{
    M5.Lcd.fillRect(0, 14, 320, 210, BLUE);
    const uint16_t *map_ptr;
    const uint16_t *map_bin;
    int maxy = 0;
    switch (size)
    {
    case 1:
        map_ptr = map_500_ptr;
        map_bin = map_500_bin;
        maxy = 500;
        break;
    case 2:
        map_ptr = map_1000_ptr;
        map_bin = map_1000_bin;
        maxy = 1000;
        break;
    case 3:
        map_ptr = map_1500_ptr;
        map_bin = map_1500_bin;
        maxy = 1500;
        break;
    case 4:
        map_ptr = map_2000_ptr;
        map_bin = map_2000_bin;
        maxy = 2000;
        break;
    case 5:
        map_ptr = map_2500_ptr;
        map_bin = map_2500_bin;
        maxy = 2500;
        break;
    default:
        return;
    }
    MapX=x0;
    MapY=y0;
    zm=maxy;
    for (int y1 = 0; y1 < 210; y1++)
    {
        if (y1 + y0 >= maxy && y1 + y0 < 0)
            continue;
        int x1 = 0;
        for (int ptr = map_ptr[y1 + y0]; map_bin[ptr] != 0xFFFF; ptr++)
        {
            uint8_t coltype = map_bin[ptr] / 0x1000;
            if (x0 <= x1 + map_bin[ptr] % 0x1000)
            {
                uint16_t color = WHITE;
                switch (coltype)
                {
                case 1:
                    color = GREEN;
                    break;
                case 2:
                    color = DARKGREEN;
                    break;
                case 3:
                    color = LIGHTGREY;
                    break;
                case 4:
                    color = 0x7BEF;
                    break;
                }
                if (coltype != 0)
                    M5.lcd.drawFastHLine(x1 - x0, y1 + 14, map_bin[ptr] % 0x1000, color);
            }
            x1 += map_bin[ptr] % 0x1000;
        }
    }
}
double Map::GetPointLatitude(uint16_t ID)
{
    for (int i = 0; i < 188; i++)
    {
        if (ID == Map_EEW_Num[i])
            return Map_EEW_lat[i];
    }
    return NAN;
}
double Map::GetPointLongitude(uint16_t ID)
{
    for (int i = 0; i < 188; i++)
    {
        if (ID == Map_EEW_Num[i])
            return Map_EEW_long[i];
    }
    return NAN;
}
uint8_t Map::GetShindoValue(String str)
{
    if (str == "1")
        return 1;
    if (str == "2")
        return 2;
    if (str == "3")
        return 3;
    if (str == "4")
        return 4;
    if (str == "5弱")
        return 5;
    if (str == "5強")
        return 6;
    if (str == "6弱")
        return 7;
    if (str == "6強")
        return 8;
    if (str == "7")
        return 9;
    return 0;
}
void Map::DrawShindo(uint16_t ID, String Shindo)
{
    double lat;
    double lon;
    for (int i = 0; i < 188; i++)
    {
        if (ID == Map_EEW_Num[i]){
            lat=Map_EEW_lat[i];
            lon=Map_EEW_long[i];
            double x = (91.096262 * lon - 11121.6491) / 2500.0;
            double y = (4322.4854 - 60.13934 * lat - 0.7343239 * pow(lat, 2)) / 2500.0;
            int shindo = GetShindoValue(Shindo);
            drawShindo(x*zm,y*zm,shindo);
            return;                          
        }
    }
}
void Map::DrawHypocenter(double longitude, double latitude){
    double x = (91.096262 * longitude - 11121.6491) / 2500.0;
    double y = (4322.4854 - 60.13934 * latitude - 0.7343239 * pow(latitude, 2)) / 2500;
    int ix=x*zm-MapX;
    int iy=y*zm-MapY+14;
    if(ix<-15||ix>320)return;
    if(iy<14||iy>217)return;
    
    M5.lcd.drawLine(ix - 5, iy - 7, ix + 7, iy + 5, RED);
    M5.lcd.drawLine(ix - 6, iy - 7, ix + 7, iy + 6, RED);
    M5.lcd.drawLine(ix - 6, iy - 6, ix + 6, iy + 6, RED);
    M5.lcd.drawLine(ix - 7, iy - 6, ix + 6, iy + 7, RED);
    M5.lcd.drawLine(ix - 7, iy - 5, ix + 5, iy + 7, RED);
    M5.lcd.drawLine(ix - 5, iy + 7, ix + 7, iy - 5, RED);
    M5.lcd.drawLine(ix - 6, iy + 7, ix + 7, iy - 6, RED);
    M5.lcd.drawLine(ix - 6, iy + 6, ix + 6, iy - 6, RED);
    M5.lcd.drawLine(ix - 7, iy + 6, ix + 6, iy - 7, RED);
    M5.lcd.drawLine(ix - 7, iy + 5, ix + 5, iy - 7, RED);
}
void Map::drawShindo(int16_t x0, int16_t y0, uint8_t value)
{
    double x,y;
    x=x0-MapX;
    y=y0-MapY;
    if(x<-7||x>320)return;
    if(y<7||y>203)return;
    if (!(value > 0 && value < 10))
        return;
    y+=14;
    m5.lcd.fillRect(x - 7, y - 7, 15,15, JPN_shindo_color[6 * value + 4]);
    m5.lcd.drawFastHLine(x - 7, y - 7, 15, JPN_shindo_color[6 * value]);
    m5.lcd.drawFastVLine(x - 7, y - 7, 15, JPN_shindo_color[6 * value + 1]);
    m5.lcd.drawFastHLine(x - 7, y + 7, 15, JPN_shindo_color[6 * value + 2]);
    m5.lcd.drawFastVLine(x + 7, y - 7, 15, JPN_shindo_color[6 * value + 3]);
    if (value == 9)
    {
        m5.lcd.drawFastHLine(x - 6, y - 6, 13, JPN_shindo_color[6 * value]);
        m5.lcd.drawFastVLine(x - 6, y - 6, 13, JPN_shindo_color[6 * value + 1]);
        m5.lcd.drawFastHLine(x - 6, y + 6, 13, JPN_shindo_color[6 * value + 2]);
        m5.lcd.drawFastVLine(x + 6, y - 6, 13, JPN_shindo_color[6 * value + 3]);
    }
    if (value >= 5 && value <= 8)
    {
        switch (value)
        {
        case 5:
            FastFont::printRom("5-", x - 5, y - 3, JPN_shindo_color[6 * value + 5]);
            break;
        case 6:
            FastFont::printRom("5+", x - 5, y - 3, JPN_shindo_color[6 * value + 5]);
            break;
        case 7:
            FastFont::printRom("6-", x - 5, y - 3, JPN_shindo_color[6 * value + 5]);
            break;
        case 8:
            FastFont::printRom("6+", x - 5, y - 3, JPN_shindo_color[6 * value + 5]);
            break;
        }
    }
    else
    {
        char *sindo = new char[5];
        if (value < 5)
        {
            sprintf(sindo, "%d", value);
            FastFont::printRom(sindo, x - 2, y - 3, JPN_shindo_color[6 * value + 5]);
        }
        else
        {
            sprintf(sindo, "%d", value - 2);
            FastFont::printRom(sindo, x - 2, y - 3, JPN_shindo_color[6 * value + 5]);
        }
        delete[] sindo;
    }
}
String Map::GetShindoName(int val){
    if(val<0||val>9)return "";
    switch(val){
        case 0: return "0";
        case 1: return "1";
        case 2: return "2";
        case 3: return "3";
        case 4: return "4";
        case 5: return "5弱";
        case 6: return "5強";
        case 7: return "6弱";
        case 8: return "6強";
        case 9: return "7";
    }
}
const uint16_t Map::JPN_shindo_color[] = { //某放送協会の震度表示の色を真似ました。外枠の線から内枠の塗り、文字色まで設定しています。
    0, 0, 0, 0, 0, 0,
    0xA6BF, 0x7E3F, 0x4479, 0x33B5, 0x559F, 0x0000,
    0xBF54, 0x9EF0, 0x6548, 0x5467, 0x7EAB, 0x0000,
    0xF777, 0xF733, 0xBD8C, 0xA4AA, 0xF6EF, 0x0000,
    0xFDB1, 0xFCCB, 0xC2E4, 0xA263, 0xFBC5, 0x0000,
    0xD3F1, 0xC24A, 0x8863, 0x7042, 0xB064, 0xFFFF,
    0xD3F1, 0xC24A, 0x8863, 0x7042, 0xB064, 0xFFFF,
    0xFF5E, 0xFEDC, 0xC30D, 0xA125, 0xFE7B, 0x8800,
    0xFF5E, 0xFEDC, 0xC30D, 0xA125, 0xFE7B, 0x8800,
    0xFF53, 0xFF2F, 0xFE82, 0xF5E0, 0xB064, 0xFFFF};