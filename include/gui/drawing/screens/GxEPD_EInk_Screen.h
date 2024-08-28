#ifndef GUI_DRAWING_SCREENS_GXEPD_EINK_SCREEN_H
#define GUI_DRAWING_SCREENS_GXEPD_EINK_SCREEN_H

#include "platform/platform.h"

#if PLATFORM & PLATFORM_MCU

#include "gui/drawing/Screen.h"

#if COLOR_MODEL != BWR_COLOR_MODEL && COLOR_MODEL != BW_COLOR_MODEL
    #error "Unsupported color model"
#endif


class GxEPD_EInk_Screen_ : public Screen_
{
private:
    static constexpr size_t _buffer_bit_size = DisplayDriver::HEIGHT * DisplayDriver::WIDTH;
    static constexpr size_t _buffer_byte_size = _buffer_bit_size / 8 + (_buffer_bit_size % 8 == 0 ? 0 : 1);

    DisplayDriver _display;
    bool _use_partial_update = true;
    byte _black_frame_buffer[_buffer_byte_size];
#if COLOR_MODEL == BWR_COLOR_MODEL
    byte _color_frame_buffer[_buffer_byte_size];
#endif

    void apply_color_to_buffers(Vector postion, color_t color);
    void draw_line(Vector position, cord_t size, color_t color, Axis axis);

public:
    GxEPD_EInk_Screen_();

    void draw_pixel(Vector position, color_t color) override;
    void draw_vertical_line(Vector position, cord_t size, color_t color) override;
    void draw_horizontal_line(Vector position, cord_t size, color_t color) override;

    Size full_viewport_size() override;

    void configure(bool use_partial_update);

    void initialize() override;
    void begin() override;
    void send() override;
    void clear() override;
};

#endif
#endif
