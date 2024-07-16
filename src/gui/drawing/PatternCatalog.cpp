#include "gui/drawing/PatternCatalog.h"
#include <stdexcept>
#include <cstring>

PatternCatalog global_instance;

PatternCatalog *PatternCatalog::instance()
{
    return &global_instance;
}

PatternFunctionId PatternCatalog::find(const char *name)
{
    blocked = true;
    for (size_t i = 0; i < _patterns.size(); i++)
        if (strcmp(name, _patterns[i].name) == 0)
            return i;
    
    throw std::runtime_error("No pattern function found with given name");
}

PatternFunction PatternCatalog::get(PatternFunctionId id)
{
    blocked = true;
    return _patterns[id];
}

PatternFunctionId PatternCatalog::register_pattern(PatternFunction function)
{
    if (blocked)
        throw std::runtime_error("PatternCatalog is blocked, enable to register new patterns. Catalog blocks after any access operation");
    _patterns.push_back(function);
    return _patterns.size() - 1;
}

