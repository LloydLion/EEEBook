#ifndef GUI_BITMAP_H
#define GUI_BITMAP_H

#include "gui/coordinates.h"
#include <cstddef>

struct Bitmap
{
    std::byte *data;
    cord_t width;
    cord_t height;

    inline Size size()
    {
        return Size(width, height);
    }
    
    inline Bitmap(std::byte *data, cord_t width, cord_t height):
        data(data), width(width), height(height) {}
};

#endif