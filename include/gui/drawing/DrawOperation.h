#ifndef GUI_DRAWING_DRAW_OPERATION_H
#define GUI_DRAWING_DRAW_OPERATION_H

#include "gui/coordinates.h"
#include "gui/color.h"
#include "gui/drawing/Pattern.h"
#include "std/byte.h"
#include "gui/drawing/Bitmap.h"
#include <cstring>

#define DRAW_AREA_MAX_SEGMENTATION 4

enum class DrawAreaType : uint8_t
{
    Rect,
    Ellipse,
    Line,
    Text,
    Bitmap
};

union DrawAreaArgs
{
    struct
    {
        cord_t thickness;
    } rect;

    struct
    {
        const char *text;
        size_t limit;
        FontId font;
    } text;

    struct
    {
        cord_t thickness;
        cord_t virtual_size_w, virtual_size_h;
        cord_t mask_position_x, mask_position_y;
    } ellipse;

    struct
    {
        cord_t thickness;
        bool is_anti_diagonal;
    } line;

    struct
    {
        byte *map;
        Bitmap::Flags flags;
    } bitmap;
};

enum class CoordinateSystemType : uint8_t
{
    Classic,
    PathBased
};

struct DrawOperation
{
    DrawAreaType area_type;
    DrawAreaArgs arguments;
    CoordinateSystemType coordinate;
    Pattern patterns[DRAW_AREA_MAX_SEGMENTATION];
    Bounds bounds;
    
    inline DrawOperation()
    {
        std::memset(this, 0, sizeof(DrawOperation));
    }
};

#endif
