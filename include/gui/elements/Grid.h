#ifndef GUI_ELEMENTS_GRID_H
#define GUI_ELEMENTS_GRID_H

#include "../cordinates.h"
#include "../UIComposer.h"

#define MAX_GRID_SIZE 20

class Grid_;
typedef Grid_ *Grid;

enum GridRCSizeType : uint8_t
{
    Fixed,
    Auto,
    Proportional
};

struct GridRCDefinition
{
    cord_t size;
    GridRCSizeType size_type;
    cord_t min_size;
    cord_t max_size;
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

GridRCDefinition define_grid_rc_auto(cord_t min_size = 0);
GridRCDefinition define_grid_rc_fixed(cord_t size);
GridRCDefinition define_grid_rc_proportional(cord_t proportion = 1, cord_t min_size = 0, cord_t max_size = MAX_DIMENSION_SIZE);
GridElement fit_into_grid(UIElement element, size_t row, size_t column);

class Grid_ : public UIComposer_
{
private:
    struct ElementsLayout
    {
        cord_t rows_positions[MAX_GRID_SIZE - 1];
        cord_t columns_positions[MAX_GRID_SIZE - 1];

        inline cord_t get_row_position(size_t index) { return index == 0 ? 0 : rows_positions[index - 1]; }
        inline cord_t get_column_position(size_t index) { return index == 0 ? 0 : columns_positions[index - 1]; }

        inline cord_t get_row_size(size_t index, cord_t total_size)
        {
            cord_t next;
            if (index != MAX_GRID_SIZE && (next = get_row_position(index + 1)) != 0)
                return next - get_row_position(index);
            else return total_size - get_row_position(index);
        };

        inline cord_t get_column_size(size_t index, cord_t total_size)
        {
            cord_t next;
            if (index != MAX_GRID_SIZE && (next = get_column_position(index + 1)) != 0)
                return next - get_column_position(index);
            else return total_size - get_column_position(index);
        }
    };

    DEFINE_CACHE_SLOT(ElementsLayout, create_layout, Size viewport);
    DEFINE_CACHE_SLOT_ACCESSOR(ElementsLayout, create_layout, Size viewport);

    std::vector<GridRCDefinition> _rows, _columns;
    std::vector<GridElement> _elements;

    SelectIterator<VectorIterator<GridElement>, GridElement, UIElement> _iterator;
    
    cord_t get_auto_rc_size(size_t index, GridRC row_or_column);
    cord_t get_rc_elements_min_size(size_t index, GridRC row_or_column);
    cord_t get_rc_sum_size(GridRC row_or_column, bool calc_min = true);
    void calculate_real_sizes(cord_t full_size, cord_t *sizes, GridRC row_or_column);
    std::vector<GridRCDefinition> *get_definitions(GridRC row_or_column);

protected:
    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;

    void reset_cache(CacheChannel channel) override;

public:
    Grid_(std::vector<GridRCDefinition> rows, std::vector<GridRCDefinition> columns, std::vector<GridElement> elements);

    void c_notify_composition_mutation(UIElement element) override;

    Iterator<UIElement> *list_children() override;
    size_t count_children() override;
};

#endif
