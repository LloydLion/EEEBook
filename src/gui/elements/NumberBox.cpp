#include "gui/elements/NumberBox.h"
#include <cstdio>

NumberBox_::NumberBox_(int64_t number): Label_(_text)
{
    std::sprintf(_text, "%ld", number);
}

NumberBox_::NumberBox_(float number) : Label_(_text)
{
    std::sprintf(_text, "%f", number);
}

NumberBox_::NumberBox_(double number) : Label_(_text)
{
    std::sprintf(_text, "%lf", number);
}

void NumberBox_::set_value(int64_t number) 
{
    std::sprintf(_text, "%d", number);
}

void NumberBox_::set_value(float number) 
{
    std::sprintf(_text, "%f", number);
}
void NumberBox_::set_value(double number) 
{
    std::sprintf(_text, "%lf", number);
}
