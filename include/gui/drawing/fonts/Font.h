#ifndef GUI_DRAWING_FONTS_FONT_H
#define GUI_DRAWING_FONTS_FONT_H
#include "gui/drawing/fonts/FontEngine.h"

class Font
{
private:
    FontId _id;

public:
    Font();
    Font(FontId id);
    Font(const char *name);
    Font(const Font &font);

    cord_t get_char_width(char c);
    cord_t get_string_width(const char *str);
    cord_t get_height();

    bool has_char(char c);
    size_t first_non_legit_char(const char *str);

    FontId id();
    const char *name();

    const Font &operator=(const Font &font);
    bool operator==(const Font &font);
};

#endif
