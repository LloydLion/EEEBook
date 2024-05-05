#ifndef GUI_ELEMENTS_DOCK_PANEL_H
#define GUI_ELEMENTS_DOCK_PANEL_H

#include "../cordinates.h"
#include "../UIContainer.h"
#include "std/iterator.h"

class DockPanel_;
typedef DockPanel_ *DockPanel;

struct DockElement
{
    UIElement ui;
    Vector point;
};

DockElement fit_into_dock(UIElement element, Vector point);

class DockPanel_ : public UIContainer_
{
private:
    std::vector<DockElement> _elements;

    SelectIterator<DockElement, UIElement> _iterator;
    VectorIterator<DockElement> _vec_it;

public:
    DockPanel_(std::vector<DockElement> elements);

    void render(const GFX& gfx) override;
    Size min_size() override;

    const Iterator<UIElement> *list_children() override;
    size_t count_children() override;
};

#endif
