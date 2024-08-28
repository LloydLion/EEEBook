#ifndef GUI_DRAWING_FONTS_DRAW_FONT_H
#define GUI_DRAWING_FONTS_DRAW_FONT_H

#include <cstdint>
#include "gui/coordinates.h"
#include "std/byte.h"
#include "gui/drawing/Bitmap.h"

struct DrawFont
{
    struct Glyph
    {
        size_t bitmap_offset;
        cord_t width;
        cord_t height;
        cord_t x_advance;
        s_cord_t x_offset;
        s_cord_t y_offset;
    };

    byte *bitmap;
    Glyph *glyphs;
    char first;
    char last;
    cord_t y_advance;
    Bitmap::Flags bitmap_flags;
    const char *name;
};


#endif
