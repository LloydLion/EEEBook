#include "gui/drawing/UniversalDrawer.h"
#include "gui/drawing/sub_drawers/RectSubDrawer.h"
#include "gui/drawing/sub_drawers/LineSubDrawer.h"
#include <stdexcept>

inline void draw_pixel_using_pattern(const DrawOperation &operation, cord_t x, cord_t y, size_t pattern_index, Screen screen)
{
    transparent_color_t color = operation.patterns[pattern_index].perform(CoordinateRangeValue(x, 0, operation.bounds.size.width()), CoordinateRangeValue(y, 0, operation.bounds.size.height()));
    if (!color.is_transparent())
        screen->draw_pixel(operation.bounds.start + Vector(x, y), color);
}

inline void draw_bitmap(const DrawOperation &operation, Screen screen);
inline void draw_text(const DrawOperation &operation, Screen screen);

UniversalDrawer_::UniversalDrawer_(Screen screen) : _screen(screen) {}

void UniversalDrawer_::draw(DrawOperationQueue queue)
{
    for (size_t i = 0; i < queue->size(); i++)
    {
        auto operation = queue->get(i);

        draw(operation);
    }
}

void UniversalDrawer_::draw(const DrawOperation &operation)
{
    DrawAreaType type = operation.area_type;
    switch (type)
    {
    case DrawAreaType::Rect:
        {
            RectSubDrawer drawer(operation.arguments.rect.thickness);
            drawer.output = _screen;
            drawer.bounds = operation.bounds;

            drawer.patterns.interior = operation.patterns[0];
            drawer.patterns.horizontal_strips = operation.patterns[1];
            drawer.patterns.vertical_strips = operation.patterns[2];
            drawer.patterns.corners = operation.patterns[3];
            
            drawer.draw();
        }
        break;
    case DrawAreaType::Ellipse:

        break;
    case DrawAreaType::Line:
        {
            auto args = operation.arguments.line;
            LineSubDrawer drawer(
                Vector(args.start_x, args.start_y),
                SignedVector(args.end_x, args.end_y),
                args.length, args.thickness
            );
            
            drawer.output = _screen;
            drawer.bounds = operation.bounds;

            drawer.pattern = operation.patterns[0];

            drawer.draw();
        }
        break;
    case DrawAreaType::Text: // TODO
        draw_text(operation, _screen);
        break;
    case DrawAreaType::Bitmap:
        draw_bitmap(operation, _screen);
        break;
    }
}

inline bool get_bitmap_pixel(
    byte *bitmap, Bitmap::Flags flags,
    cord_t x, cord_t y,
    cord_t width, cord_t height)
{
    size_t idx = flags & Bitmap::YPrimary ? y + x * height : x + y * width;
    size_t byte_idx = idx / 8;
    size_t bit_idx = flags & Bitmap::BigEndian ? 7 - idx % 8 : idx % 8;
    bool state = bitmap[byte_idx] & (1 << bit_idx);
    if (flags & Bitmap::Inverted)
        state = !state;
    return state;
}

inline void draw_bitmap(const DrawOperation &operation, Screen screen)
{
    cord_t width = operation.bounds.size.width();
    cord_t height = operation.bounds.size.height();
    auto operation_args = operation.arguments.bitmap;
    for (cord_t x = 0; x < width; x++)
        for (cord_t y = 0; y < height; y++)
        {
            bool state = get_bitmap_pixel(
                operation.arguments.bitmap.map, operation.arguments.bitmap.flags,
                x, y, width, height);
            draw_pixel_using_pattern(operation, x, y, state ? 1 : 0, screen);
        }
}

inline bool draw_char(
    const DrawFont::Glyph *glyph,
    const DrawFont *font,
    const DrawOperation &operation,
    Screen screen, Vector cursor, cord_t x_limit)
{
    Size bitmap_size = Size(glyph->width, glyph->height);

    if (bitmap_size.width() == 0 or bitmap_size.height() == 0)
        return false;

    Vector bitmap_position = cursor + Vector(glyph->x_offset, glyph->y_offset);
    byte *bitmap = font->bitmap + glyph->bitmap_offset;
    bool was_clipped = false;

    for (uint8_t in_bitmap_x = 0; in_bitmap_x < bitmap_size.width(); in_bitmap_x++)
    {
        for (uint8_t in_bitmap_y = 0; in_bitmap_y < bitmap_size.height(); in_bitmap_y++)
        {
            Vector current = bitmap_position + Vector(in_bitmap_x, in_bitmap_y);

            bool state = get_bitmap_pixel(bitmap, font->bitmap_flags, in_bitmap_x, in_bitmap_y, bitmap_size.width(), bitmap_size.height());

            if (state)
            {
                if (current.x() <= x_limit)
                    draw_pixel_using_pattern(operation, current.x(), current.y(), 0, screen);
                else
                    was_clipped = true;
            }
        }
    }

    return was_clipped;
}

inline void draw_text(const DrawOperation &operation, Screen screen)
{
    const char *text = operation.arguments.text.text;
    FontId font_id = operation.arguments.text.font;
    const DrawFont *font = DrawingContext::instance().font_engine->get_raw(font_id);

    Vector cursor = operation.bounds.start;
    cursor = cursor + Vector(0, DrawingContext::instance().font_engine->get_y_offset(font_id));

    size_t i = 0;
    while (char c = *text++)
    {
        if (i++ >= operation.arguments.text.limit)
            break;

        uint8_t first = font->first;
        if ((c >= first) && (c <= font->last))
        {
            const DrawFont::Glyph *glyph = font->glyphs + (c - first);

            bool was_clipped = draw_char(glyph, font, operation, screen, cursor, operation.bounds.end().x());

            if (was_clipped)
                break;

            cursor = cursor + Vector(glyph->x_advance, 0);
        }
    }
}
