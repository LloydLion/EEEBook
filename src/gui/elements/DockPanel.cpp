#include "gui/elements/DockPanel.h"

DockElement fit_in_dock(UIElement element, Point point)
{
    DockElement dock_element;
    dock_element.ui = element;
    dock_element.point = point;
    return dock_element;
}

DockPanel_::DockPanel_(std::vector<DockElement> elements): _elements(elements) {}

std::vector<UIElement>::iterator DockPanel_::list_children()
{

}
size_t DockPanel_::count_children()
{
    return _elements.size();
}
void DockPanel_::render(const GFX& gfx)
{
    //TODO: ADD min size
    Point corner = create_point(gfx.size().width, gfx.size().height);

    for (auto el : _elements)
    {
        GFX new_gfx = gfx.slice(create_bounds(el.point, corner));
        el.ui->render(new_gfx);
    }
}
Size DockPanel_::min_size()
{
    return create_size(100, 100);
}