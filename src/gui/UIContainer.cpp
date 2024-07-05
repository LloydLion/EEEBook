#include "gui/UIContainer.h"

Size UIContainer_::child_max_size(UIElement element)
{
    if (element == nullptr or element->visibility() == UIVisibility::Collapsed)
        return Size();
    return _p_padding.expand(element->max_size());
}

Size UIContainer_::child_min_size(UIElement element)
{
    if (element == nullptr or element->visibility() == UIVisibility::Collapsed)
        return Size();
    return _p_padding.expand(element->min_size());
}

void UIContainer_::render_child(UIElement element, GFX gfx)
{
    Serial.println(3);
    if (element != nullptr and element->visibility() == UIVisibility::Visible)
    {
        Serial.println(4);
        element->render(gfx.slice(_p_padding));
    }
    else Serial.println(5);
}

void UIContainer_::render_child(UIElement element, GFX gfx, Bounds viewport)
{
    if (element != nullptr and element->visibility() == UIVisibility::Visible)
        element->render(gfx.slice(_p_padding.cast(viewport)));
}

void UIContainer_::subscribe_all_children()
{
    Iterator<UIElement> *iterator = list_children();

    while (iterator->next())
    {
        UIElement &value = iterator->current();
        UIElement value2 = value;
        subscribe_child(value2);

    }
}

void UIContainer_::subscribe_child(UIElement element)
{
    if (element != nullptr)
        element->bind_parent(this);
}

void UIContainer_::unsubscribe_child(UIElement element)
{
    if (element != nullptr)
        element->unbind_parent(this);
}

void UIContainer_::c_notify_composition_changed(UIElement child)
{
    trigger_mutation(ChildComposition);    
}

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(UIContainer_, PaddingSize, padding, Composition)
