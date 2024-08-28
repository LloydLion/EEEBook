#include "gui/render/UIView.h"

UIView_::UIView_(UIElement element): _element(element), _child_iterator(&_element)
{
    subscribe_child(element);
}

UIView_::UIView_(): UIView_(nullptr) { }

Iterator<UIElement> *UIView_::list_children()
{
    if (_element == nullptr)
        return EmptyIterator<UIElement>::instance();

    _child_iterator.reset();
    return &_child_iterator;
}

size_t UIView_::count_children()
{
    return _element == nullptr ? 0 : 1;
}

UIElement UIView_::get_element()
{
    return _element;
}

void UIView_::change_child(UIElement new_child)
{
    unsubscribe_child(_element);
    _element = new_child;
    subscribe_child(_element);
    trigger_mutation(Composition);
}
