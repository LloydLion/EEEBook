#ifndef GUI_UI_ELEMENT_H
#define GUI_UI_ELEMENT_H

#include "cordinates.h"
#include "GFX.h"

class UIElement_;
typedef UIElement_ *UIElement;

class UIElement_
{
public:
    color_t foreground_color;
    color_t background_color;

    UIElement_();
    ~UIElement_();

    virtual void render(const GFX& gfx) = 0;

    virtual Size min_size() = 0;
};

#endif
