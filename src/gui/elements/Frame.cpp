#include "gui/elements/Frame.h"

Frame_::Frame_(cord_t margin, cord_t thickness, UIElement element): _margin(margin), _thickness(thickness), _element(element) {}

void Frame_::render(const GFX& gfx)
{
    cord_t offset = 2 * _margin + _thickness;
    Bounds inner_bounds = Bounds(Vector( offset, offset), Vector(gfx.size().width - offset, gfx.size().height - offset));
    //Black rectangle
    gfx.draw_rectangle(Bounds(Vector(_margin, _margin), Vector(gfx.size().width - _margin, gfx.size().height - _margin)), foreground_color);
    //White rectangle
    gfx.draw_rectangle(Bounds(Vector(_margin + _thickness, _margin + _thickness), Vector(gfx.size().width -_margin - _thickness, gfx.size().height -_margin - _thickness)), foreground_color);
    GFX new_gfx = gfx.slice(inner_bounds);
    _element->render(new_gfx);
}

std::vector<UIElement>::iterator Frame_::list_children()
{
    //TODO: list children
}

size_t Frame_::count_children() { return 1; };

Size Frame_::min_size()
{
    cord_t offset = 2 * _margin + _thickness;
    return Size(_element->min_size().width + offset, _element->min_size().height + offset);
}
