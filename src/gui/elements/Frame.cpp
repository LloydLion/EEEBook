#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t thickness, UIElement element): _thickness(thickness), _element(element) { }

void Frame_::render(const GFX& pgfx)
{
    ASSUME_MARGIN(pgfx);

    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color, _thickness);

    if (_thickness != 0)
        gfx.slice(Distance4Sides(_thickness)).fill_screen(background_color);
}

Size Frame_::min_size()
{
    return margin.expand(Distance4Sides(_thickness).expand(_element->min_size()));
}

UIElement Frame_::get_element()
{
    return _element;
}
