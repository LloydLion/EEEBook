#include "gui/render/builtin/Label.h"

Label_::Label_(const char *text, Font font): _p_text(text), TextElement_(font)
{
    finish_initialization();
}

void Label_::text(const char* value)
{
    _p_text = value;
    trigger_mutation(Composition);
}