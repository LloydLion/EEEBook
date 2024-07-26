#include "gui/drawing/PatternCatalog.h"
#include <stdexcept>
#include <cstring>

PatternFunctionId PatternCatalog_::find(const char *name) const
{
    blocked = true;
    for (size_t i = 0; i < _patterns.size(); i++)
        if (strcmp(name, _patterns[i].name) == 0)
            return i;
    
    throw std::runtime_error("No pattern function found with given name");
}

PatternFunction PatternCatalog_::get(PatternFunctionId id) const
{
    blocked = true;
    return _patterns[id];
}

PatternFunctionId PatternCatalog_::register_pattern(PatternFunction function)
{
    if (blocked)
        throw std::runtime_error("PatternCatalog is blocked, enable to register new patterns. Catalog blocks after any access operation");
    _patterns.push_back(function);
    return _patterns.size() - 1;
}

