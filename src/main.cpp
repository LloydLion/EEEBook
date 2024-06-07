#include "config.h"
#include "ui.h"

#if IS_VIRTUAL_DISPLAY_USED

#include "gui/engines/Serial_GraphicsEngine.h"

void init_display()
{

}

GraphicsEngine create_graphics_engine()
{
    return new Serial_GraphicsEngine();
}

#else

#include "gui/engines/GxEPD_GraphicsEngine.h"
//#include "gui/fonts/FreeMonoBold12pt7b.h"
#include <Fonts/FreeMonoBold12pt7b.h>

DISPLAY_TYPE display(DISPLAY_DRIVER(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_BUSY_PIN));

void init_display()
{
    display.init(115200, true, 2, false);
#ifdef DISPLAY_ROTATION
    display.setRotation(DISPLAY_ROTATION);
#endif
    display.fillScreen(GxEPD_WHITE);
}

GraphicsEngine create_graphics_engine()
{
    auto engine = new GxEPD_GraphicsEngine(&display);
    engine->register_font(&FreeMonoBold12pt7b);
    return engine;
}

#endif

UIElement root;
GraphicsEngine engine;

void setup()
{
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);

    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.println("----RESTART----");
    Serial.println();

    delay(200);

    init_display();

    engine = create_graphics_engine();
    root = setup_ui();

#if IS_VIRTUAL_DISPLAY_USED
    delay(4000); //Time to connect VScreen to ESP
#endif
}

void loop()
{
    try
    {
        digitalWrite(2, HIGH);
        delay(100);
        digitalWrite(2, LOW);

        static uint8_t time = 0;

        Serial.println("----UPDATE----");

        update_ui(root, time);

        Serial.println("----RENDER----");

        GFX root_gfx(engine, Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
        root->render(root_gfx);

        Serial.println("----DRAWING----");

        DrawSettings draw_settings = create_draw_settings(time);
        engine->push(draw_settings);

        Serial.println("----DONE----");

        time += 1;
        delay(10000);
    }
    catch (const std::runtime_error &err)
    {
        Serial.println();
        Serial.println();
        Serial.println("----CRITICAL ERROR----");
        Serial.println("Manual restart required");
        Serial.println(err.what());

        while (1) delay(100);
    }
}
