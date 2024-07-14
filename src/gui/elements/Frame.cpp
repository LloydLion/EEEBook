#include "gui/elements/Frame.h"
#define THICKNESS_DISTANCES Distance4Sides(_p_thickness)

Frame_::Frame_(cord_t thickness, UIElement element): _p_thickness(thickness), UIView_(element)
{
    finish_initialization();
}

void Frame_::thickness(cord_t value)
{
    _p_thickness = value;
}

void Frame_::i_render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), foreground_color(), _p_thickness);

    if (_p_thickness != 0)
    {
        gfx.slice(THICKNESS_DISTANCES).fill_screen(background_color());
        render_child(get_element(), gfx.slice(THICKNESS_DISTANCES));
    }
    else
    {
        render_child(get_element(), gfx);
    }
}

Size Frame_::i_min_size()
{
    return THICKNESS_DISTANCES.expand(child_min_size(get_element()));
}

Size Frame_::i_max_size()
{
    return THICKNESS_DISTANCES.expand(child_max_size(get_element()));
}
