#ifndef GUI_DRAWING_DRAW_OPERATION_H
#define GUI_DRAWING_DRAW_OPERATION_H

#include "gui/coordinates.h"
#include "gui/color.h"
#include "gui/drawing/Pattern.h"

#define DRAW_AREA_MAX_SEGMENTATION 4

enum class DrawAreaType : uint8_t
{
    Rect,
    Ellipse,
    Line,
    Text
};

union DrawAreaArgs
{
    struct {
        cord_t thickness;
    } rect;
    
    struct {
        const char *text;
        size_t limit;
    } text;
    
    struct {
        cord_t thickness;
        Size virtual_size;
        Vector mask_position;
    } ellipse;

    struct {
        cord_t thickness;
        bool is_anti_diagonal;
    } line;
};

enum class CoordinateSystemType : uint8_t
{
    Classic,
    PathBased
};

struct DrawOperation
{
    DrawAreaType area;
    DrawAreaArgs arguments;
    CoordinateSystemType coordinate;
    Pattern patterns[DRAW_AREA_MAX_SEGMENTATION];
    Bounds bounds;
};

#endif
