#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t thickness): _thickness(thickness)
{ }

Rectangle_::Rectangle_(): _thickness(0)
{ }

void Rectangle_::i_render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color(), _thickness);

    if (_thickness != 0)
        gfx.slice(Distance4Sides(_thickness)).fill_screen(background_color());
}

Size Rectangle_::i_min_size()
{
    return Size(_thickness + 1, _thickness + 1);
}

Size Rectangle_::i_max_size()
{
    return Size::max_size();
}
