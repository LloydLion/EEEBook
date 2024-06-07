#include "gui/elements/DockPanel.h"

DockElement fit_into_dock(UIElement element, Corner origin, Vector point)
{
    DockElement el;
    el.ui = element;
    el.type = DockElement::PositionType::Corner;
    el.position.corner.origin = origin;
    el.position.corner.offset = point;
    return el;
}

DockElement fit_into_dock(UIElement element, Side origin, cord_t offset)
{
    DockElement el;
    el.ui = element;
    el.type = DockElement::PositionType::Side;
    el.position.side.origin = origin;
    el.position.side.offset = offset;
    return el;
}


UIElement &selector(DockElement &el) { return el.ui; }

DockPanel_::DockPanel_(std::vector<DockElement> elements):
    _elements(elements), _iterator(VectorIterator<DockElement>(_elements), selector)
{ }

void DockPanel_::i_render(const GFX& gfx)
{
    gfx.fill_screen(background_color());

    PaddingSize padding = this->padding();
    Size dock_size = gfx.size();
    LocalVector dock_end = dock_size.start_to_end();

    for (auto el : _elements)
    {
        LocalBounds new_gfx_bounds;
        Size max_size = padding.safe_expand(el.ui->max_size());

        if (el.type == DockElement::PositionType::Side)
        {
            DockElement::SidePosition side_position = el.position.side;
            cord_t offset = side_position.offset;
            
            if (side_position.origin == Side::Up)
                new_gfx_bounds = LocalBounds(LocalVector(0, offset), Size(dock_size.width, min(dock_size.height, max_size.height)));
            else if (side_position.origin == Side::Left)
                new_gfx_bounds = LocalBounds(LocalVector(offset, 0), Size(min(dock_size.width, max_size.width), dock_size.height));
            else if (side_position.origin == Side::Down)
                new_gfx_bounds = LocalBounds(LocalVector(0, max(0LL, (int64_t)dock_size.height - max_size.height - offset)), dock_end - LocalVector(0, offset));
            else //if (side_position.origin == Side::Right)
                new_gfx_bounds = LocalBounds(LocalVector(max(0LL, (int64_t)dock_size.width - max_size.width - offset), 0), dock_end - LocalVector(offset, 0));
        }
        else if (el.type == DockElement::PositionType::Corner)
        {
            DockElement::CornerPostion corner_position = el.position.corner;
            LocalVector offset = corner_position.offset;
            Size size = Size::intersect(dock_size - offset, max_size);

            if (corner_position.origin == Corner::LeftUp)
                new_gfx_bounds = LocalBounds(offset, size);
            else if (corner_position.origin == Corner::LeftDown)
                new_gfx_bounds = LocalBounds(LocalVector(offset.x, dock_size.height - offset.y - size.height), size);
            else if (corner_position.origin == Corner::RightUp)
                new_gfx_bounds = LocalBounds(LocalVector(dock_size.width - offset.x - size.width, offset.y), size);
            else //if (corner_position.origin == Corner::RightDown)
                new_gfx_bounds = LocalBounds(LocalVector(dock_size.width - offset.x - size.width, dock_size.height - offset.y - size.height), size);
        }
        else throw std::runtime_error("Invalid dock element type");

        UI_PRINT_SELF; Serial.printf("new_gfx_bounds: Bounds(%d, %d, %d, %d)", new_gfx_bounds.start.x, new_gfx_bounds.start.y, new_gfx_bounds.size.width, new_gfx_bounds.size.height);
        el.ui->render(gfx.slice(new_gfx_bounds));
    }
}

Size DockPanel_::i_min_size()
{
    Size result;

    for (auto el : _elements)
    {
        Size el_size = el.ui->min_size();
        Size required_size;

        if (el.type == DockElement::PositionType::Side)
        {
            cord_t offset = el.position.side.offset;
            int origin_orientation = (int)el.position.side.origin & 1;
            required_size = el_size + LocalVector(offset * !origin_orientation, offset * origin_orientation);
        }
        else if (el.type == DockElement::PositionType::Corner)
        {
            required_size = el_size + el.position.corner.offset;
        }
        else throw std::runtime_error("Invalid dock element type");

        result = Size::combine(result, required_size);
    }

    return padding().expand(result);
}

Size DockPanel_::i_max_size()
{
    return Size::max_size();
}

Iterator<UIElement> *DockPanel_::list_children()
{
    _iterator.reset();
    return &_iterator;
}

size_t DockPanel_::count_children() { return _elements.size(); }