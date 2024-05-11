#ifndef GUI_BITMAP_H
#define GUI_BITMAP_H

#include "cordinates.h"
#include <Arduino.h>

struct Bitmap
{
    byte *data;
    cord_t width;
    cord_t height;

    inline Size size()
    {
        return Size(width, height);
    }
    
    inline Bitmap(byte *data, cord_t width, cord_t height):
        data(data), width(width), height(height) {}
};

#endif