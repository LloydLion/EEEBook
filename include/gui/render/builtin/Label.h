#ifndef GUI_RENDER_BUILTIN_LABEL_H
#define GUI_RENDER_BUILTIN_LABEL_H

#include "gui/coordinates.h"
#include "gui/render/builtin/TextElement.h"

class Label_;
typedef Label_ *Label;

class Label_ : public TextElement_
{
private:
    const char *_p_text;

protected:
    const char *get_text() override { return _p_text; }

public:
    Label_(const char *text, Font font);

    PROPERTY(const char*, text) AUTO_GET(_p_text);
};

#endif