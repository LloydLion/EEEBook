#include "gui/drawing/screens/BMP_File_Screen.h"

#if PLATFORM & PLATFORM_PC

BMP_File_Screen_::BMP_File_Screen_(Size full_viewport_size): _full_viewport_size(full_viewport_size), _bitmap(full_viewport_size.width(), full_viewport_size.height())
{

}


bmp::Pixel color_to_pixel(color_t color)
{
    auto value = color_to_rgb888(color);
    return bmp::Pixel(value.r, value.g, value.b);
}

void BMP_File_Screen_::draw_pixel(Vector position, color_t color)
{
    _bitmap.set(position.x(), position.y(), color_to_pixel(color));
}

void BMP_File_Screen_::draw_vertical_line(Vector position, cord_t size, color_t color)
{
    _bitmap.draw_line(position.x(), position.y(), position.x(), position.y() + size - 1, color_to_pixel(color));
}

void BMP_File_Screen_::draw_horizontal_line(Vector position, cord_t size, color_t color)
{
    _bitmap.draw_line(position.x(), position.y(), position.x() + size - 1, position.y(), color_to_pixel(color));
}

void BMP_File_Screen_::initialize()
{

}

void BMP_File_Screen_::begin()
{

}

void BMP_File_Screen_::send()
{
    _bitmap.save("file.bmp");
}

void BMP_File_Screen_::clear()
{
    _bitmap.clear(bmp::Pixel(255, 255, 255));
}

#endif
