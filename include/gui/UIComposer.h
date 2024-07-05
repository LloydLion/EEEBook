#ifndef GUI_UI_COMPOSER_H
#define GUI_UI_COMPOSER_H
#include "gui/UIContainer.h"

#define DEFAULT_REMOVE_CHILD_IMPLEMENTATION(CLASS, COLLECTION, ELEMENT_PATH) void CLASS::remove_child(UIElement child) \
{ \
    cord_t index = 0; bool found = false; \
    for (size_t i = 0; i < COLLECTION.size(); i++) \
        if (COLLECTION[i]ELEMENT_PATH == child) \
            { found = true; index = i; break; } \
    if (found == false) throw std::runtime_error("Enable to remove_child, no child"); \
    unsubscribe_child(child); \
    COLLECTION.erase(COLLECTION.begin() + index); \
    trigger_mutation(Composition); \
} \

class UIComposer_;
typedef UIComposer_ *UIComposer;

class UIComposer_ : public UIContainer_
{
public:
    virtual void remove_child(UIElement child) = 0;
};

#endif
