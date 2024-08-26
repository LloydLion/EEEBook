#ifndef GUI_BITMAP_H
#define GUI_BITMAP_H

#include "gui/coordinates.h"
#include <cstddef>
#include "std/byte.h"

struct Bitmap
{
    enum Flags : uint8_t
    {
        BigEndian = 0b001,
        LittleEndian = 0b000,
        Inverted = 0b010,
        XPrimary = 0b000,
        YPrimary = 0b100
    };

    byte *data;
    Size size;
    Flags flags;
    
    inline Bitmap(byte *data, Size size, Flags flags):
        data(data), size(size), flags(flags) {}

    bool get_pixel(Vector position);
};

#endif