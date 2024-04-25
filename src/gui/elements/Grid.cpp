#include "gui/elements/Grid.h"
#include <Arduino.h>

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


Grid_::Grid_(std::vector<GridRCDefinition> rows, std::vector<GridRCDefinition> columns, std::vector<GridElement> elements)
    : _rows(rows), _columns(columns), _elements(elements)
{
    
}

Size Grid_::min_size()
{
    //TODO: fix it
    return create_size(100, 100);
}

bool Grid_::is_updated()
{
    return !_is_drawn;
}

void Grid_::render(const GFX& gfx)
{
    if (_is_drawn and !_is_moved)
        return;

    Serial.println(1);

    cord_t rows_rs[2];
    cord_t columns_rs[2];

    Serial.println(2);

    calculate_real_sizes(gfx.size().width, columns_rs, GridRC::Column);
    calculate_real_sizes(gfx.size().height, rows_rs, GridRC::Row);

    cord_t rows_pos[2];
    cord_t columns_pos[2];

    Serial.println(_rows.size());
    Serial.println(_columns.size());

    Serial.println(3);

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

        GFX new_gfx = gfx.slice(create_bounds(
            create_point(columns_pos[c], rows_pos[r]),
            create_size(columns_rs[c], rows_rs[r])
        ));

        Serial.println(5);

        element.ui->render(new_gfx);
    }

    Serial.println(6);

    _is_moved = false;
    _is_drawn = true;
    Serial.println(7);
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
    //TODO: implement
    return 4;
}
