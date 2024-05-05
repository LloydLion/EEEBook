#ifndef GUI_UI_VIEW_H
#define GUI_UI_VIEW_H
#include "UIElement.h"

class UIView_;
typedef UIView_ *UIView; 

class UIView_ : public UIElement_
{
public:
    virtual UIElement get_element() = 0;
};

#endif
