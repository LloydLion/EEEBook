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
private:
    PaddingSize _p_padding;

protected:
    GFX assume_padding(const GFX &gfx);

    void subscribe_all_children();
    void subscribe_child(UIElement element);
    void unsubscribe_child(UIElement element);

public:
    PROPERTY(PaddingSize, padding) AUTO_GET(_p_padding);

    virtual void c_notify_composition_mutation(UIElement element);

    virtual Iterator<UIElement> *list_children() = 0;
    virtual size_t count_children() = 0;
};

#endif
