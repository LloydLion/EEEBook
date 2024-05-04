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

font_id_t Font_::id()
{
    return _id;
}
