#ifndef GUI_UI_STORAGE_H
#define GUI_UI_STORAGE_H

#include "cordinates.h"
#include "UIElement.h"
#include <vector>

class UIStorage_;
typedef UIStorage_ *UIStorage;

class UIStorage_ : public UIElement_
{
public:
    UIStorage_() { };
    ~UIStorage_() { };

    virtual std::vector<UIElement>::iterator list_children() = 0;
    virtual size_t count_children() = 0;
};

#endif
