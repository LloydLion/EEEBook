#ifndef GUI_ELEMENTS_LABEL_H
#define GUI_ELEMENTS_LABEL_H

#include "../coordinates.h"
#include "../TextElement.h"

class Label_;
typedef Label_ *Label;

class Label_ : public TextElement_
{
public:
    Label_(const char *text);

    void set_text(const char *text);

    void i_render(const GFX& gfx) override;
    Size i_min_size() override;
    Size i_max_size() override;
    
private:
    const char* _text;
};

#endif