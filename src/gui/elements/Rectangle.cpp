#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t margin, cord_t thickness) : _margin(margin), _thickness(thickness)
{ }

void Rectangle_::render(const GFX& gfx)
{
    gfx.draw_rectangle(create_bounds(create_point(_margin, _margin), create_point(gfx.size().width - _margin, gfx.size().height - _margin)));
}

Size Rectangle_::min_size() {return create_size(2 * (_margin + _thickness), 2 * (_margin + _thickness));}