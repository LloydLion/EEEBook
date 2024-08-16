#include "gui/GFX.h"
#include <limits>

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

void GFX::draw_rectangle(LocalBounds bounds, transparent_color_t color, cord_t thickness) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Rect;
    operation.arguments.rect.thickness = thickness;
    operation.bounds = _bounds.cast(bounds);
    operation.patterns[0].palette[0] = color;
    _queue->enqueue(operation);
}

void GFX::draw_rectangle(LocalBounds bounds, Pattern fill_pattern) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Rect;
    operation.bounds = _bounds.cast(bounds);
    operation.patterns[0] = fill_pattern;
    _queue->enqueue(operation);
}

void GFX::draw_rectangle(LocalBounds bounds, cord_t thickness, Pattern interior, Pattern boundary) const
{
    draw_rectangle(bounds, thickness, interior, boundary, boundary, boundary);
}

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

void GFX::fill_screen(transparent_color_t color) const
{
    draw_rectangle(LocalBounds(LocalVector(), size()), color);
}

void GFX::fill_screen(Pattern pattern) const
{
    draw_rectangle(LocalBounds(LocalVector(), size()), pattern);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Text;
    operation.bounds = _bounds.cast(LocalBounds(start, Size(width_limit, font.get_height())));
    operation.arguments.text.font = font.id();
    operation.arguments.text.limit = len_limit;
    operation.arguments.text.text = text;
    operation.patterns[0].palette[0] = color;
    _queue->enqueue(operation);
}

void GFX::print_text(LocalVector start, const char *text, transparent_color_t color, size_t len_limit, Font font) const
{
    print_text(start, MAX_DIMENSION_SIZE, text, color, len_limit, font);
}

void GFX::print_text(LocalVector start, cord_t width_limit, const char *text, Pattern pattern, size_t len_limit, Font font) const
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Text;
    cord_t remaining_width = constrained_cords_subtract(size().width(), start.x());
    operation.bounds = _bounds.cast(LocalBounds(start, Size(std::min(remaining_width, width_limit), font.get_height())));
    operation.arguments.text.font = font.id();
    operation.arguments.text.limit = len_limit;
    operation.arguments.text.text = text;
    operation.patterns[0] = pattern;
    _queue->enqueue(operation);
}

void GFX::draw_line(LocalVector start, LocalVector end, transparent_color_t color, cord_t thickness, cord_t length)
{
    DrawOperation operation; //TODO: GFX::draw_line(*) remove code duplication
    operation.area_type = DrawAreaType::Line;
    operation.bounds = _bounds;
    operation.patterns[0].palette[0] = color;
    operation.arguments.line.start_x = start.x();
    operation.arguments.line.start_y = start.y();
    operation.arguments.line.end_x = end.x();
    operation.arguments.line.end_y = end.y();
    operation.arguments.line.thickness = thickness;
    operation.arguments.line.length = length;
    _queue->enqueue(operation);
}

void GFX::draw_line(LocalVector start, LocalVector end, Pattern pattern, cord_t thickness, cord_t length)
{
    DrawOperation operation;
    operation.area_type = DrawAreaType::Line;
    operation.bounds = _bounds;
    operation.patterns[0] = pattern;
    operation.arguments.line.start_x = start.x();
    operation.arguments.line.start_y = start.y();
    operation.arguments.line.end_x = end.x();
    operation.arguments.line.end_y = end.y();
    operation.arguments.line.thickness = thickness;
    operation.arguments.line.length = length;
    _queue->enqueue(operation);
}

void GFX::draw_line(LocalVector start, SignedVector direction, cord_t length, Pattern pattern, cord_t thickness)
{
    SignedVector end = direction + start;
    DrawOperation operation;
    operation.area_type = DrawAreaType::Line;
    operation.bounds = _bounds;
    operation.patterns[0] = pattern;
    operation.arguments.line.start_x = start.x();
    operation.arguments.line.start_y = start.y();
    operation.arguments.line.end_x = end.x();
    operation.arguments.line.end_y = end.y();
    operation.arguments.line.thickness = thickness;
    operation.arguments.line.length = length;
    _queue->enqueue(operation);
}
