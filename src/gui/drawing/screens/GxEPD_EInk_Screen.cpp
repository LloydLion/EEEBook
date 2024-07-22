#include "gui/drawing/screens/GxEPD_EInk_Screen.h"
#if PLATFORM & PLATFORM_MCU

#include <stdexcept>
#include "platform/stdout.h"

#define HEIGHT DisplayDriver::HEIGHT
#define WIDTH DisplayDriver::WIDTH

#define CHECK_IF_POSITION_NOT_IN_BOUNDS(POSITION) POSITION.x() >= WIDTH or POSITION.y() >= HEIGHT

#if GXEPD_SCREEN_TRACE_OPTIONS & GXEPD_SCREEN_TRACE_LOG_DRAWS
    #define RETURN_IF_POSITION_NOT_IN_BOUNDS(POSITION) if (CHECK_IF_POSITION_NOT_IN_BOUNDS(POSITION)) { std_println("NOT IN BOUNDS ERROR -> end"); return; }
    #define APPLY_COLOR_TO_BUFFER(BUFFER, COLOR_CONDITION, BYTE_INDEX, BIT_INDEX) \
        if (color == COLOR_CONDITION) { std_printf("Writing %d to %d:%d -> ", COLOR_CONDITION, BYTE_INDEX, BIT_INDEX); BUFFER[BYTE_INDEX] &= ~(1u << BIT_INDEX); } \
        else BUFFER[BYTE_INDEX] |= (1u << BIT_INDEX)
#else
    #define RETURN_IF_POSITION_NOT_IN_BOUNDS(POSITION) if (CHECK_IF_POSITION_NOT_IN_BOUNDS(POSITION)) return;
    #define APPLY_COLOR_TO_BUFFER(BUFFER, COLOR_CONDITION, BYTE_INDEX, BIT_INDEX) \
        if (color == COLOR_CONDITION) BUFFER[BYTE_INDEX] &= ~(1u << BIT_INDEX); \
        else BUFFER[BYTE_INDEX] |= (1u << BIT_INDEX)
#endif

#define IS_BWR COLOR_MODEL == BWR_COLOR_MODEL

#ifndef GXEPD_EINK_SCREEN_DISPLAY_ROTATION
    #define GXEPD_EINK_SCREEN_DISPLAY_ROTATION 0
#endif
#if GXEPD_EINK_SCREEN_DISPLAY_ROTATION >= 4 || GXEPD_EINK_SCREEN_DISPLAY_ROTATION < 0
    #error Invalid GXEPD_EINK_SCREEN_DISPLAY_ROTATION, must be in 0..3 range (inclusive)
#endif

#if GXEPD_SCREEN_TRACE_OPTIONS & GXEPD_SCREEN_TRACE_LOG_DRAWS
    #define trace_log(...) std_printf(__VA_ARGS__)
#else
    #define trace_log(...)
#endif

Vector rotate(Vector original)
{
    cord_t x = original.x(), y = original.y();
    switch (GXEPD_EINK_SCREEN_DISPLAY_ROTATION)
    {
    case 1:
        std::swap(x, y);
        x = WIDTH - x - 1;
        break;
    case 2:
        x = WIDTH - x - 1;
        y = HEIGHT - y - 1;
        break;
    case 3:
        std::swap(x, y);
        y = HEIGHT - y - 1;
        break;
    }

    return Vector(x, y);
}

GxEPD_EInk_Screen_::GxEPD_EInk_Screen_():
    _display(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_BUSY_PIN)
{
    clear();
}

void GxEPD_EInk_Screen_::apply_color_to_buffers(Vector postion, color_t color)
{
    size_t bit_index = postion.x() + postion.y() * WIDTH;
    size_t byte_index = bit_index / 8;
    bit_index = 7 - (bit_index % 8);
    
    APPLY_COLOR_TO_BUFFER(_black_frame_buffer, ColorMap::Black, byte_index, bit_index);
#if IS_BWR
    APPLY_COLOR_TO_BUFFER(_color_frame_buffer, ColorMap::Red, byte_index, bit_index);
#endif
}

void GxEPD_EInk_Screen_::draw_line(Vector position, cord_t size, color_t color, Axis axis)
{
    Vector rotated_position = rotate(position);
    trace_log("rotated to {%d, %d} -> ", rotated_position.x(), rotated_position.y());
    RETURN_IF_POSITION_NOT_IN_BOUNDS(rotated_position);

    Vector vectorAlongAxis = Vector().with(1, axis);
    for (cord_t offset = 0; offset < size; offset++)
    {
        position = position + vectorAlongAxis;
        rotated_position = rotate(position);
        if (rotated_position.x() >= WIDTH or rotated_position.y() >= HEIGHT)
        {
            trace_log("Boundary reached -> end\n");
            return;
        }

        trace_log("At {%d, %d} -> ", rotated_position.x(), rotated_position.y());
        apply_color_to_buffers(rotated_position, color);
    }
    
    trace_log("end\n");
}

