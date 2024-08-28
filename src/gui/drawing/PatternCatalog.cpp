#include "gui/drawing/PatternCatalog.h"
#include <stdexcept>
#include <cstring>

uint8_t fill_pattern_function(CoordinateRangeValue a, CoordinateRangeValue b, UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters, PatternState<GUI_PATTERN_STATE_SIZE> *cache)
{
    return 0;
}

PatternFunctionId PatternCatalog_::find(const char *name) const
{
    blocked = true;
    if (strcmp(name, "fill") == 0)
        return 0;

    for (size_t i = 0; i < _patterns.size(); i++)
        if (strcmp(name, _patterns[i].name) == 0)
            return i + 1;
    
    throw std::runtime_error("No pattern function found with given name");
}

PatternFunction PatternCatalog_::get(PatternFunctionId id) const
{
    blocked = true;
    if (id == 0)
        return PatternFunction(fill_pattern_function, "fill");
    return _patterns[id - 1];
}

PatternFunctionId PatternCatalog_::register_pattern(PatternFunction function)
{
    if (blocked)
        throw std::runtime_error("PatternCatalog is blocked, enable to register new patterns. Catalog blocks after any access operation");
    _patterns.push_back(function);
    return _patterns.size();
}

