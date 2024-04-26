#include "gui/elements/Label.h"

Label_::Label_(char* text) : _text(text){}

void Label_::render(const GFX& gfx)
{
    if (_is_drawn and !_is_moved)
    return;

    gfx.print_text(create_point(0,0),_text);

    _is_drawn = true;
    _is_moved = false;
}

bool Label_::is_updated() {return !_is_drawn;}

Size Label_::min_size() 
{
    //TODO 
    return create_size(10,30);
}

