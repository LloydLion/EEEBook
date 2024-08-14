#include "gui/color.h"

#if COLOR_MODEL == BW_COLOR_MODEL
constexpr color_t ColorMap::Black;
constexpr color_t ColorMap::White;

uint16_t color_to_rgb565(color_t color)
{
    switch (color)
    {
    case ColorMap::Black: return 0x0000;
    case ColorMap::White: return 0xFFFF;
    default: return 0x0000;
    }
}
std::tuple<uint8_t, uint8_t, uint8_t> color_to_rgb888(color_t color)
{
    switch (color)
    {
    case ColorMap::Black: return std::tuple<uint8_t, uint8_t, uint8_t>(0x00, 0x00, 0x00);
    case ColorMap::White: return std::tuple<uint8_t, uint8_t, uint8_t>(0xFF, 0xFF, 0xFF);
    default: return std::tuple<uint8_t, uint8_t, uint8_t>(0x00, 0x00, 0x00);
    }
}
#elif COLOR_MODEL == BWR_COLOR_MODEL
constexpr color_t ColorMap::Black;
constexpr color_t ColorMap::White;
constexpr color_t ColorMap::Red;

uint16_t color_to_rgb565(color_t color)
{
    switch (color)
    {
    case ColorMap::Black: return 0x0000;
    case ColorMap::White: return 0xFFFF;
    case ColorMap::Red: return 0xF800;
    default: return 0x0000;
    }
}

rgb888_color color_to_rgb888(color_t color)
{
    switch (color)
    {
    case ColorMap::Black: return rgb888_color(0x00, 0x00, 0x00);
    case ColorMap::White: return rgb888_color(0xFF, 0xFF, 0xFF);
    case ColorMap::Red: return rgb888_color(0xFF, 0x00, 0x00);
    default: return rgb888_color(0xFF, 0x00, 0x00);
    }
}
#elif COLOR_MODEL == RGB_COLOR_MODEL
constexpr color_t ColorMap::Black;
constexpr color_t ColorMap::White;
constexpr color_t ColorMap::Red;
constexpr color_t ColorMap::Green;
constexpr color_t ColorMap::Blue;
constexpr color_t ColorMap::Yellow;

uint16_t color_to_rgb565(color_t color)
{
    uint8_t red = (color & ColorMap::Red) << 1;
    uint8_t green = (color & ColorMap::Green) >> 2;
    uint8_t blue = (color & ColorMap::Blue) << 3;
    
    return (((uint16_t)(red | green)) << 8) | (uint16_t)blue;
}

rgb888_color color_to_rgb888(color_t color)
{
    uint8_t red = (color & ColorMap::Red) << 1;
    uint8_t green = (color & ColorMap::Green) << 3;
    uint8_t blue = (color & ColorMap::Blue) << 6;
    
    return rgb888_color(red, green, blue);
}
#endif

template<typename TColor>
TColor ColorArrayBase<TColor>::get(size_t index) const
{
    size_t bit_offset = index * _depth;
    size_t byte_index = bit_offset / 8;
    uint16_t store;

    store = (uint16_t)_array[byte_index];
    if (byte_index + 1 < _array_size)
        store = (uint16_t)_array[byte_index + 1] << 8;

    size_t bit_offset_remaining = bit_offset - (8 * byte_index);

    uint8_t value = (store >> bit_offset_remaining) & _mask;
    return (TColor)value;
}

template<typename TColor>
void ColorArrayBase<TColor>::set(size_t index, TColor color)
{
    size_t bit_offset = index * _depth;
    size_t byte_index = bit_offset / 8;
    uint16_t store;

    store = (uint16_t)_array[byte_index];
    if (byte_index + 1 < _array_size)
        store = (uint16_t)_array[byte_index + 1] << 8;

    size_t bit_offset_remaining = bit_offset - (8 * byte_index);
    
    store &= ~(_mask << bit_offset_remaining);
    store |= (uint16_t)color << bit_offset_remaining;

    _array[byte_index] = (byte)store;
    if (byte_index + 1 < _array_size)
        _array[byte_index + 1] = (byte)(store >> 8);
}

template class ColorArrayBase<color_t>;
template class ColorArrayBase<transparent_color_t>;
