#include "gui/GFX.h"

GFX::GFX(GraphicsEngine engine, Bounds bounds): _engine(engine), _bounds(bounds)
{

}

GFX::GFX(GraphicsEngine engine, Size display_size):
    _engine(engine), _bounds(Vector(0, 0), display_size)
{

}

Size GFX::size() const
{
    return _bounds.size;
}

GFX GFX::slice(LocalBounds local_bounds) const
{
    return GFX(_engine, _bounds.cast(local_bounds));
}

void GFX::draw_rectangle(LocalBounds bounds) const
{
    _engine->draw_rectangle(_bounds.cast(bounds));
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, Font font) const
{
    if (font == nullptr)
        font = get_default_font();

    _engine->print_text(_bounds.cast(start), width_limit, text, font);
}

void GFX::print_text(LocalVector start, const char *text, Font font) const
{
    print_text(start, size().width - start.x, text, font);
}

void GFX::cut_and_print_text(Vector start_point, char *text_buffer, Font font) const
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
