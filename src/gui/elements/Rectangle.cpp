#include "gui/elements/Rectangle.h"

Rectangle_::Rectangle_(cord_t thickness): _p_thickness(thickness)
{
    finish_initialization();
}

Rectangle_::Rectangle_(): _p_thickness(0)
{
    finish_initialization();
}

void Rectangle_::i_render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color(), _p_thickness);

    if (_p_thickness != 0)
        gfx.slice(Distance4Sides(_p_thickness)).fill_screen(background_color());
}

Size Rectangle_::i_min_size()
{
    return Size(2 * _p_thickness + 1, 2 * _p_thickness + 1);
}

Size Rectangle_::i_max_size()
{
    return Size::max_size();
}

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(Rectangle_, cord_t, thickness, Composition);