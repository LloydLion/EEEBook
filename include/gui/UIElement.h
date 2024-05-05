#ifndef GUI_UI_ELEMENT_H
#define GUI_UI_ELEMENT_H

#include "cordinates.h"
#include "GFX.h"

#define ASSUME_MARGIN(X) GFX gfx = X.slice(margin)

class UIElement_;
typedef UIElement_ *UIElement;

class UIElement_
{
public:
    color_t foreground_color = color_t::Black;
    color_t background_color = color_t::White;
    MarginSize margin = MarginSize(0);

    virtual void render(const GFX& gfx) = 0;
    virtual Size min_size() = 0;
};

#endif
