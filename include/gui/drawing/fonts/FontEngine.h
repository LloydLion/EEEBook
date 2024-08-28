#ifndef GUI_DRAWING_FONTS_FONT_ENGINE_H
#define GUI_DRAWING_FONTS_FONT_ENGINE_H

#include <stdint.h>
#include <stddef.h>
#include <vector>
#include "gui/coordinates.h"
#include "gui/drawing/fonts/DrawFont.h"

typedef uint8_t FontId;

class FontEngine_;
typedef FontEngine_ *FontEngine;

class FontEngine_
{
private:
    struct InternalFont
    {
        const DrawFont *font;
        cord_t height;
        cord_t y_offset;
    };
   

    std::vector<InternalFont> _fonts;

public:
    FontId find_required(const char *name);
    FontId find_or_default(const char *name);

    cord_t get_char_width(FontId font, char c);
    cord_t get_height(FontId font);
    bool has_char(FontId font, char c);

    const char *get_name(FontId font);
    const DrawFont *get_raw(FontId font);

    cord_t get_y_offset(FontId font);

    FontId register_font(const DrawFont *draw_font);
};

#endif
