#ifndef GUI_DRAWING_SCREENS_BMP_FILE_SCREEN_H
#define GUI_DRAWING_SCREENS_BMP_FILE_SCREEN_H

#include "platform/platform.h"

#if PLATFORM & PLATFORM_PC

#include "gui/drawing/Screen.h"
#include "BitmapPlusPlus.hpp"

class BMP_File_Screen_ : public Screen_
{
private:
    Size _full_viewport_size;
    bmp::Bitmap _bitmap;

public:
    BMP_File_Screen_(Size full_viewport_size);

    void draw_pixel(Vector position, color_t color) override;
    void draw_vertical_line(Vector position, cord_t size, color_t color) override;
    void draw_horizontal_line(Vector position, cord_t size, color_t color) override;

    inline Size full_viewport_size() { return _full_viewport_size; }

    void initialize() override;
    void begin() override;
    void send() override;
    void clear() override;
};

#endif

#endif
