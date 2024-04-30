#include "gui/elements/Grid.h"
#include <exception>
#include <Arduino.h>

#define MAX_GRID_SIZE 20

GridRCDefinition define_grid_rc(GridRCSizeType size_type, cord_t size)
{
    GridRCDefinition d;
    d.size_type = size_type;
    d.size = size;
    return d;
}

GridElement fit_in_grid(UIElement element, size_t row, size_t column)
{
    GridElement el;
    el.ui = element;
    el.row = row;
    el.column = column;
    return el;
}


Grid_::Grid_(std::vector<GridRCDefinition> rows, std::vector<GridRCDefinition> columns, std::vector<GridElement> elements): _elements(elements)
{
    if (rows.size() > MAX_GRID_SIZE)
        throw std::runtime_error("Rows are so many, max rows count is determined by MAX_GRID_SIZE define in Grid.cpp");
        
    if (columns.size() > MAX_GRID_SIZE)
        throw std::runtime_error("Columns are so many, max columns count is determined by MAX_GRID_SIZE define in Grid.cpp");

    _rows = rows;
    _columns = columns;
}

Size Grid_::min_size()
{
    //TODO: fix it
    return Size(100, 100);
}

void Grid_::render(const GFX& gfx)
{
    cord_t rows_rs[MAX_GRID_SIZE];
    cord_t columns_rs[MAX_GRID_SIZE];
    memset(&rows_rs, 0, MAX_GRID_SIZE * sizeof(cord_t));
    memset(&columns_rs, 0, MAX_GRID_SIZE * sizeof(cord_t));

    calculate_real_sizes(gfx.size().width, columns_rs, GridRC::Column);
    calculate_real_sizes(gfx.size().height, rows_rs, GridRC::Row);

    cord_t rows_pos[MAX_GRID_SIZE];
    cord_t columns_pos[MAX_GRID_SIZE];
    memset(&rows_pos, 0, MAX_GRID_SIZE * sizeof(cord_t));
    memset(&columns_pos, 0, MAX_GRID_SIZE * sizeof(cord_t));

    rows_pos[0] = 0;
    for (size_t i = 1; i <= _rows.size(); i++)
        rows_pos[i] += rows_pos[i - 1] + rows_rs[i - 1];

    columns_pos[0] = 0;
    for (size_t i = 1; i <= _columns.size(); i++)
        columns_pos[i] += columns_pos[i - 1] + columns_rs[i - 1];

    for (auto element : _elements)
    {
        size_t r = element.row;
        size_t c = element.column;

        GFX new_gfx = gfx.slice(Bounds(
            columns_pos[c], rows_pos[r],
            columns_rs[c], rows_rs[r]
        ));

        element.ui->render(new_gfx);
    }
}

void Grid_::calculate_real_sizes(cord_t full_size, cord_t *sizes, GridRC row_or_column)
{
    std::vector<GridRCDefinition> *target_collection = nullptr;
    if (row_or_column == GridRC::Row)
        target_collection = &_rows;
    else if (row_or_column == GridRC::Column)
        target_collection = &_columns;

    size_t size = target_collection->size();


    cord_t rest_space = full_size;

    for (size_t i = 0; i < size; i++)
    {
        auto definition = target_collection->operator[](i);
        if (definition.size_type == GridRCSizeType::Fixed)
        {
            sizes[i] = definition.size;
        }
        else if (definition.size_type == GridRCSizeType::Auto)
        {
            sizes[i] = get_auto_size(i, row_or_column);
        }

        rest_space -= sizes[i];
    }

    cord_t proportional_sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        auto definition = target_collection->operator[](i);;
        if (definition.size_type == GridRCSizeType::Proportional)
        {
            proportional_sum += definition.size;
        }
    }

    cord_t size_per_prop = rest_space / proportional_sum;
    rest_space -= size_per_prop * proportional_sum;

    for (size_t i = 0; i < size; i++)
    {
        auto definition = target_collection->operator[](i);;
        if (definition.size_type == GridRCSizeType::Proportional)
        {
            sizes[i] = definition.size * size_per_prop;
        }
    }

    for (size_t i = 0; rest_space != 0; i++)
    {
        sizes[i % size] += 1;
        rest_space -= 1;
    }
}

cord_t Grid_::get_auto_size(size_t index, GridRC row_or_column)
{
    //TODO: implement it
    return 20;
}

std::vector<UIElement>::iterator Grid_::list_children()
{
    //TODO: implement
    throw 0;
}

size_t Grid_::count_children()
{
    return _elements.size();
}
