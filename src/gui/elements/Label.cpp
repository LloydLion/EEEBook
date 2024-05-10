#include "gui/elements/Label.h"

Label_::Label_(const char* text): _text(text)
{

}

void Label_::i_render(const GFX& gfx)
{
    gfx.fill_screen(background_color());
    gfx.print_text(Vector(), _text, foreground_color(), -1, get_font());
}

void Label_::set_text(const char *text)
{
    _text = text;
}

Size Label_::i_min_size()
{
    cord_t width = get_font()->get_string_width(_text);
    cord_t height = get_font()->get_height();
    
    return Size(width, height);
}


Size Label_::i_max_size()
{
    return i_min_size();
}

