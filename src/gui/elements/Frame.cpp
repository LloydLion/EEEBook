#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t margin, cord_t thickness, UIElement element): _margin(margin), _thickness(thickness), _element(element) {}

void Frame_::render(const GFX& gfx)
{
    cord_t offset = 2 * _margin + _thickness;
    Bounds inner_bounds;
    inner_bounds = create_bounds(create_point( offset, offset), create_point(gfx.size().width - offset, gfx.size().height - offset));
    //Black rectangle
    gfx.draw_rectangle(create_bounds(create_point(_margin, _margin), create_point(gfx.size().width - _margin, gfx.size().height - _margin)));
    //White rectangle
    gfx.draw_rectangle(create_bounds(create_point(_margin + _thickness, _margin + _thickness), create_point(gfx.size().width -_margin - _thickness, gfx.size().height -_margin - _thickness)));
    GFX new_gfx = gfx.slice(inner_bounds);
    _element->render(new_gfx);
}
Size Frame_::min_size()
{
    cord_t offset = 2 * _margin + _thickness;
    return create_size(_element->min_size().width + offset, _element->min_size().height + offset);
}