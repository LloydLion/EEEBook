#ifndef GUI_ELEMENTS_DOCK_PANEL_H
#define GUI_ELEMENTS_DOCK_PANEL_H

#include "../cordinates.h"
#include "../UIComposer.h"
#include "std/iterator.h"

class DockPanel_;
typedef DockPanel_ *DockPanel;

struct DockElement
{
    struct CornerPostion
    {
        Corner origin;
        LocalVector offset;

        CornerPostion() { }
    };

    struct SidePosition
    {
        Side origin;
        cord_t offset;

        SidePosition() { }
    };

    enum PositionType
    {
        Corner,
        Side
    };

    union Position
    {
        CornerPostion corner;
        SidePosition side;

        Position() { }
    };

    PositionType type;
    Position position;
    UIElement ui;

    DockElement() { }
};

DockElement fit_into_dock(UIElement element, Corner origin, Vector point);
DockElement fit_into_dock(UIElement element, Side origin, cord_t offset);

class DockPanel_ : public UIComposer_
{
private:
    std::vector<DockElement> _elements;

    SelectIterator<VectorIterator<DockElement>, DockElement, UIElement> _iterator;

public:
    DockPanel_(std::vector<DockElement> elements);

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;
};

#endif
