#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t thickness, UIElement element): _thickness(thickness), UIView_(element) { }

void Frame_::i_render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color(), _thickness);

    if (_thickness != 0)
        gfx.slice(Distance4Sides(_thickness)).fill_screen(background_color());
}

Size Frame_::i_min_size()
{
    return Distance4Sides(_thickness).expand(get_element()->min_size());
}

Size Frame_::i_max_size()
{
    //TODO: delete code repetition
    return Distance4Sides(_thickness).safe_expand(get_element()->max_size());
}
