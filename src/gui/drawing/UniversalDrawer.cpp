#include "gui/drawing/UniversalDrawer.h"
#include <stdexcept>

inline void draw_pixel_using_pattern(const DrawOperation &operation, cord_t x, cord_t y, size_t pattern_index, Screen screen)
{
    transparent_color_t color = operation.patterns[pattern_index].perform(x, y, operation.bounds.size.width(), operation.bounds.size.height());
    if (!color.is_transparent())
        screen->draw_pixel(operation.bounds.start + Vector(x, y), color);
}

inline void draw_rect(const DrawOperation &operation, Screen screen);
inline void draw_bitmap(const DrawOperation &operation, Screen screen);
inline void draw_text(const DrawOperation &operation, Screen screen);
inline void draw_thick_line(Vector start, SignedVector end, cord_t length, cord_t thickness, Bounds limits, Pattern pattern, Screen output);

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
        {
            auto arguments = operation.arguments.line;
            draw_thick_line(
                Vector(arguments.start_x, arguments.start_y),
                SignedVector(arguments.end_x, arguments.end_y),
                arguments.length, arguments.thickness, operation.bounds, operation.patterns[0], _screen
            );
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

class BresenhamAlgorithm
{
public:
    Axis axis = AxisX;
    cord_t delta_p;
    cord_t delta_s;
    s_cord_t p_direction;
    s_cord_t s_direction;
    s_cord_t error;
    s_cord_t s_offset = 0;

    BresenhamAlgorithm(SignedVector direction)
    {
        Vector abs_delta = direction.absolute();

        axis = abs_delta.y() > abs_delta.x() ? AxisY : AxisX;

        delta_p = abs_delta[axis];
        error = delta_p / 2; 
        delta_s = abs_delta[~axis];

        p_direction = direction[axis] > 0 ? 1 : -1;
        s_direction = direction[~axis] > 0 ? 1 : -1;
    }

    void step()
    {
        error -= delta_s;
    }

    bool should_compensate_error()
    {
        return error < 0;
    }

    void compensate_error()
    {
        s_offset += s_direction;
        error += delta_p;
    }

    Vector position(s_cord_t p_offset, Vector start)
    {
        return Vector(start[axis] + p_offset, start[~axis] + s_offset, axis);
    }
    
    SignedVector position(s_cord_t p_offset, SignedVector start)
    {
        return SignedVector(start[axis] + p_offset, start[~axis] + s_offset, axis);
    }
};

SignedVector ray_cast(Vector start, SignedVector direction, cord_t length)
{
    BresenhamAlgorithm algorithm(direction);

    for (s_cord_t p_offset = 0; std::abs(p_offset) < length; p_offset += algorithm.p_direction)
    {
        algorithm.step();
        if (algorithm.should_compensate_error())
            algorithm.compensate_error();
    }

    return SignedVector(length * algorithm.p_direction, algorithm.s_offset, algorithm.axis);
}

void draw_line_pixel_using_pattern(
    cord_t length_position, cord_t length,
    s_cord_t thickness_position, cord_t thickness,
    Vector start, Vector end, Bounds limits, SignedVector position,
    Pattern pattern, Screen output
)
{
    if (position.is_positive() == false or limits.is_inside_abs(position.remove_sings()) == false)
        return;

    transparent_color_t color;
    if (pattern.decoration_options.flags & Pattern::UsePathBasedCoordinateSystem)
    {
        color = pattern.perform(
            length_position,
            thickness_position,
            length,
            thickness
        );
    }
    else
    {
        Bounds outline_box = Bounds(
            Vector(constrained_cords_subtract(start.x(), thickness), constrained_cords_subtract(start.y(), thickness)),
            Vector(end.x() + thickness, end.y() + thickness)
        );

        Bounds limited_outline_box = Bounds::intersect(limits, outline_box);

        LocalVector local_position = (position - limited_outline_box.start).remove_sings();

        color = pattern.perform(
            local_position.x(), local_position.y(),
            limited_outline_box.size.width(), limited_outline_box.size.height()
        );
    }

    if (color.is_transparent() == false)
        output->draw_pixel(position.remove_sings(), color.color());
}

struct SubLinePatternParamters { cord_t length_position; cord_t length; s_cord_t thickness_position_offset; Pattern pattern; Vector line_start; Vector line_end; };
enum class SubLineCornerMode { None, Post, Pre };

#define DRAW_SUB_LINE_PIXEL_DRAW_CALL(position) \
    draw_line_pixel_using_pattern( \
        pp.length_position, pp.length, \
        pp.thickness_position_offset + p_offset, thickness, \
        pp.line_start, pp.line_end, limits, \
        position, pp.pattern, output)

void draw_sub_line(SignedVector start, SignedVector direction, cord_t thickness, Screen output, Bounds limits, SubLineCornerMode corner_mode, SubLinePatternParamters pp)
{
    BresenhamAlgorithm algorithm(direction);

    bool error_compensation_last_time = false;

    for (s_cord_t p_offset = 0; std::abs(p_offset) < thickness; p_offset += algorithm.p_direction)
    {
        SignedVector physical_position = algorithm.position(p_offset, start);

        DRAW_SUB_LINE_PIXEL_DRAW_CALL(physical_position);

        if (physical_position.is_positive() and limits.is_inside_abs(physical_position.remove_sings()))
            output->draw_pixel(physical_position.remove_sings(), ColorMap::Red);

        if (error_compensation_last_time && corner_mode == SubLineCornerMode::Post)
        {
            physical_position = physical_position.with(physical_position[~algorithm.axis] - algorithm.s_direction, ~algorithm.axis);
            DRAW_SUB_LINE_PIXEL_DRAW_CALL(physical_position);
        }

        algorithm.step();
        error_compensation_last_time = false;
        if (algorithm.should_compensate_error())
        {
            error_compensation_last_time = true;
            algorithm.compensate_error();

            if (corner_mode == SubLineCornerMode::Pre)
            {
                DRAW_SUB_LINE_PIXEL_DRAW_CALL(algorithm.position(p_offset, start));
            }
        }
    }
}

inline void draw_thick_line(Vector start, SignedVector end, cord_t length, cord_t thickness, Bounds limits, Pattern pattern, Screen output)
{
    if (thickness == 0) thickness = 1;
    
    SignedVector start_to_end = end - start;
    SignedVector perpendicular = start_to_end.rotate_clockwise();
    SignedVector offset = ray_cast(start, perpendicular.rotate_180(), thickness / 2);
    SignedVector shifted_start = offset + start;
    SignedVector shifted_end = offset + end;

    BresenhamAlgorithm algorithm(start_to_end);

    bool should_draw_next_sub_line_in_corner_mode = algorithm.error < algorithm.delta_s; //If should_compensate_error() after first iteration

    bool magic_characteristic = (algorithm.s_direction == 1) == (algorithm.p_direction == 1) == (algorithm.axis == AxisX);
    SubLineCornerMode corner_mode = magic_characteristic ? SubLineCornerMode::Pre : SubLineCornerMode::Post;

    cord_t total_steps = length == 0 ? algorithm.delta_p : length;

    SubLinePatternParamters pp;
    pp.length = length;
    pp.line_start = start;
    pp.line_end = end.constrain();
    pp.pattern = pattern;
    pp.thickness_position_offset = thickness / 2; //ray cast length


    for (s_cord_t p_offset = 0; std::abs(p_offset) <= total_steps; p_offset += algorithm.p_direction)
    {
        SignedVector cursor = algorithm.position(p_offset, shifted_start);
        
        pp.length_position = p_offset;

        draw_sub_line(cursor, perpendicular, thickness, output, limits, should_draw_next_sub_line_in_corner_mode ? corner_mode : SubLineCornerMode::None, pp);
        should_draw_next_sub_line_in_corner_mode = false;

        algorithm.step();
        if (algorithm.should_compensate_error())
        {
            algorithm.compensate_error();
            should_draw_next_sub_line_in_corner_mode = true;
        }
    }
}
