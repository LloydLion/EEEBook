#include "gui/render/builtin/FloatNumberBox.h"
#include <cstdio>


FloatNumberBox_::FloatNumberBox_(Font font): TextElement_(font)
{
    finish_initialization();
}

void create_buffer(char* buffer, uint8_t precision, float value)
{
    sprintf(buffer, "%f", value); //TODO: use precision
}

void FloatNumberBox_::precision(uint8_t value)
{
    _p_precision = value;
    create_buffer(_buffer, _p_precision, _p_number);
    trigger_mutation(Composition);
}

void FloatNumberBox_::number(float value)
{
    _p_number = value;
    create_buffer(_buffer, _p_precision, _p_number);
    trigger_mutation(Composition);
}
