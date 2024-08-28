#ifndef GUI_RENDER_UI_VIEW_H
#define GUI_RENDER_UI_VIEW_H
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
    UIView_();

    UIElement get_element();

    void change_child(UIElement new_child);

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;
};


#endif
