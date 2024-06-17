#include "gui/elements/StackPanel.h"

StackPanel_::StackPanel_(std::vector<UIElement> elements):
    _elements(elements), _elements_iterator(elements)
{

}

void StackPanel_::orientation(Axis value)
{
    trigger_mutation(MutationType::CompositionState);
    _p_orientation = value;
}

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
        Size min_size = ui->min_size();
        cord_t required_space = min_size[_p_orientation];

        ui->render(gfx.slice(LocalBounds(
            LocalVector(p_offset, 0, _p_orientation),
            gfx.size().with(required_space, _p_orientation)
        )));

        p_offset += required_space;
    }
}

Size StackPanel_::i_min_size()  
{
    Size result = Size();

    for (auto ui : _elements)
    {
        Size el_size = padding().expand(ui->min_size());
        result = Size::concat(result, el_size, _p_orientation);
    }

    return result;
}

Size StackPanel_::i_max_size()
{
    return Size::max_size();
}
