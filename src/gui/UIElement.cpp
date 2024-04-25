#include "gui/UIElement.h"

UIElement_::UIElement_()
{

}


UIElement_::~UIElement_()
{

}

bool UIElement_::is_moved() { return _is_moved; }

void UIElement_::mark_as_moved() { _is_moved = true; } 