#include "gui/drawing/sub_drawers/TextSubDrawer.h"

TextSubDrawer::TextSubDrawer(const char *text, size_t text_limit, FontId font) : _text(text), _text_limit(text_limit), _font_id(font) 
{
    _font = DrawingContext::instance().font_engine->get_raw(_font_id);
}

inline bool TextSubDrawer::draw_char(Vector cursor, const DrawFont::Glyph *glyph)
{
    Size bitmap_size = Size(glyph->width, glyph->height);

    if (bitmap_size.width() == 0 or bitmap_size.height() == 0)
        return false;

    Vector bitmap_position = cursor + Vector(glyph->x_offset, glyph->y_offset);
    Bitmap bitmap = Bitmap(_font->bitmap + glyph->bitmap_offset, bitmap_size, _font->bitmap_flags);

    bool was_clipped = false;

    for (uint8_t in_bitmap_x = 0; in_bitmap_x < bitmap_size.width(); in_bitmap_x++)
    {
        for (uint8_t in_bitmap_y = 0; in_bitmap_y < bitmap_size.height(); in_bitmap_y++)
        {
            Vector in_bitmap = Vector(in_bitmap_x, in_bitmap_y);
            Vector current = bitmap_position + in_bitmap;

            bool state = bitmap.get_pixel(in_bitmap);

            if (state)
            {
                if (current.x() < bounds.size.width())
                {
                    CoordinateRangeValue width_range = CoordinateRangeValue(current.x(), 0, _text_size.width());
                    CoordinateRangeValue height_range = CoordinateRangeValue(current.y(), 0, _text_size.height());

                    transparent_color_t color = glyph_pattern.perform(width_range, height_range);
                    if (!color.is_transparent())
                        output->draw_pixel(bounds.start + current, color);
                }
                else
                    was_clipped = true;
            }
        }
    }

    return was_clipped;
}

void TextSubDrawer::draw()
{
    /*Text size*/
    Font font_origin = Font(_font_id);
    _text_size = Size(font_origin.get_string_width(_text), font_origin.get_height());

    /*Text default offset*/
    Vector cursor = bounds.start;
    cursor = cursor + Vector(0, DrawingContext::instance().font_engine->get_y_offset(_font_id));

    /*Drawing background */
    for (size_t i = 0; i < _text_size.width(); i++)
        for (size_t j = 0; j < _text_size.height(); j++)
        {
            CoordinateRangeValue width_range = CoordinateRangeValue(i, 0, _text_size.width());
            CoordinateRangeValue height_range = CoordinateRangeValue(j, 0, _text_size.height());
            transparent_color_t color = background_pattern.perform(width_range, height_range);
            if(color.is_transparent())
                continue;
            output->draw_pixel(bounds.start + Vector(i,j), color);
        }

    /*Drawing text*/
    size_t i = 0;
    while (char c = *_text++)
    {

        uint8_t first = _font->first;
        if (i++ == _text_limit)
            break;

        if ((c >= first) && (c <= _font->last))
        {
            const DrawFont::Glyph *glyph = _font->glyphs + (c - first);

            bool was_clipped = draw_char(cursor, glyph);

            if (was_clipped)
                break;

            cursor = cursor + Vector(glyph->x_advance, 0);
        }
    }
}