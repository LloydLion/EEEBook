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

void GFX::print_text(Point start_point, cord_t width_limit, const char *text, Font font) const
{
    if (font == nullptr)
        font = get_default_font();

    Point point;
    point.x = start_point.x + _bounds.start_point.x;
    point.y = start_point.y + _bounds.start_point.y;
    _engine->print_text(point, width_limit, text, font);
}

void GFX::print_text(Point start_point, const char *text, Font font) const
{
    print_text(start_point, size().width - start_point.x, text, font);
}

void GFX::cut_and_print_text(Point start_point, char *text_buffer, Font font) const
{
    if (font == nullptr)
        font = get_default_font();

    cord_t width_limit = size().width - start_point.x;
    font->cut_text(width_limit, text_buffer);
    print_text(start_point, width_limit, text_buffer);
}

Font GFX::get_default_font() const
{
    return _engine->get_default_font();
}
