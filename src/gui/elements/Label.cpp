#include "gui/elements/Label.h"

Label_::Label_(const char* text): _text(text)
{

}

void Label_::render(const GFX& pgfx)
{
    ASSUME_MARGIN(pgfx);
    gfx.fill_screen(background_color);
    gfx.print_text(Vector(), _text, foreground_color, -1, get_font());
}

void Label_::set_text(const char *text)
{
    _text = text;
}

Size Label_::min_size()
{
    cord_t width = get_font()->get_string_width(_text);
    cord_t height = get_font()->get_height();

    return margin.expand(Size(width, height));
}

