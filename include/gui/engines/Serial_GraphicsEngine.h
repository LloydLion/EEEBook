#ifndef GUI_ENGINES_SERIAL_GRAPHICSENGINE_H
#define GUI_ENGINES_SERIAL_GRAPHICSENGINE_H
#include "config.h"
#include "../GraphicsEngine.h"
#include "platform/platform.h"

#if IS_VIRTUAL_DISPLAY_USED && (PLATFORM & PLATFORM_MCU_ESP32)

class Serial_GraphicsEngine : public GraphicsEngine_
{
private:
    bool _clearing_required = false;
public:
    void draw_rectangle(Bounds bounds) override;
    void print_text(Point start_point, cord_t width_limit, const char *text) override;
    void push(UpdateRule rule);
};

#endif
#endif
