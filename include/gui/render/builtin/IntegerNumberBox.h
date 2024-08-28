#ifndef GUI_RENDER_BUILTIN_INTEGER_NUMBER_BOX_H
#define GUI_RENDER_BUILTIN_INTEGER_NUMBER_BOX_H

#include "gui/render/builtin/TextElement.h"

class IntegerNumberBox_;
typedef IntegerNumberBox_ *IntegerNumberBox;

class IntegerNumberBox_ : public TextElement_
{
private:
    char _buffer[20] = "0";
    int64_t _p_number = 0;

protected:
    inline const char *get_text() override { return _buffer; }

public:
    IntegerNumberBox_(Font font);

    PROPERTY(int64_t, number) AUTO_GET(_p_number);
};

#endif
