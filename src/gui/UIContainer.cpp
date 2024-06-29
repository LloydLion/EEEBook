#include "gui/UIContainer.h"

Size UIContainer_::child_max_size(UIElement element)
{
    if (element->visibility() == UIVisibility::Collapsed)
        return Size();
    return _p_padding.expand(element->max_size());
}

Size UIContainer_::child_min_size(UIElement element)
{
    if (element->visibility() == UIVisibility::Collapsed)
        return Size();
    return _p_padding.expand(element->min_size());
}

void UIContainer_::render_child(UIElement element, GFX gfx)
{
    return element->render(gfx.slice(_p_padding));
}

void UIContainer_::render_child(UIElement element, GFX gfx, Bounds viewport)
{
    return element->render(gfx.slice(_p_padding.cast(viewport)));
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

void UIContainer_::padding(PaddingSize value)
{
    trigger_mutation(CompositionState);
    _p_padding = value;
}

void UIContainer_::c_notify_composition_mutation(UIElement child)
{
    
}
