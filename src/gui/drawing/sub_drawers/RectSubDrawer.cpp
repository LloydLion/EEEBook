#include "gui/drawing/sub_drawers/RectSubDrawer.h"

RectSubDrawer::RectSubDrawer(cord_t thickness): _thickness(thickness) { }

void RectSubDrawer::draw()
{
    cord_t width = bounds.size.width();
    cord_t height = bounds.size.height();

    if (_thickness == 0)
    {
        for (cord_t x = 0; x < width; x++)
            for (cord_t y = 0; y < height; y++)
            {
                draw_pixel_using_pattern(x, y, patterns.interior);
            }
        return;
    }

    // corners
    for (cord_t x = 0; x < _thickness; x++)
        for (cord_t y = 0; y < _thickness; y++)
        {
            draw_pixel_using_pattern(x, y, patterns.corners);
            draw_pixel_using_pattern(x + width - _thickness - 1, y, patterns.corners);
            draw_pixel_using_pattern(x, y + height - _thickness - 1, patterns.corners);
            draw_pixel_using_pattern(x + width - _thickness - 1, y + height - _thickness - 1, patterns.corners);
        }

    // vertical strips
    for (cord_t x = 0; x < _thickness; x++)
        for (cord_t y = _thickness; y < height - _thickness; y++)
        {
            draw_pixel_using_pattern(x, y, patterns.vertical_strips);
            draw_pixel_using_pattern(x + width - _thickness - 1, y, patterns.vertical_strips);
        }

    // horizontal strips
    for (cord_t x = _thickness; x < width - _thickness; x++)
        for (cord_t y = 0; y < _thickness; y++)
        {
            draw_pixel_using_pattern(x, y, patterns.horizontal_strips);
            draw_pixel_using_pattern(x, y + height - _thickness - 1, patterns.horizontal_strips);
        }

    // interior
    for (cord_t x = _thickness; x < width - _thickness; x++)
        for (cord_t y = _thickness; y < height - _thickness; y++)
            draw_pixel_using_pattern(x, y, patterns.interior);
}
