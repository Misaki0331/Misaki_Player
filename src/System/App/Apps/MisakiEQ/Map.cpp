#include "MisakiEQ.hpp"
using namespace App::MisakiEQ;
void Map::Draw(double longitude, double latitude, uint8_t size)
{
    double x = (90.5988 * longitude - 11055.691) / 2500.0;
    double y = (-105.72 * latitude + 5018.9725) / 2500;
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
    int ix=160;
    int iy=14+105;
    M5.lcd.drawLine(ix - 5, iy - 7, ix + 7, iy + 5,RED);
    M5.lcd.drawLine(ix - 6, iy - 7, ix + 7, iy + 6,RED);
    M5.lcd.drawLine(ix - 6, iy - 6, ix + 6, iy + 6,RED);
    M5.lcd.drawLine(ix - 7, iy - 6, ix + 6, iy + 7,RED);
    M5.lcd.drawLine(ix - 7, iy - 5, ix + 5, iy + 7,RED);
    M5.lcd.drawLine(ix - 5, iy + 7, ix + 7, iy - 5,RED);
    M5.lcd.drawLine(ix - 6, iy + 7, ix + 7, iy - 6,RED);
    M5.lcd.drawLine(ix - 6, iy + 6, ix + 6, iy - 6,RED);
    M5.lcd.drawLine(ix - 7, iy + 6, ix + 6, iy - 7,RED);
    M5.lcd.drawLine(ix - 7, iy + 5, ix + 5, iy - 7,RED);
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