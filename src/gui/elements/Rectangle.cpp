#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t margin, cord_t thickness) : _margin(margin), _thickness(thickness)
{ }

void Rectangle_::render(const GFX& gfx)
{
    if (_is_drawn and !_is_moved)
        return;

    gfx.draw_rectangle(create_bounds(create_point(_margin, _margin), create_point(gfx.size().width - _margin, gfx.size().height - _margin)));
    
    _is_drawn = true;
    _is_moved = false;
}

bool Rectangle_::is_updated() { return !_is_drawn; }

Size Rectangle_::min_size() {return create_size(2 * (_margin + _thickness), 2 * (_margin + _thickness));}