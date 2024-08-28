#ifndef GUI_DRAWING_DRAWING_CONTEXT_H
#define GUI_DRAWING_DRAWING_CONTEXT_H

#include "gui/drawing/PatternCatalog.h"
#include "gui/drawing/fonts/FontEngine.h"

struct DrawingContext
{
    FontEngine font_engine;
    PatternCatalog pattern_catalog;

    static const DrawingContext &instance();
    static void initialize();
};


#endif
