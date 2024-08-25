#include "gui/drawing/sub_drawers/LineSubDrawer.h"

SignedVector ray_cast(SignedVector direction, cord_t length);
Vector ray_cast_bounded(SignedVector direction, cord_t length, Bounds bounds, Vector start, cord_t *actual_length);

LineSubDrawer::LineSubDrawer(Vector start, SignedVector pre_end, cord_t length, cord_t thickness):
    _start(start), _pre_end(pre_end), _length(length), _thickness(thickness) { }


void LineSubDrawer::draw()
{
    // Thickness correction
    if (_thickness == 0) _thickness = 1;

    // End point determination
    if (_length != 0)
        _end = ray_cast_bounded(_pre_end - _start, _length, bounds, _start, &_length);
    else
        _end = _pre_end.remove_sings();

    // Outline box determination
    Bounds line_box = Bounds(_start, _end);

    Bounds outline_box = Bounds(
        Vector(constrained_cords_subtract(line_box.start.x(), _thickness), constrained_cords_subtract(line_box.start.y(), _thickness)),
        Vector(line_box.end().x() + _thickness, line_box.end().y() + _thickness)
    );

    _outline_box = Bounds::intersect(bounds, outline_box);

    // Run
    draw_thick_line();
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

void LineSubDrawer::draw_thick_line()
{   
    SignedVector start_to_end = _end - _start;
    SignedVector perpendicular = start_to_end.rotate_clockwise();
    SignedVector offset = ray_cast(perpendicular.rotate_180(), _thickness / 2);
    SignedVector shifted_start = offset + _start;
    SignedVector shifted_end = offset + _end;

    BresenhamAlgorithm algorithm(start_to_end);

    bool should_draw_next_sub_line_in_corner_mode = algorithm.error < algorithm.delta_s; //If should_compensate_error() after first iteration

    bool magic_characteristic = (algorithm.s_direction == 1) == (algorithm.p_direction == 1) == (algorithm.axis == AxisX);
    SubLineCornerMode corner_mode = magic_characteristic ? SubLineCornerMode::Pre : SubLineCornerMode::Post;


    for (s_cord_t p_offset = 0; std::abs(p_offset) <= algorithm.delta_p; p_offset += algorithm.p_direction)
    {
        SignedVector cursor = algorithm.position(p_offset, shifted_start);
        
        draw_sub_line(cursor, perpendicular, should_draw_next_sub_line_in_corner_mode ? corner_mode : SubLineCornerMode::None, p_offset);
        should_draw_next_sub_line_in_corner_mode = false;

        algorithm.step();
        if (algorithm.should_compensate_error())
        {
            algorithm.compensate_error();
            should_draw_next_sub_line_in_corner_mode = true;
        }
    }
}

void LineSubDrawer::draw_sub_line(SignedVector global_cursor, SignedVector direction, SubLineCornerMode corner_mode, cord_t length_position)
{
    BresenhamAlgorithm algorithm(direction);

    bool error_compensation_last_time = false;

    for (s_cord_t p_offset = 0; std::abs(p_offset) < _thickness; p_offset += algorithm.p_direction)
    {
        SignedVector physical_position = algorithm.position(p_offset, global_cursor);

        s_cord_t thickness_position = p_offset - (_thickness / 2);

        draw_line_pixel_using_pattern(length_position, thickness_position, physical_position);

        if (error_compensation_last_time && corner_mode == SubLineCornerMode::Post)
        {
            physical_position = physical_position.with(physical_position[~algorithm.axis] - algorithm.s_direction, ~algorithm.axis);
            draw_line_pixel_using_pattern(length_position, thickness_position, physical_position);
        }

        algorithm.step();
        error_compensation_last_time = false;
        if (algorithm.should_compensate_error())
        {
            error_compensation_last_time = true;
            algorithm.compensate_error();

            if (corner_mode == SubLineCornerMode::Pre)
            {
                draw_line_pixel_using_pattern(length_position, thickness_position, algorithm.position(p_offset, _start));
            }
        }
    }
}

void LineSubDrawer::draw_line_pixel_using_pattern(cord_t length_position, s_cord_t thickness_position, SignedVector position)
{
    if (position.is_positive() == false or bounds.is_inside_abs(position.remove_sings()) == false)
        return;

    transparent_color_t color;
    if (pattern.interpretation_options & Pattern::UsePathBasedCoordinateSystem)
    {
        color = pattern.perform(
            CoordinateRangeValue(length_position, 0, _length),
            CoordinateRangeValue(thickness_position, -(_thickness / 2), _thickness - (_thickness / 2))
        );
    }
    else
    {
        LocalVector local_position = (position - _outline_box.start).remove_sings();

        color = pattern.perform(
            CoordinateRangeValue(local_position.x(), 0, _outline_box.size.width()),
            CoordinateRangeValue(local_position.y(), 0, _outline_box.size.height())
        );
    }

    if (color.is_transparent() == false)
        output->draw_pixel(position.remove_sings(), color.color());
}

SignedVector ray_cast(SignedVector direction, cord_t length)
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

Vector ray_cast_bounded(SignedVector direction, cord_t length, Bounds bounds, Vector start, cord_t *actual_length)
{
    if (bounds.is_inside_abs(start) == false)
        throw std::runtime_error("Enable to ray cast from out of bounds point");

    BresenhamAlgorithm algorithm(direction);

    Vector cursor = start;
    for (s_cord_t p_offset = 0; std::abs(p_offset) < length; p_offset += algorithm.p_direction)
    {
        SignedVector cursor_candidate_offset = SignedVector(p_offset * algorithm.p_direction, algorithm.s_offset, algorithm.axis);
        SignedVector cursor_candidate = cursor_candidate_offset + start;

        if (cursor_candidate.is_positive() == false || bounds.is_inside_abs(cursor_candidate.remove_sings()) == false)
        {
            *actual_length = std::abs(p_offset);
            break; //Bad candidate
        }

        cursor = cursor_candidate.remove_sings();

        algorithm.step();
        if (algorithm.should_compensate_error())
            algorithm.compensate_error();
    }

    return cursor;
}
