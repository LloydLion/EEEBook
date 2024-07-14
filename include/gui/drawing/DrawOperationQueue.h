#ifndef GUI_DRAWING_DRAW_OPERATION_QUEUE_H
#define GUI_DRAWING_DRAW_OPERATION_QUEUE_H

#include "gui/drawing/DrawOperation.h"
#include <vector>

class DrawOperationQueue_;
typedef DrawOperationQueue_ *DrawOperationQueue;

class DrawOperationQueue_
{
private:
    std::vector<DrawOperation> _operations;

public:
    void enqueue(const DrawOperation &operation);
    void clear();

    size_t size();
    const DrawOperation &get(size_t index);
};

#endif
