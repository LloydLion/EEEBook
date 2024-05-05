#include "gui/elements/NumberBox.h"
#include <Arduino.h>

NumberBox_::NumberBox_(long number): Label_(_text)
{
    sprintf(_text, "%ld", number);
}

NumberBox_::NumberBox_(float number) : Label_(_text)
{
    sprintf(_text, "%f", number);
}

NumberBox_::NumberBox_(double number) : Label_(_text)
{
    sprintf(_text, "%lf", number);
}

void NumberBox_::set_value(long number) 
{
    sprintf(_text, "%d", number);
}

void NumberBox_::set_value(float number) 
{
    sprintf(_text, "%f", number);
}
void NumberBox_::set_value(double number) 
{
    sprintf(_text, "%lf", number);
}
