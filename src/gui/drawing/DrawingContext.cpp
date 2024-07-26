#include "gui/drawing/DrawingContext.h"

DrawingContext _instance;

void DrawingContext::initialize()
{
    _instance.font_engine = new FontEngine_();
    _instance.pattern_catalog = new PatternCatalog_();
}

const DrawingContext &DrawingContext::instance()
{
    return _instance;
}
