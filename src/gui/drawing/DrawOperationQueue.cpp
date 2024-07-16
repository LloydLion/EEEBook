#include "gui/drawing/DrawOperationQueue.h"

void DrawOperationQueue_::enqueue(const DrawOperation &operation)
{
    _operations.push_back(operation);
}

void DrawOperationQueue_::clear()
{
    _operations.clear();
}

size_t DrawOperationQueue_::size()
{
    return _operations.size();
}

const DrawOperation &DrawOperationQueue_::get(size_t index)
{
    return _operations[index];
}
