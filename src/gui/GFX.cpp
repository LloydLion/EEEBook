#include "gui/GFX.h"
#include <limits>

inline Pattern fill_with_color(transparent_color_t color)
{
    Pattern a;
    a.palette[0] = color;
    return a;
}

GFX::GFX(DrawOperationQueue queue, Bounds bounds) : _queue(queue), _bounds(bounds)
{
}

GFX::GFX(DrawOperationQueue queue, Size display_size) : _queue(queue), _bounds(Vector(0, 0), display_size)
{
}

Size GFX::size() const
{
    return _bounds.size;
}

GFX GFX::slice(LocalBounds local_bounds) const
{
    return GFX(_queue, _bounds.cast(local_bounds));
}

GFX GFX::slice(Distance4Sides distances) const
{
    return GFX(_queue, distances.cast(_bounds));
}


#pragma region Rectangle

void GFX::draw_rectangle(LocalBounds bounds, cord_t thickness,
    Pattern interior, Pattern corners, Pattern vertical, Pattern horizontal) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Rect;
    operation.bounds = _bounds.cast(bounds);
    operation.arguments.rect.thickness = thickness;
    operation.patterns[0] = interior;
    operation.patterns[1] = horizontal;
    operation.patterns[2] = vertical;
    operation.patterns[3] = corners;
    _queue->enqueue(operation);
}

void GFX::draw_rectangle(LocalBounds bounds, cord_t thickness, Pattern interior, Pattern boundary) const
{
    draw_rectangle(bounds, thickness, interior, boundary, boundary, boundary);
}

void GFX::draw_rectangle(LocalBounds bounds, transparent_color_t color, cord_t thickness) const
{
    Pattern colored_pattern = fill_with_color(color);
    if (thickness == 0)
        draw_rectangle(bounds, thickness, colored_pattern, Pattern(), Pattern(), Pattern());
    else
        draw_rectangle(bounds, thickness, fill_with_color(transparent_color_t()), colored_pattern, colored_pattern, colored_pattern);
}

void GFX::draw_rectangle(LocalBounds bounds, Pattern fill_pattern) const
{
    draw_rectangle(bounds, 0, fill_pattern, Pattern(), Pattern(), Pattern());
}

void GFX::fill_screen(transparent_color_t color) const
{
    draw_rectangle(LocalBounds(LocalVector(), size()), color);
}

void GFX::fill_screen(Pattern pattern) const
{
    draw_rectangle(LocalBounds(LocalVector(), size()), pattern);
}

#pragma endregion

#pragma region Text

void GFX::print_text(LocalVector start, cord_t width_limit, Pattern background_pattern, const char *text, Pattern pattern, size_t len_limit, Font font) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Text;
    cord_t remaining_width = constrained_cords_subtract(size().width(), start.x());
    operation.bounds = _bounds.cast(LocalBounds(start, Size(std::min(remaining_width, width_limit), font.get_height())));
    operation.arguments.text.text = text;
    operation.arguments.text.limit = len_limit;
    operation.arguments.text.font = font.id();
    operation.patterns[0] = pattern;
    operation.patterns[1] = background_pattern;
    _queue->enqueue(operation);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, width_limit, fill_with_color(transparent_color_t()), text, fill_with_color(color), len_limit, font);
}

void GFX::print_text(LocalVector start, cord_t width_limit, transparent_color_t background_color, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, width_limit, fill_with_color(background_color), text, fill_with_color(color), len_limit, font);
}

void GFX::print_text(LocalVector start, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, MAX_DIMENSION_SIZE, fill_with_color(transparent_color_t()), text, fill_with_color(color), len_limit, font);
}

void GFX::print_text(LocalVector start, transparent_color_t background_color, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, MAX_DIMENSION_SIZE, fill_with_color(background_color), text, fill_with_color(color), len_limit, font);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, Pattern pattern, size_t len_limit, Font font) const
{
    print_text(start, width_limit, fill_with_color(transparent_color_t()), text, pattern, len_limit, font);
}

#pragma endregion

#pragma region Line

void __GFX__draw_line(DrawOperationQueue queue, Bounds bounds, LocalVector start, SignedVector end, cord_t length, Pattern pattern, cord_t thickness)
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Line;
    operation.bounds = bounds;
    operation.patterns[0] = pattern;
    operation.arguments.line.start_x = start.x();
    operation.arguments.line.start_y = start.y();
    operation.arguments.line.end_x = end.x();
    operation.arguments.line.end_y = end.y();
    operation.arguments.line.thickness = thickness;
    operation.arguments.line.length = length;
    queue->enqueue(operation);
}

void GFX::draw_line(LocalVector start, SignedVector direction, cord_t length, Pattern pattern, cord_t thickness)
{
    SignedVector end = direction + start;
    __GFX__draw_line(_queue, _bounds, start, end, length, pattern, thickness);
}

void GFX::draw_line(LocalVector start, LocalVector end, transparent_color_t color, cord_t thickness, cord_t length)
{
    __GFX__draw_line(_queue, _bounds, start, end, length, fill_with_color(color), thickness);
}

void GFX::draw_line(LocalVector start, LocalVector end, Pattern pattern, cord_t thickness, cord_t length)
{
    __GFX__draw_line(_queue, _bounds, start, end, length, pattern, thickness);
}

#pragma endregion

#pragma region Bitmap

void GFX::draw_bitmap(LocalVector start, Bitmap bitmap, Pattern on_pattern, Pattern off_pattern)
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Bitmap;
    operation.bounds = _bounds.cast(LocalBounds(start, bitmap.size)); //TODO: imagine something with oversized bitmaps
    operation.patterns[0] = on_pattern;
    operation.patterns[1] = off_pattern;
    operation.arguments.bitmap.flags = bitmap.flags;
    operation.arguments.bitmap.map = bitmap.data;
    _queue->enqueue(operation);
}

void GFX::draw_bitmap(LocalVector start, Bitmap bitmap, transparent_color_t on_color, transparent_color_t off_color)
{
    draw_bitmap(start, bitmap, fill_with_color(on_color), fill_with_color(off_color));
}

#pragma endregion
