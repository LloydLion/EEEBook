#include "gui/GFX.h"
#include <limits>

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

void GFX::draw_rectangle(LocalBounds bounds, color_t color) const
{
    _engine->draw_rectangle(_bounds.cast(bounds), color);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, color_t color, size_t len_limit, Font font) const
{
    if (font == nullptr)
        font = get_default_font();

    _engine->print_text(_bounds.cast(start), min(size().width - start.x, width_limit), text, len_limit, color, font);
}

void GFX::print_text(LocalVector start, const char *text, color_t color, size_t len_limit, Font font) const
{
    print_text(start, std::numeric_limits<cord_t>::max(), text, color, len_limit, font);
}

Font GFX::get_default_font() const
{
    return _engine->get_default_font();
}
