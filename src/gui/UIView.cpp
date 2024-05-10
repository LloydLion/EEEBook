#include "gui/UIView.h"

UIView_::UIView_(UIElement element): _element(element), _child_iterator(&_element)
{

}

Iterator<UIElement> *UIView_::list_children()
{
    _child_iterator.reset();
    return &_child_iterator;
}

size_t UIView_::count_children()
{
    return 1;
}

UIElement UIView_::get_element()
{
    return _element;
}
