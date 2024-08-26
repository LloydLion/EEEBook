#include"gui/drawing/Bitmap.h"

bool Bitmap::get_pixel(Vector position)
{
    cord_t width = size.width();
    cord_t height = size.height();

    size_t idx = flags & Bitmap::YPrimary ? position.y() + position.x() * height : position.x() + position.y() * width;
    size_t byte_idx = idx / 8;
    size_t bit_idx = flags & Bitmap::BigEndian ? 7 - idx % 8 : idx % 8;
    bool state = data[byte_idx] & (1 << bit_idx);
    if (flags & Bitmap::Inverted)
        state = !state;
    return state;
}