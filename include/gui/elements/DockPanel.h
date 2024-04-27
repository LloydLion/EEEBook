#ifndef GUI_ELEMENTS_DOCK_PANEL_H
#define GUI_ELEMENTS_DOCK_PANEL_H

#include "../cordinates.h"
#include "../UIStorage.h"

class DockPanel_;
typedef DockPanel_ *DockPanel;

struct DockElement
{
    UIElement ui;
    Point point;
};

DockElement fit_in_dock(UIElement element, Point point);

class DockPanel_ : public UIStorage_
{
private:
    std::vector<DockElement> _elements;
public:
    DockPanel_(std::vector<DockElement> elements);

    std::vector<UIElement>::iterator list_children() override;
    size_t count_children() override;
    void render(const GFX& gfx) override;
    Size min_size() override;
};

#endif
