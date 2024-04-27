#include "gui/elements/Label.h"

Label_::Label_(const char* text)
{
    _text = text;
}

void Label_::render(const GFX& gfx)
{
    gfx.print_text(create_point(0,0),_text);
    
}

Size Label_::min_size() 
{
    //TODO 
    return create_size(10,30);
}

