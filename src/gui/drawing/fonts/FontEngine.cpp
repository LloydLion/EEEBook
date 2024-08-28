#include "gui/drawing/fonts/FontEngine.h"
#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <limits>

FontId FontEngine_::find_required(const char *name)
{
    FontId id = find_or_default(name);

    if (id == 0 and name != nullptr)
        throw std::runtime_error("No font with given name found in engine");
    
    return id;
}

FontId FontEngine_::find_or_default(const char *name)
{
    for (size_t i = 0; i < _fonts.size(); i++)
        if (strcmp(name, _fonts[i].font->name) == 0)
            return i;
    
    return 0;
}

cord_t FontEngine_::get_char_width(FontId font, char c)
{
    const DrawFont *raw = _fonts[font].font;
    if (has_char(font, c) == false)
        throw std::runtime_error("Enable to get width of unknown char");

    size_t index = c - raw->first;
    const DrawFont::Glyph &glyph = raw->glyphs[index];
    return glyph.x_advance;
}

cord_t FontEngine_::get_height(FontId font)
{
    return _fonts[font].height;
}

bool FontEngine_::has_char(FontId font, char c)
{
    const DrawFont *raw = _fonts[font].font;
    return c >= raw->first and c <= raw->last;
}

FontId FontEngine_::register_font(const DrawFont *draw_font)
{   
    InternalFont i_font;
    i_font.font = draw_font;

    s_cord_t min_y = std::numeric_limits<s_cord_t>::max();
    s_cord_t max_y = std::numeric_limits<s_cord_t>::min();

    for (char c = 0; c <= draw_font->last - draw_font->first; c++)
    {
        DrawFont::Glyph *glyph = draw_font->glyphs + c;

        min_y = std::min(min_y, (s_cord_t)glyph->y_offset);
        max_y = std::max(max_y, (s_cord_t)glyph->y_offset + (s_cord_t)glyph->height);
    }

    i_font.height = max_y - min_y;
    i_font.y_offset = -min_y;

    _fonts.push_back(i_font);
    return _fonts.size() - 1;
}

const char *FontEngine_::get_name(FontId font)
{
    return _fonts[font].font->name;
}

const DrawFont *FontEngine_::get_raw(FontId font)
{
    return _fonts[font].font;
}

cord_t FontEngine_::get_y_offset(FontId font)
{
    return _fonts[font].y_offset;
}
