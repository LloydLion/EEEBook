#ifndef GUI_DRAWING_SUB_DRAWERS_BITMAP_SUB_DRAWER_H
#define GUI_DRAWING_SUB_DRAWERS_BITMAP_SUB_DRAWER_H

#include "gui/drawing/Pattern.h"
#include "gui/coordinates.h"
#include "gui/drawing/Screen.h"
#include "gui/drawing/PatternBag.h"

class BitmapSubDrawer
{
private:
    byte *_map;
    Bitmap::Flags _flags;

    
    bool get_bitmap_pixel(cord_t x, cord_t y);

public:
    Bounds bounds;
    Screen output;
    struct
    {
        PatternBag for_enabled;
        PatternBag for_disabled;
    } patterns;

    BitmapSubDrawer(byte *map, Bitmap::Flags flags);

    void draw();
};

#endif
