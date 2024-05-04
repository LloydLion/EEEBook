#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t margin, cord_t thickness) : _margin(margin), _thickness(thickness)
{ }

Rectangle_::Rectangle_(cord_t margin) : _margin(margin), _thickness(-1)
{ }

void Rectangle_::render(const GFX& gfx)
{
    cord_t w = gfx.size().width;
    cord_t h = gfx.size().height;

    cord_t o_size = _margin;
    gfx.draw_rectangle(Bounds(Vector(o_size, o_size), Vector(w - o_size, h - o_size)), background_color);

    if (_thickness != -1)
    {
        cord_t i_size = _margin + _thickness;
        gfx.draw_rectangle(Bounds(Vector(i_size, i_size), Vector(w - i_size, h - i_size)), foreground_color);
    }
}

Size Rectangle_::min_size()
{
    cord_t min_size = 2 * (_margin + _thickness) + 1;
    return Size(min_size, min_size);
}
