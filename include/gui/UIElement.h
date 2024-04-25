#ifndef GUI_UIELEMENT_H
#define GUI_UIELEMENT_H

#include "cordinates.h"
#include "GFX.h"

class UIElement_;
typedef UIElement_ *UIElement;

class UIElement_
{
public:
    UIElement_();
    ~UIElement_();

    virtual void render(const GFX& gfx) = 0;

    virtual bool is_updated() = 0;
    bool is_moved();
    void mark_as_moved();
    virtual Size min_size() = 0;
protected:
    bool _is_moved;
};

#endif
