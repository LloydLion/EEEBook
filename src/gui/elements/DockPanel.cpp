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
    //TODO: write min size
    return Size(100, 100);
}

const Iterator<UIElement> *DockPanel_::list_children()
{
    _iterator.reset();
    return &_iterator;
}

size_t DockPanel_::count_children() { return _elements.size(); }