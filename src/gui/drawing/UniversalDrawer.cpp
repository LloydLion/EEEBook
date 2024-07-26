#include "gui/drawing/UniversalDrawer.h"
#include <stdexcept>

inline void draw_pixel_using_pattern(const DrawOperation &operation, cord_t x, cord_t y, size_t pattern_index, Screen screen)
{
    transparent_color_t color = operation.patterns[0].perform(x, y, operation.bounds.size.width(), operation.bounds.size.height());
    if (!color.is_transparent())
        screen->draw_pixel(operation.bounds.start + Vector(x, y), color);
}

inline void draw_rect(const DrawOperation &operation, Screen screen);
inline void draw_bitmap(const DrawOperation &operation, Screen screen);
inline void draw_text(const DrawOperation &operation, Screen screen);


UniversalDrawer_::UniversalDrawer_(Screen screen): _screen(screen) {}

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
        draw_rect(operation, _screen);
        break;
    case DrawAreaType::Ellipse:

        break;
    case DrawAreaType::Line:

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
    cord_t width, cord_t height
)
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

inline void draw_rect(const DrawOperation &operation, Screen screen)
{
    cord_t thickness = operation.arguments.rect.thickness;
    cord_t width = operation.bounds.size.width();
    cord_t height = operation.bounds.size.height();
    if (thickness == 0)
    {
        for (cord_t x = 0; x < width; x++)
            for (cord_t y = 0; y < height; y++)
            {
                draw_pixel_using_pattern(operation, x, y, 0, screen);
            }
        return;
    }

    // corners
    for (cord_t x = 0; x < thickness; x++)
        for (cord_t y = 0; y < thickness; y++)
        {
            draw_pixel_using_pattern(operation, x, y, 3, screen);
            draw_pixel_using_pattern(operation, x + width - thickness - 1, y, 3, screen);
            draw_pixel_using_pattern(operation, x, y + height - thickness - 1, 3, screen);
            draw_pixel_using_pattern(operation, x + width - thickness - 1, y + height - thickness - 1, 3, screen);
        }

    // vertical strips
    for (cord_t x = 0; x < thickness; x++)
        for (cord_t y = thickness; y < height - thickness; y++)
        {
            draw_pixel_using_pattern(operation, x, y, 2, screen);
            draw_pixel_using_pattern(operation, x + width - thickness - 1, y, 2, screen);
        }

    // horizontal strips
    for (cord_t x = thickness; x < width - thickness; x++)
        for (cord_t y = 0; y < thickness; y++)
        {
            draw_pixel_using_pattern(operation, x, y, 1, screen);
            draw_pixel_using_pattern(operation, x, y + height - thickness - 1, 1, screen);
        }

    // interior
    for (cord_t x = thickness; x < width - thickness; x++)
        for (cord_t y = thickness; y < height - thickness; y++)
            draw_pixel_using_pattern(operation, x, y, 0, screen);
}

inline bool draw_char(
    const DrawFont::Glyph *glyph,
    const DrawFont *font,
    const DrawOperation &operation,
    Screen screen,
    cord_t cursor_y, cord_t cursor_x, cord_t x_limit)
{

    bool was_clipped = false;

    cord_t width = glyph->width;
    cord_t height = glyph->height;

    if ((width > 0) && (height > 0))
    {
        cord_t xo = glyph->x_offset;
        cord_t yo = glyph->y_offset;
        
        byte *bitmap = font->bitmap;
        uint16_t bo = glyph->bitmap_offset;

        uint8_t bits = 0, bit = 0;

        for (uint8_t yy = 0; yy < height; yy++)
        {
            for (uint8_t xx = 0; xx < width; xx++)
            {
                if (!(bit++ & 7))
                    bits = bitmap[bo++];

                if (bits & 0x80)
                    if (cursor_x + xo + xx <= x_limit)
                        draw_pixel_using_pattern(operation,
                            cursor_x + xo + xx,
                            cursor_y + yo + yy, 0, screen);
                    else was_clipped = true;

                bits <<= 1;
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

    uint16_t cursor_x = operation.bounds.start.x();
    uint16_t pos_y = operation.bounds.start.y() + DrawingContext::instance().font_engine->get_y_offset(font_id);

    size_t i = 0;
    while (char c = *text++)
    {
        if (i++ >= operation.arguments.text.limit) break;

        uint8_t first = font->first;
        if ((c >= first) && (c <= font->last))
        {
            const DrawFont::Glyph *glyph = font->glyphs + (c - first);

            bool was_clipped = draw_char(glyph, font, operation, screen, pos_y, cursor_x, operation.bounds.end().x());

            if (was_clipped)
                break;

            cursor_x += glyph->x_advance;
        }
    }
}
