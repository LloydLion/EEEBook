#include "gui/drawing/fonts/Font.h"
#include "gui/drawing/DrawingContext.h"


Font::Font(): _id(0) { }

Font::Font(FontId id): _id(id) { }

Font::Font(const char *name): _id(DrawingContext::instance().font_engine->find_required(name)) { }

Font::Font(const Font &font): _id(font._id) { }

cord_t Font::get_char_width(char c)
{
    return DrawingContext::instance().font_engine->get_char_width(_id, c);
}

cord_t Font::get_string_width(const char *str)
{
    cord_t total_size = 0;
    while (char c = *str++)
        total_size += get_char_width(c);

    return total_size;
}

cord_t Font::get_height()
{
    return DrawingContext::instance().font_engine->get_height(_id);
}

bool Font::has_char(char c)
{
    return DrawingContext::instance().font_engine->has_char(_id, c);
}

size_t Font::first_non_legit_char(const char *str)
{
    size_t position = 0;
    cord_t total_size = 0;
    while (char c = *(str + position++))
    {
        if (has_char(c) == false)
            return position;
    }

    return -1;
}

FontId Font::id()
{
    return _id;
}

const char *Font::name()
{
    return DrawingContext::instance().font_engine->get_name(_id);
}

const Font &Font::operator=(const Font &font)
{
    _id = font._id;
    return font;
}

bool Font::operator==(const Font &font)
{
    return _id == font._id;
}
