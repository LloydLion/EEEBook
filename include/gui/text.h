#ifndef GUI_TEXT_H
#define GUI_TEXT_H
#include "GFX.h"
#include <Arduino.h>

size_t cut_text(const GFX &gfx, cord_t width_limit, char* text);

#endif
