#include "gui/elements/Label.h"
#include "gui/text.h"

Label_::Label_(const char* text): _original_text(text)
{
    _text_buffer = new char[strlen(text)];
}

Label_::~Label_()
{
    delete[] _text_buffer;
}

void Label_::render(const GFX& gfx)
{
    strcpy(_text_buffer, _original_text);
    gfx.cut_and_print_text(create_point(0, 0), _text_buffer);
}

void Label_::set_text(const char *text)
{
    delete[] _text_buffer;
    _text_buffer = new char[strlen(text)];
    _original_text = text;
}

Size Label_::min_size()
{
    //TODO 
    return create_size(10,30);
}

