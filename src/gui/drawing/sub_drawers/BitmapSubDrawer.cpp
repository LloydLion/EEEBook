#include "gui/drawing/sub_drawers/BitmapSubDrawer.h"

BitmapSubDrawer::BitmapSubDrawer(byte *map, Bitmap::Flags flags)
{ }


bool BitmapSubDrawer::get_bitmap_pixel(cord_t x, cord_t y)
{
    cord_t width = bounds.size.width();
    cord_t height = bounds.size.height();

    size_t idx = _flags & Bitmap::YPrimary ? y + x * height : x + y * width;
    size_t byte_idx = idx / 8;
    size_t bit_idx = _flags & Bitmap::BigEndian ? 7 - idx % 8 : idx % 8;
    bool state = _map[byte_idx] & (1 << bit_idx);
    if (_flags & Bitmap::Inverted)
        state = !state;
    return state;
}


void BitmapSubDrawer::draw()
{
    cord_t width = bounds.size.width();
    cord_t height = bounds.size.height();
    for (cord_t x = 0; x < width; x++)
        for (cord_t y = 0; y < height; y++)
        {
            bool state = get_bitmap_pixel(x, y);
            const Pattern &pattern = state ? patterns.for_enabled : patterns.for_disabled;
            transparent_color_t color = pattern.perform(CoordinateRangeValue(x, 0, bounds.size.width()), CoordinateRangeValue(x, 0, bounds.size.height()));
            if (!color.is_transparent())
                output->draw_pixel(bounds.start + Vector(x, y), color.color());
        }
}
