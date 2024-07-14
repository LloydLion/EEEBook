#ifndef GUI_DRAWING_DRAW_OPERATION_H
#define GUI_DRAWING_DRAW_OPERATION_H

#include "gui/coordinates.h"
#include "gui/color.h"

enum class DrawOperationType
{
    Rect,
    Text,
    Ellipse,
    Line,
    Bitmap
};

union DrawOperationArgs
{
    struct Rect
    {
        cord_t thickness;
    };
    
    struct Text
    {
        const char *text;
        size_t limit;
    };
    
    struct Ellipse
    {
        cord_t thickness;
        Size virtual_size;
        Vector mask_position;
    };

    struct Line
    {
        cord_t thickness;
        bool is_anti_diagonal;
    };

    struct Bitmap
    {
        std::byte *data;
    };
    

    Rect rect;
    Text text;
    Ellipse ellipse;
    Line line;
    Bitmap bitmap;
};

struct DrawOperation
{
    DrawOperationType type;
    DrawOperationArgs arguments;
    Bounds bounds;
    color_t color;
};


#endif
