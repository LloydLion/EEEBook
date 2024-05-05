#ifndef GUI_ELEMENTS_GRID_H
#define GUI_ELEMENTS_GRID_H

#include "../cordinates.h"
#include "../UIContainer.h"

class Grid_;
typedef Grid_ *Grid;

enum GridRCSizeType
{
    Fixed,
    Auto,
    Proportional
};

struct GridRCDefinition
{
    cord_t size;
    GridRCSizeType size_type;
};

enum GridRC
{
    Row,
    Column
};

struct GridElement
{
    UIElement ui;
    size_t row;
    size_t column;
};

GridRCDefinition define_grid_rc(GridRCSizeType size_type, cord_t size = 0);
GridElement fit_in_grid(UIElement element, size_t row, size_t column);

class Grid_ : public UIContainer_
{
private:
    std::vector<GridRCDefinition> _rows, _columns;
    std::vector<GridElement> _elements;

    SelectIterator<VectorIterator<GridElement>, GridElement, UIElement> _iterator;
    
    cord_t get_auto_size(size_t index, GridRC row_or_column);
    void calculate_real_sizes(cord_t full_size, cord_t *sizes, GridRC row_or_column);
    cord_t get_min_size_dimension(std::vector<GridRCDefinition> defenitions, GridRC type);

public:
    Grid_(std::vector<GridRCDefinition> rows, std::vector<GridRCDefinition> columns, std::vector<GridElement> elements);

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;
    void render(const GFX& gfx) override;
    Size min_size() override;
};

#endif
