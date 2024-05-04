#include "gui/elements/Label.h"

Label_::Label_(const char* text): _text(text)
{

}

void Label_::render(const GFX& gfx)
{
    gfx.draw_rectangle(LocalBounds(Vector(), gfx.size()), background_color);
    gfx.print_text(Vector(), _text, foreground_color, -1, get_font());
}

void Label_::set_text(const char *text)
{
    _text = text;
}

Size Label_::min_size()
{
    //TODO 
    return Size(10,30);
}

