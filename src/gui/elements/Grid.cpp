#include "gui/elements/Grid.h"
#include <exception>
#include <Arduino.h>

GridRCDefinition define_grid_rc_auto(cord_t min_size)
{
    GridRCDefinition rc;
    rc.size_type = GridRCSizeType::Auto;
    rc.min_size = min_size;

    return rc;
}

GridRCDefinition define_grid_rc_fixed(cord_t size)
{
    GridRCDefinition rc;
    rc.size_type = GridRCSizeType::Fixed;
    rc.size = size;

    return rc;
}

GridRCDefinition define_grid_rc_proportional(cord_t proportion, cord_t min_size, cord_t max_size)
{
    GridRCDefinition rc;
    rc.size_type = GridRCSizeType::Proportional;
    rc.size = proportion;
    rc.min_size = min_size;
    rc.max_size = max_size;

    return rc;
}


GridElement fit_into_grid(UIElement element, size_t row, size_t column)
{
    GridElement el;
    el.ui = element;
    el.row = row;
    el.column = column;
    return el;
}


UIElement &selector(GridElement &el) { return el.ui; }

Grid_::Grid_(std::vector<GridRCDefinition> rows, std::vector<GridRCDefinition> columns, std::vector<GridElement> elements):
    _elements(elements), _iterator(VectorIterator<GridElement>(_elements), selector)
{
    if (rows.size() > MAX_GRID_SIZE)
        throw std::runtime_error("Rows are so many, max rows count is determined by MAX_GRID_SIZE define in Grid.h");
        
    if (columns.size() > MAX_GRID_SIZE)
        throw std::runtime_error("Columns are so many, max columns count is determined by MAX_GRID_SIZE define in Grid.h");

    _rows = rows;
    _columns = columns;
}

Size Grid_::i_min_size()
{
    auto size = Size(get_rc_sum_size(GridRC::Column), get_rc_sum_size(GridRC::Row));
    return size;
}

Size Grid_::i_max_size()
{
    auto size = Size(get_rc_sum_size(GridRC::Column, false), get_rc_sum_size(GridRC::Row, false));
    return size;
}

Iterator<UIElement> *Grid_::list_children()
{
    _iterator.reset();
    return &_iterator;
}

size_t Grid_::count_children()
{
    return _elements.size();
}

void Grid_::reset_cache(CacheChannel channel)
{
    CONNECT_CACHE_CHANNEL(create_layout, CacheChannel::Render | CacheChannel::Composition);

    this->UIComposer_::reset_cache(channel);
}

void Grid_::c_notify_composition_mutation(UIElement element)
{
    reset_cache(CacheChannel::Render);
}

void Grid_::i_render(const GFX& gfx)
{
    gfx.fill_screen(background_color());

    ElementsLayout e_layout = create_layout(gfx.size());

    for (auto element : _elements)
    {
        size_t r = element.row;
        size_t c = element.column;

        GFX new_gfx = assume_padding(gfx.slice(LocalBounds(
            e_layout.get_column_position(c), e_layout.get_row_position(r),
            e_layout.get_column_size(c, gfx.size().width), e_layout.get_row_size(r, gfx.size().height)
        )));

        element.ui->render(new_gfx);
    }
}

IMPLEMENT_CACHE_SLOT(Grid_::ElementsLayout, Grid_, create_layout, (Size viewport), (viewport))
{
    cord_t rows_rs[MAX_GRID_SIZE];
    cord_t columns_rs[MAX_GRID_SIZE];
    memset(&rows_rs, 0, MAX_GRID_SIZE * sizeof(cord_t));
    memset(&columns_rs, 0, MAX_GRID_SIZE * sizeof(cord_t));

    calculate_real_sizes(viewport.width, columns_rs, GridRC::Column);
    calculate_real_sizes(viewport.height, rows_rs, GridRC::Row);

    ElementsLayout layout;
    memset(&layout.rows_positions, 0, (MAX_GRID_SIZE - 1) * sizeof(cord_t));
    memset(&layout.columns_positions, 0, (MAX_GRID_SIZE - 1) * sizeof(cord_t));

    UI_PRINT_SELF;
    Serial.println("Printing grid layout:");
    Serial.print("\tRows:");
    Serial.printf(" [v-v%d, ->%d]", rows_rs[0], layout.get_row_position(0));
    for (size_t row = 1; row < _rows.size(); row++)
    {
        layout.rows_positions[row - 1] += layout.get_row_position(row - 1) + rows_rs[row - 1];
        Serial.printf(" [v-v%d, ->%d]", rows_rs[row], layout.get_row_position(row));
    }

    Serial.println();
    Serial.print("\tColumns:");
    Serial.printf(" [v-v%d, ->%d]", columns_rs[0], layout.get_column_position(0));
    for (size_t column = 1; column < _columns.size(); column++)
    {
        layout.columns_positions[column - 1] += layout.get_column_position(column - 1) + columns_rs[column - 1];
        Serial.printf(" [v-v%d, ->%d]", columns_rs[column], layout.get_column_position(column));
    }
    Serial.println();

    return layout;
}

