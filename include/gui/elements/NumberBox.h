#ifndef GUI_ELEMENTS_NUMBER_BOX_H
#define GUI_ELEMENTS_NUMBER_BOX_H

#include "Label.h"

class NumberBox_;
typedef NumberBox_ *NumberBox;

class NumberBox_ : public Label_
{
public:
    //Integer numbers
    NumberBox_(long number);

    //Floating point numbers
    NumberBox_(float number);
    NumberBox_(double number);
};

#endif