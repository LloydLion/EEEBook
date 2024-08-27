#ifndef GUI_DRAWING_SUB_DRAWERS_RECT_SUB_DRAWER_H
#define GUI_DRAWING_SUB_DRAWERS_RECT_SUB_DRAWER_H

#include "gui/coordinates.h"
#include "gui/drawing/Pattern.h"
#include "gui/drawing/Screen.h"
#include "gui/drawing/PatternBag.h"

class RectSubDrawer
{
private:
    cord_t _thickness;

    inline void draw_pixel_using_pattern(cord_t offset_x, cord_t offset_y, PatternBag &pattern)
    {
        transparent_color_t color = pattern.perform(CoordinateRangeValue(offset_x, 0, bounds.size.width()), CoordinateRangeValue(offset_y, 0, bounds.size.height()));
        if (!color.is_transparent())
            output->draw_pixel(bounds.start + Vector(offset_x, offset_y), color);
    }
public:
    Bounds bounds;
    Screen output;
    struct {
        PatternBag interior;
        PatternBag horizontal_strips;
        PatternBag vertical_strips;
        PatternBag corners;
    } patterns;
    
    RectSubDrawer(cord_t thickness);

    void draw();
};

#endif
