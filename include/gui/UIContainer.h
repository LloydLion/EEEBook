#ifndef GUI_UI_Container_H
#define GUI_UI_Container_H

#include "cordinates.h"
#include "UIElement.h"
#include "std/iterator.h"
#include <vector>

class UIContainer_;
typedef UIContainer_ *UIContainer;

class UIContainer_ : public UIElement_
{
protected:
    std::vector<UIElement> _elements;

    GFX assume_padding(const GFX &gfx);

public:
    PaddingSize padding;

    virtual const Iterator<UIElement> *list_children() = 0;
    virtual size_t count_children() = 0;
};

#endif
