#include "gui/GFX.h"

GFX::GFX(GraphicsEngine engine, Bounds bounds): _engine(engine), _bounds(bounds)
{

}

GFX::GFX(GraphicsEngine engine, Size display_size):
    _engine(engine), _bounds(create_bounds(create_point(0, 0), display_size))
{

}

Size GFX::size() const
{
    return _bounds.size;
}

GFX GFX::slice(Bounds local_bounds) const
{
    return GFX(_engine, cast(_bounds, local_bounds));
}

void GFX::draw_rectangle(Bounds bounds) const
{
    _engine->draw_rectangle(cast(_bounds, bounds));
}

void GFX::print_text(Point start_point, char* text) const
{
    Point point;
    point.x = start_point.x + _bounds.start_point.x;
    point.y = start_point.y + _bounds.start_point.y;
    _engine->print_text(point,text);
}
