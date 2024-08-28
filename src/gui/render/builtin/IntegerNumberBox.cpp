#include "gui/render/builtin/IntegerNumberBox.h"
#include <cstdio>


IntegerNumberBox_::IntegerNumberBox_(Font font): TextElement_(font)
{
    finish_initialization();
}

void create_buffer(char* buffer, int64_t value)
{
    sprintf(buffer, "%ld", value);
}

void IntegerNumberBox_::number(int64_t value)
{
    _p_number = value;
    create_buffer(_buffer, _p_number);
    trigger_mutation(Composition);
}
