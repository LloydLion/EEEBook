#include "gui/text.h"
#include <exception>

size_t cut_text(const GFX &gfx, cord_t width_limit, char* text)
{
    char *new_line = strchr(text, '\n');
    if (new_line != nullptr)
    {
        *new_line = '\0';
    }

    size_t text_len = strlen(text);

recheck:
    Size size = gfx.get_text_size(text);
    if (width_limit < size.width)
    {
        if (text_len < 0)
            throw std::runtime_error("Strage text recived, enable to fit in box");

        text[text_len] = '\0';
        text_len--;
        goto recheck;
    }

    return text_len;
}
