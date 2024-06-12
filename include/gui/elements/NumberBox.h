#ifndef GUI_ELEMENTS_NUMBER_BOX_H
#define GUI_ELEMENTS_NUMBER_BOX_H

#include "Label.h"
#include <stdint.h>

class NumberBox_;
typedef NumberBox_ *NumberBox;

class NumberBox_ : public Label_
{
private:
    char _text[20];

public:
    //Integer numbers
    NumberBox_(int64_t number);

    //Floating point numbers
    NumberBox_(float number);
    NumberBox_(double number);

    void set_value(int64_t number);
    void set_value(float number);
    void set_value(double number);

};

#endif