#ifndef GUI_UI_ELEMENT_H
#define GUI_UI_ELEMENT_H

#include "cordinates.h"
#include "GFX.h"

class UIElement_;
typedef UIElement_ *UIElement;

class UIElement_
{
protected:
    virtual void i_render(const GFX& gfx) = 0;
    virtual Size i_min_size() = 0;
    virtual Size i_max_size() = 0;

public:
    transparent_color_t foreground_color = color_t::Black;
    transparent_color_t background_color = color_t::White;
    MarginSize margin = MarginSize(0);

    void render(const GFX& gfx);
    Size min_size();
    Size max_size();
};

#endif
