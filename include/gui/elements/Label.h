#ifndef GUI_ELEMENTS_LABEL_H
#define GUI_ELEMENTS_LABEL_H

#include "../cordinates.h"
#include "../TextElement.h"

class Label_;
typedef Label_ *Label;

class Label_ : public TextElement_
{
public:
    Label_(const char *text);
    ~Label_();

    void render(const GFX &gfx) override;
    void set_text(const char *text);
    
    Size min_size() override;
private:
    const char* _original_text;
    char* _text_buffer;
};

#endif