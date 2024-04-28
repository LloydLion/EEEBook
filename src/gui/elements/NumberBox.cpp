#include "gui/elements/NumberBox.h"
#include <Arduino.h>

NumberBox_::NumberBox_(long number): Label_(_text)
{
    sprintf(_text, "%d", number);
}

NumberBox_::NumberBox_(float number) : Label_(_text)
{
    sprintf(_text, "%d", number);
}

NumberBox_::NumberBox_(double number) : Label_(_text)
{
    sprintf(_text, "%d", number);
}
