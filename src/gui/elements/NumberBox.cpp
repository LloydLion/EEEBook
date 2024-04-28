#include "gui/elements/NumberBox.h"
#include <Arduino.h>

char* to_string(long number)
{
    char string[25];
    sprintf(string,"%d",number);
    return string;
}
char* to_string(float number)
{
    char string[25];
    sprintf(string,"%d",number);
    return string;
}
char* to_string(double number)
{
    char string[25];
    sprintf(string,"%d",number);
    return string;
}

class NumberBox_ : public Label_ {
public:
    NumberBox_(long number): Label_(to_string(number)) {}

    NumberBox_(float number) : Label_(to_string(number)) {}

    NumberBox_(double number) : Label_(to_string(number)) {}
};
