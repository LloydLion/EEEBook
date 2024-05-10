#include "gui/UIContainer.h"

GFX UIContainer_::assume_padding(const GFX &gfx)
{
    return gfx.slice(padding);
}

void UIContainer_::subscribe_all_children()
{
    Iterator<UIElement> *iterator = list_children();

    while (iterator->next())
        subscribe_child(iterator->current());
}

void UIContainer_::subscribe_child(UIElement element)
{
    element->bind_parent(this);
}

void UIContainer_::unsubscribe_child(UIElement element)
{
    element->unbind_parent(this);
}

void UIContainer_::c_notify_composition_mutation(UIElement child)
{
    
}
