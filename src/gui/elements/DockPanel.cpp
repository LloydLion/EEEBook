#include "gui/elements/DockPanel.h"

DockElement fit_into_dock(UIElement element, Vector point)
{
    DockElement dock_element;
    dock_element.ui = element;
    dock_element.point = point;
    return dock_element;
}

UIElement &selector(DockElement &el) { return el.ui; }

DockPanel_::DockPanel_(std::vector<DockElement> elements):
    _elements(elements), _vec_it(_elements), _iterator(&_vec_it, selector)
{ }

void DockPanel_::render(const GFX& pgfx)
{
    ASSUME_MARGIN(pgfx);

    Vector corner = Vector(gfx.size().width, gfx.size().height);
    for (auto el : _elements)
    {
        GFX new_gfx = assume_padding(gfx.slice(Bounds(el.point, corner)));
        el.ui->render(new_gfx);
    }
}

Size DockPanel_::min_size()
{
    cord_t max_width, max_height;
    for( auto el : _elements)
    {   
        cord_t current_width, current_height;
        current_width = el.point.x + el.ui->min_size().width;
        current_height = el.point.y + el.ui->min_size().height;

        max_width = max_width < current_width ? current_width : max_width;
        max_height = max_height < current_height ? current_height : max_height; 
    }
    return Size(max_width, max_height);
}

const Iterator<UIElement> *DockPanel_::list_children()
{
    _iterator.reset();
    return &_iterator;
}

size_t DockPanel_::count_children() { return _elements.size(); }