void GxEPD_EInk_Screen_::draw_pixel(Vector position, color_t color)
{
    trace_log("draw_pixel({%d, %d}, %d) -> ", position.x(), position.y(), color);
    position = rotate(position);
    trace_log("rotated to {%d, %d} -> ", position.x(), position.y());
    RETURN_IF_POSITION_NOT_IN_BOUNDS(position)

    apply_color_to_buffers(position, color);
    trace_log("end");
}

void GxEPD_EInk_Screen_::draw_vertical_line(Vector position, cord_t size, color_t color)
{
    trace_log("draw_vertical_line({%d, %d}, %d) -> ", position.x(), position.y(), color);
    draw_line(position, size, color, AxisX);
}

void GxEPD_EInk_Screen_::draw_horizontal_line(Vector position, cord_t size, color_t color)
{
    trace_log("draw_horizontal_line({%d, %d}, %d) -> ", position.x(), position.y(), color);
    draw_line(position, size, color, AxisY);
}

Size GxEPD_EInk_Screen_::full_viewport_size()
{
    return GXEPD_EINK_SCREEN_DISPLAY_ROTATION % 2 == 0 ? Size(WIDTH, HEIGHT) : Size(HEIGHT, WIDTH);
}

void GxEPD_EInk_Screen_::initialize()
{
    _display.init();
}

void GxEPD_EInk_Screen_::begin()
{
    
}

void GxEPD_EInk_Screen_::configure(bool use_partial_update)
{
    _use_partial_update = use_partial_update;
}

void GxEPD_EInk_Screen_::clear()
{
    memset(_black_frame_buffer, 0xFF, _buffer_byte_size);
#if IS_BWR
    memset(_color_frame_buffer, 0xFF, _buffer_byte_size);
#endif
}

void GxEPD_EInk_Screen_::send()
{
#if GXEPD_SCREEN_TRACE_OPTIONS & GXEPD_SCREEN_TRACE_PRINT_BUFFER
    for (size_t pos = 0; pos < _buffer_bit_size; pos++)
    {
        if (pos % WIDTH == 0)
            std_println();
        size_t byte_index = pos / 8;
        size_t bit_index = 7 - (pos % 8);

        bool black = _black_frame_buffer[byte_index] & (1u << bit_index);
#if IS_BWR
        bool color = _color_frame_buffer[byte_index] & (1u << bit_index);
        std_print(black ? (color ? 'W' : 'R') : (color ? 'B' : 'I'));
#else
        std_print(black ? 'W' : 'B');
#endif
    }
#endif
    
    bool use_partial_update = true;

#if IS_BWR
    if (use_partial_update and _display.hasPartialUpdate)
    {
        _display.writeImage(_black_frame_buffer, _color_frame_buffer, 0, 0, WIDTH, HEIGHT);
        _display.refresh(0, 0, WIDTH, HEIGHT);
        if (_display.hasFastPartialUpdate)
            _display.writeImage(_black_frame_buffer, _color_frame_buffer, 0, 0, WIDTH, HEIGHT);
    }
    else
    {
        _display.setPaged();
        _display.writeImage(_black_frame_buffer, _color_frame_buffer, 0, 0, WIDTH, HEIGHT);

        if (_display.panel == GxEPD2::GDEW0154Z04)
            _display.writeImage(_black_frame_buffer, _color_frame_buffer, 0, 0, WIDTH, HEIGHT);
        _display.refresh(false);
        _display.powerOff();
    }
#else
    if (use_partial_update)
    {
        _display.writeImage(black_frame_buffer, 0, 0, WIDTH, HEIGHT);
        _display.refresh(0, 0, WIDTH, HEIGHT);
        if (_display.hasFastPartialUpdate)
            _display.writeImageAgain(black_frame_buffer, 0, 0, WIDTH, HEIGHT);
    }
    else
    {
        _display.writeImageForFullRefresh(black_frame_buffer, 0, 0, WIDTH, HEIGHT);
        _display.refresh(false);
        if (_display.hasFastPartialUpdate)
        {
            _display.writeImageAgain(black_frame_buffer, 0, 0, WIDTH, HEIGHT);
            _display.powerOff();
        }
    }
#endif
}

#endif
