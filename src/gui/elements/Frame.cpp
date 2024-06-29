#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t thickness, UIElement element): _thickness(thickness), UIView_(element) { }

void Frame_::i_render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color(), _thickness);

    if (_thickness != 0)
        gfx.slice(Distance4Sides(_thickness)).fill_screen(background_color());

    render_child(get_element(), gfx, );
}

Size Frame_::i_min_size()
{
    return Distance4Sides(_thickness).expand(child_min_size(get_element()));
}

Size Frame_::i_max_size()
{
    //TODO: delete code repetition
    return Distance4Sides(_thickness).expand(child_max_size(get_element()));
}
