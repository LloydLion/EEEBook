#ifndef GUI_TEXT_ELEMENT_H
#define GUI_TEXT_ELEMENT_H

#include "cordinates.h"
#include  "UIElement.h"
#include "Font.h"

class TextElement_;
typedef TextElement_ *UITextElement;

class TextElement_ : public UIElement_
{
private:
    Font _font = nullptr;
public:
    void set_font(Font font);
    Font get_font();
};

#endif