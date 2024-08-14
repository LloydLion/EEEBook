#include "gui/drawing/UniversalDrawer.h"
#include <stdexcept>
#include "platform/stdout.h"

inline void draw_pixel_using_pattern(const DrawOperation &operation, cord_t x, cord_t y, size_t pattern_index, Screen screen)
{
    transparent_color_t color = operation.patterns[0].perform(x, y, operation.bounds.size.width(), operation.bounds.size.height());
    if (!color.is_transparent())
        screen->draw_pixel(operation.bounds.start + Vector(x, y), color);
}

inline void draw_rect(const DrawOperation &operation, Screen screen);
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

void execute_along_ray(Vector start, Vector head, Size screen_size, cord_t limit, void (*p)(void*, Vector), void* parameter)
{
    cord_t x0 = start.x();
    cord_t x1 = head.x();
    cord_t y0 = start.y();
    cord_t y1 = head.y();

    bool steep = abs_cords_subtract(y1, y0) > abs_cords_subtract(x1, x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    cord_t dx = x1 - x0;
    cord_t dy = abs_cords_subtract(y1, y0);

    s_cord_t error = dx / 2;
    s_cord_t y_step = (y0 < y1) ? 1 : -1;

    cord_t y = y0;
    cord_t counter = 0;

    for (int x = x0; true; x++)
    {
        Vector physical_position = Vector(steep ? y : x, steep ? x : y);
        if (physical_position.x() >= screen_size.width() or physical_position.y() >= screen_size.height())
            return;
        p(parameter, physical_position);
        counter++;
        if (counter >= limit)
            return;
        error -= dy;
        if (error < 0)
        {
            y += y_step;
            error += dx;
        }
    }
}

void draw_line(Vector start, Vector end, Pattern pattern, Screen screen, cord_t b_cord, cord_t b_size)
{
    if (end.x() < start.x())
    {
        std::swap(start,end);
    }

    Vector Dv = (end - start).remove_sings();

    s_cord_t y = 0;
    cord_t x = 0;

    bool step = Dv.y() < 0 ? -1 : 1;

    bool swap = false;
    if (Dv.y() / Dv.x() > 1)
    {
        start = Vector(start.y(), start.x());
        end = Vector(end.y(), end.x());
        swap = true;
    }

    s_cord_t P = 2 * Dv.x() - Dv.y();
    cord_t total_iterations = end.x() - start.x() + 1;
    while (start.x() + x <= end.x())
    {
        color_t color = pattern.perform(x, b_cord, total_iterations, b_size);
        screen->draw_pixel(swap ? Vector(x + start.y() ,y + start.x()) : start + Vector(x, y), color);
        x++;
        if (P < 0)
            P += 2 * Dv.y();
        else
        {
            P += 2 * Dv.y() - 2 * Dv.x();
            y += step;
        }
    }
}

SignedVector raycast(Vector start, SignedVector direction, cord_t length)
{
    SignedVector begin = start;
    SignedVector imaginated_end = direction + begin;

    Axis axis = std::abs(imaginated_end.y() - begin.y()) > std::abs(imaginated_end.x() - begin.x()) ? AxisY : AxisX;

    cord_t dp = abs_cords_subtract(imaginated_end[axis], begin[axis]);
    cord_t ds = abs_cords_subtract(imaginated_end[~axis], begin[~axis]);

    s_cord_t p_direction = (begin[axis] < imaginated_end[axis]) ? 1 : -1;
    s_cord_t s_direction = (begin[~axis] < imaginated_end[~axis]) ? 1 : -1;
    s_cord_t error = dp / 2;

    s_cord_t offset_s = 0;

    cord_t path_counter = 0;

    for (s_cord_t offset_p = 0; true; offset_p += p_direction)
    {
        Vector physical_position = Vector(offset_p + begin[axis], offset_s + begin[~axis], axis);

        if (path_counter >= length)
            return SignedVector(offset_p, offset_s, axis);

        path_counter++;
        error -= ds;
        if (error < 0)
        {
            offset_s += s_direction;
            error += dp;
        }
    }
}

void draw_subline(Vector start, SignedVector direction, cord_t length, Screen output, int cornered_mode)
{
    SignedVector begin = start;
    SignedVector imaginated_end = direction + begin;

    Axis axis = std::abs(imaginated_end.y() - begin.y()) > std::abs(imaginated_end.x() - begin.x()) ? AxisY : AxisX;

    cord_t dp = abs_cords_subtract(imaginated_end[axis], begin[axis]);
    cord_t ds = abs_cords_subtract(imaginated_end[~axis], begin[~axis]);

    s_cord_t p_direction = (begin[axis] < imaginated_end[axis]) ? 1 : -1;
    s_cord_t s_direction = (begin[~axis] < imaginated_end[~axis]) ? 1 : -1;
    s_cord_t error = dp / 2;

    cord_t s_base = begin[~axis];
    cord_t cursor_s = s_base;

    cord_t path_counter = 0;

    Size viewport = output->full_viewport_size();

    bool error_corection_last_time = false;

    for (s_cord_t p = begin[axis]; true; p += p_direction)
    {
        Vector physical_position = Vector(p, cursor_s, axis);

        if (path_counter >= length)
            return;

        if (physical_position.x() < viewport.width() and physical_position.y() < viewport.height())
            output->draw_pixel(physical_position, ColorMap::Red);

        if (error_corection_last_time and cornered_mode == -1) //Post corner
        {
            physical_position = physical_position.with(physical_position[~axis] - s_direction, ~axis);
            if (physical_position.x() >= viewport.width() or physical_position.y() >= viewport.height())
                continue;
            output->draw_pixel(physical_position, ColorMap::Green);
        }

        path_counter++;

        error -= ds;
        error_corection_last_time = false;
        if (error < 0)
        {
            error_corection_last_time = true;
            cursor_s += s_direction;
            error += dp;

            if (cornered_mode == 1) //Pre corner
            {
                physical_position = Vector(p, cursor_s, axis);
                if (physical_position.x() >= viewport.width() or physical_position.y() >= viewport.height())
                    continue;
                output->draw_pixel(physical_position, ColorMap::Green);
            }
        }
    }
}

void draw_thick_line(Vector start, Vector end, cord_t thinkness, Screen output)
{
    Axis axis = abs_cords_subtract(end.y(), start.y()) > abs_cords_subtract(end.x(), start.x()) ? AxisY : AxisX;

    if (start[axis] > end[axis])
        std::swap(start, end);

    cord_t dp = end[axis] - start[axis];
    cord_t ds = abs_cords_subtract(end[~axis], start[~axis]);

    s_cord_t s_direction = (start[~axis] < end[~axis]) ? 1 : -1;
    s_cord_t error = dp / 2;

    SignedVector perpendicular = (end - start).rotate_clockwise();

    SignedVector offset = raycast(start, perpendicular.rotate_180(), thinkness / 2);

    cord_t cursor_s = start[~axis] + offset[~axis];

    bool should_draw_next_subline_in_cornred_mode = false;

    bool magic_characteristic = (s_direction == 1) == (axis == AxisX);

    //std_printf("s_direction: %d; primary axis: %d -> magic_characteristic: %d\n", s_direction, axis.index(), magic_characteristic);

    for (s_cord_t p_offset = 0; p_offset <= dp; p_offset++)
    {
        Vector cursor = Vector(start[axis] + offset[axis] + p_offset, cursor_s, axis);

        draw_subline(cursor, perpendicular, thinkness, output, should_draw_next_subline_in_cornred_mode ? (magic_characteristic ? 1 : -1) : 0);
        should_draw_next_subline_in_cornred_mode = false;

        output->draw_pixel(cursor, ColorMap::Black);
        output->draw_pixel(Vector(start[axis] + p_offset, cursor_s - offset[~axis], axis), ColorMap::Blue);

        error -= ds;
        if (error < 0)
        {
            cursor_s += s_direction;
            error += dp;
            should_draw_next_subline_in_cornred_mode = true;
        }
    }
}
