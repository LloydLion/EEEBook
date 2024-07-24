#ifndef GUI_DRAWING_PATTERN_CATALOG_H
#define GUI_DRAWING_PATTERN_CATALOG_H

#include "gui/drawing/PatternFunction.h"
#include <vector>

class PatternCatalog
{
private:
    std::vector<PatternFunction> _patterns = { };
    mutable bool blocked = false;

public:
    static PatternCatalog *instance();

    PatternFunctionId find(const char *name) const;
    PatternFunction get(PatternFunctionId id) const;
    PatternFunctionId register_pattern(PatternFunction function);
};

#endif
