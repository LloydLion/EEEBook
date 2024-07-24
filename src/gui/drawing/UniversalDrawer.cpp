#include "gui/drawing/UniversalDrawer.h"
#include <stdexcept>

inline void draw_pixel_using_pattern(const DrawOperation &operation, cord_t x, cord_t y, size_t pattern_index, Screen screen)
{
    transparent_color_t color = operation.patterns[0].perform(PatternCatalog::instance(), x, y, operation.bounds.size.width(), operation.bounds.size.height());
    if (!color.is_transparent())
        screen->draw_pixel(operation.bounds.start + Vector(x, y), color);
}
inline void draw_rect(const DrawOperation &operation, Screen screen);
inline void draw_bitmap(const DrawOperation &operation, Screen screen);

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

        break;
    case DrawAreaType::Bitmap:
        draw_bitmap(operation, _screen);
        break;
    }
}

inline void draw_bitmap(const DrawOperation &operation, Screen screen)
{
    cord_t width = operation.bounds.size.width();
    cord_t height = operation.bounds.size.height();
    auto operation_args = operation.arguments.bitmap;
    for (cord_t x = 0; x < width; x++)
        for (cord_t y = 0; y < height; y++)
        {
            size_t idx = operation_args.is_transposed ? y + x * height : x + y * width;
            size_t byte_idx = idx / 8;
            size_t bit_idx = operation_args.is_big_indian ? 7 - idx % 8 : idx % 8;
            bool state = operation_args.map[byte_idx] & (1 << bit_idx);
            if (operation_args.is_inverted)
                state = !state;
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