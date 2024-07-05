#include "gui/elements/StackPanel.h"

StackPanel_::StackPanel_(std::vector<UIElement> elements):
    _elements(elements), _elements_iterator(&elements)
{
    subscribe_all_children();
    finish_initialization();
}

DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(StackPanel_, Axis, orientation, Composition);
DEFAULT_PROPERTY_SETTER_IMPLEMENTATION_P(StackPanel_, cord_t, spacing, Composition);

Iterator<UIElement> *StackPanel_::list_children()
{
    _elements_iterator.reset();
    return &_elements_iterator;
}

size_t StackPanel_::count_children()
{
    return _elements.size();
}

void StackPanel_::i_render(const GFX& gfx)
{
    cord_t p_offset = 0;

    for (auto ui : _elements)
    {
        Size min_size = child_min_size(ui);
        cord_t required_space = min_size[_p_orientation];

        render_child(ui, gfx, LocalBounds(
            LocalVector(p_offset, 0, _p_orientation),
            gfx.size().with(required_space, _p_orientation)
        ));

        p_offset += required_space;
        if (ui->visibility() != UIVisibility::Collapsed) p_offset += _p_spacing;
    }
}

Size StackPanel_::i_min_size()  
{
    Size result = Size();
    size_t non_collapsed_counter = 0;

    for (auto ui : _elements)
    {
        if (ui->visibility() != UIVisibility::Collapsed)
            non_collapsed_counter += 1;

        Size el_size = child_min_size(ui);
        result = Size::concat(result, el_size, _p_orientation);
    }

    if (non_collapsed_counter != 0)
        result.with(result[_p_orientation] + (_p_spacing * (non_collapsed_counter - 1)), _p_orientation);

    return result;
}

Size StackPanel_::i_max_size()
{
    return Size::max_size();
}

void StackPanel_::add_child(UIElement child, size_t index)
{
    subscribe_child(child);
    _elements.insert(_elements.begin() + index, child);
    trigger_mutation(Composition);
}

void StackPanel_::modify_child(UIElement child, size_t index)
{
    for (size_t i = 0; i < _elements.size(); i++)
        if (_elements[i] == child)
        {
            _elements.erase(_elements.begin() + i);
            break;
        }
        
    _elements.insert(_elements.begin() + index, child);
}

void StackPanel_::swap_children(UIElement child_a, UIElement child_b)
{
    size_t index_a, index_b;
    bool found_a = false, found_b = false;
    for (size_t i = 0; i < _elements.size(); i++)
    {
        if (_elements[i] == child_a)
        {
            index_a = i;
            found_a = true;
        }

        if (_elements[i] == child_b)
        {
            index_b = i;
            found_b = true;
        }

        if (found_a and found_b)
            break;
    }

    if (found_a == false)
        throw std::runtime_error("No 'child_a' in StackPanel for swap");
    if (found_b == false)
        throw std::runtime_error("No 'child_b' in StackPanel for swap");

    
    _elements[index_b] = child_a;
    _elements[index_a] = child_b;
}

DEFAULT_REMOVE_CHILD_IMPLEMENTATION(StackPanel_, _elements, );
