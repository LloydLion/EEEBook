#include "gui/drawing/sub_drawers/BitmapSubDrawer.h"

BitmapSubDrawer::BitmapSubDrawer(byte *map, Bitmap::Flags flags)
{ }


void BitmapSubDrawer::draw()
{
    cord_t width = bounds.size.width();
    cord_t height = bounds.size.height();

    Bitmap bitmap  = Bitmap(_map, bounds.size, _flags);

    for (cord_t x = 0; x < width; x++)
        for (cord_t y = 0; y < height; y++)
        {
            Vector position = Vector(x, y);
            bool state = bitmap.get_pixel(position);
            PatternBag &pattern = state ? patterns.for_enabled : patterns.for_disabled;
            transparent_color_t color = pattern.perform(CoordinateRangeValue(x, 0, bounds.size.width()), CoordinateRangeValue(x, 0, bounds.size.height()));
            if (!color.is_transparent())
                output->draw_pixel(bounds.start + position, color.color());
        }
}
