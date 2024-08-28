#ifndef GUI_RENDER_BUILTIN_FLOAT_NUMBER_BOX_H
#define GUI_RENDER_BUILTIN_FLOAT_NUMBER_BOX_H

#include "gui/render/builtin/TextElement.h"

class FloatNumberBox_;
typedef FloatNumberBox_ *FloatNumberBox;

class FloatNumberBox_ : public TextElement_
{
private:
    char _buffer[20] = "0.00";
    uint8_t _p_precision = 2;
    float _p_number = 0.0f;

protected:
    inline const char *get_text() override { return _buffer; }

public:
    FloatNumberBox_(Font font);

    PROPERTY(uint8_t, precision) AUTO_GET(_p_precision);
    PROPERTY(float, number) AUTO_GET(_p_number);
};

#endif
