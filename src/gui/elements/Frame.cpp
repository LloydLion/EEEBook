#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t thickness, UIElement element): thickness(thickness), _element(element) { }

void Frame_::render(const GFX& pgfx)
{
    ASSUME_MARGIN(pgfx);

    gfx.fill_screen(foreground_color);

    gfx = gfx.slice(Distance4Sides(thickness));

    gfx.fill_screen(background_color);

    _element->render(gfx);
}

Size Frame_::min_size()
{
    return margin.expand(Distance4Sides(thickness).expand(_element->min_size()));
}

UIElement Frame_::get_element()
{
    return _element;
}
