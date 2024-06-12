#ifndef GUI_FONT_ENGINE_H
#define GUI_FONT_ENGINE_H
#include <stdint.h>
#include <stddef.h>
#include "gui/cordinates.h"

typedef uint8_t font_id_t;

class FontEngine_;
typedef FontEngine_ *FontEngine;

class FontEngine_
{
public:
    virtual cord_t get_char_width(font_id_t font, char c) = 0;
    virtual cord_t get_height(font_id_t font) = 0;

    virtual bool is_legit_char(font_id_t font, char c) = 0;
    virtual size_t first_non_legit_char(font_id_t font, const char* str) = 0;
};

#endif
