#include "gui/GFX.h"
#include <limits>

#define RETURN_IF_COLOR_TRANSPERENT if (color.is_transparent) return

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

GFX GFX::slice(Distance4Sides distances) const
{
    return GFX(_engine, distances.cast(_bounds));
}

void GFX::draw_rectangle(LocalBounds bounds, transparent_color_t color, cord_t thickness) const
{
    RETURN_IF_COLOR_TRANSPERENT;
    _engine->draw_rectangle(_bounds.cast(bounds), color.color, thickness);
}

void GFX::fill_screen(transparent_color_t color) const
{
    RETURN_IF_COLOR_TRANSPERENT;
    _engine->draw_rectangle(_bounds, color.color, 0);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    RETURN_IF_COLOR_TRANSPERENT;
    if (font == nullptr)
        font = get_default_font();

    _engine->print_text(_bounds.cast(start), min(size().width - start.x, width_limit), text, len_limit, color.color, font);
}

void GFX::print_text(LocalVector start, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, std::numeric_limits<cord_t>::max(), text, color, len_limit, font);
}

Font GFX::get_default_font() const
{
    return _engine->get_default_font();
}
