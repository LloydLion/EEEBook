#include "gui/Font.h"

Font_::Font_(font_id_t id, FontEngine owner): _id(id), _owner(owner)
{

}


cord_t Font_::get_char_width(char c)
{
    return _owner->get_char_width(_id, c);
}

cord_t Font_::get_string_width(const char *str)
{
    cord_t total_size = 0;
    while (char c = *str++)
    {
        total_size += get_char_width(c);
    }

    return total_size;
}

cord_t Font_::get_height()
{
    return _owner->get_height(_id);
}

size_t Font_::cut_text(cord_t width_limit, char *text)
{
    size_t non_legit_index = _owner->first_non_legit_char(_id, text);
    if (non_legit_index != -1)
    {
        text[non_legit_index] = '\0';
    }

    size_t text_len = strlen(text);
    cord_t total_width = get_string_width(text);

    while (total_width > width_limit)
    {
        total_width -= get_char_width(text[text_len - 1]);
        text[text_len - 1] = '\0';
        text_len--;
    }

    return text_len;
}