void Grid_::calculate_real_sizes(cord_t full_size, cord_t *sizes, GridRC row_or_column)
{
    std::vector<GridRCDefinition> *target_collection = get_definitions(row_or_column);
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
            sizes[i] = get_auto_rc_size(i, row_or_column);
        }

        rest_space -= sizes[i];
    }

proportional_reset:
    cord_t proportional_sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        auto definition = target_collection->operator[](i);
        if (sizes[i] == 0 && definition.size_type == GridRCSizeType::Proportional)
        {
            if (definition.size == 0)
                throw std::runtime_error("Grid's proportional column/row with 0 size detected");
            proportional_sum += definition.size;
        }
    }

    if (proportional_sum != 0)
    {
        cord_t size_per_prop = rest_space / proportional_sum;

        for (size_t i = 0; i < size; i++) //Restricted rc processing
        {
            auto definition = target_collection->operator[](i);
            if (sizes[i] == 0 && definition.size_type == GridRCSizeType::Proportional)
            {
                cord_t prop_size = definition.size * size_per_prop;

                if (prop_size > definition.max_size)
                {
                    sizes[i] = definition.max_size;
                    rest_space -= definition.max_size;
                    goto proportional_reset;
                }
                else if (prop_size < definition.min_size)
                {
                    if (rest_space < definition.min_size)
                        throw std::runtime_error("No space rest for Grid proportional rc, min_size hit");
                    sizes[i] = definition.min_size;
                    rest_space -= definition.min_size;
                    goto proportional_reset;
                }
            }
        }

        for (size_t i = 0; i < size; i++) //Unrestricted rc processing
        {
            auto definition = target_collection->operator[](i);
            if (sizes[i] == 0 && definition.size_type == GridRCSizeType::Proportional)
            {
                cord_t prop_size = definition.size * size_per_prop;
                sizes[i] = prop_size;
                rest_space -= prop_size;
            }
        }
    }

    bool has_free_scale_rc = false;
    for (size_t i = 0; rest_space != 0; i++)
    {
        size_t abs_i = i % size;
        auto definition = target_collection->operator[](abs_i);

        if (definition.size_type == GridRCSizeType::Proportional && (definition.max_size == 0 || sizes[abs_i] < definition.max_size))
        {
            sizes[abs_i] += 1;
            rest_space -= 1;
            has_free_scale_rc = true;
        }

        if (abs_i == size - 1)
        {
            if (has_free_scale_rc == false)
                throw std::runtime_error("No free scale rc in Grid available for rest space distribution");
            has_free_scale_rc = false;
        }
    }
}

cord_t Grid_::get_rc_elements_min_size(size_t index, GridRC row_or_column)
{
    GridRCDefinition definition = get_definitions(row_or_column)->operator[](index);
    cord_t result = 0;

    for (auto el : _elements)
    {
        if (row_or_column == GridRC::Column)
        {
            if (el.column == index)
                result = max(result, padding().expand(el.ui->min_size()).width);
        }
        else //GridRC::Row
        {
            if (el.row == index)
                result = max(result, padding().expand(el.ui->min_size()).height);
        }
    }

    result = max(result, definition.min_size);

    return result;
}

cord_t Grid_::get_rc_sum_size(GridRC type, bool min)
{
    std::vector<GridRCDefinition> *definitions = get_definitions(type);

    cord_t size = 0;

    for (size_t i = 0; i < definitions->size(); i++)
    {
        GridRCDefinition definition = definitions->operator[](i);
        switch (definition.size_type)
        {
        case GridRCSizeType::Auto:
            size += get_auto_rc_size(i, GridRC::Column);
            break;

        case GridRCSizeType::Fixed:
            size += definition.size;
            break;
        
        case GridRCSizeType::Proportional:
            if (min)
                size += definition.min_size;
            else
            {
                if (definition.max_size != MAX_DIMENSION_SIZE)
                    size += definition.max_size;
                else return MAX_DIMENSION_SIZE;
            }
            break;
        }
    }

    return size;
}

cord_t Grid_::get_auto_rc_size(size_t index, GridRC row_or_column)
{
    cord_t min_size = get_definitions(row_or_column)->operator[](index).min_size;

    return max(get_rc_elements_min_size(index, row_or_column), min_size);
}

std::vector<GridRCDefinition> *Grid_::get_definitions(GridRC row_or_column)
{
    if (row_or_column == Row)
        return &_rows;
    else return &_columns;
}
