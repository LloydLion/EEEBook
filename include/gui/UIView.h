#ifndef GUI_UI_VIEW_H
#define GUI_UI_VIEW_H
#include "UIContainer.h"

class UIView_;
typedef UIView_ *UIView; 

class UIView_ : public UIContainer_
{
private:
    UIElement _element;
    SingleElementIterator<UIElement> _child_iterator;

public:
    UIView_(UIElement element);

    UIElement get_element();

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;
};

#endif
