#ifndef GUI_FONT_H
#define GUI_FONT_H
#include <Arduino.h>
#include "gui/FontEngine.h"

class Font_;
typedef Font_ *Font;

class Font_
{
private:
    font_id_t _id;
    FontEngine _owner;
public:
    Font_(font_id_t id, FontEngine owner);

    cord_t get_char_width(char c);
    cord_t get_string_width(const char* str);
    cord_t get_height();

    size_t cut_text(cord_t width_limit, char* text);
};

#endif
