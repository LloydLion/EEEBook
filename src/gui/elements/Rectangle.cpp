#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t thickness): _thickness(thickness)
{ }

Rectangle_::Rectangle_(): _thickness(0)
{ }

void Rectangle_::render(const GFX& pgfx)
{
    ASSUME_MARGIN(pgfx);

    gfx.fill_screen(background_color);

    if (_thickness != 0)
        gfx.slice(Distance4Sides(_thickness)).fill_screen(foreground_color);
}

Size Rectangle_::min_size()
{
    return margin.expand(Size(_thickness + 1, _thickness + 1));
}
