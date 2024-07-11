#include "config.h"
#include "ui.h"
#include "platform/platform.h"
#include "platform/time.h"
#include <stdexcept>

#pragma region Engine specific
#if ENGINE == ENGINE_GXEPD
#include "gui/engines/GxEPD_GraphicsEngine.h"
#include <Fonts/Org_01.h>

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
    engine->register_font(&Org_01);
    return engine;
}

#elif ENGINE == ENGINE_STDOUT
#include "gui/engines/StdOut_GraphicsEngine.h"

void init_display()
{

}

GraphicsEngine create_graphics_engine()
{
    return new StdOut_GraphicsEngine();
}

#else
#error "Unknown graphics engine"
#endif
#pragma endregion

#pragma region Platform specific
#if PLATFORM & PLATFORM_MCU
#include <Arduino.h>

void say_hello()
{
    delay(4000);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
}

#define ON_ERROR_BEHAVIOR while(true) { digitalWrite(LED_BUILTIN, HIGH); delay(100); digitalWrite(LED_BUILTIN, LOW); delay(100); }
#define ON_END_BEHAVIOR while(true) { digitalWrite(LED_BUILTIN, HIGH); delay(100); digitalWrite(LED_BUILTIN, LOW); delay(1000);}

int main();
void setup() { main(); }
void loop() {  }

#elif PLATFORM & PLATFORM_PC

void say_hello()
{
    std_println("Program start OK");
}

#define ON_ERROR_BEHAVIOR return -1;
#define ON_END_BEHAVIOR return 0;

#else
#error "Unknown platform"
#endif
#pragma endregion

int main()
{
    try
    {
        say_hello();

        init_display();
        GraphicsEngine engine = create_graphics_engine();

        DrawSettings draw_settings = create_draw_settings(0);
        UIElement root = setup_ui(engine);

        uint8_t time = 0;
        while (true)
        {
        #ifdef TIME_LIMIT
            if (time >= TIME_LIMIT) break;
        #endif

            time++;

            std_println("----UPDATE----");
            update_ui(root, time);

            std_println("----RENDER----");
            GFX root_gfx(engine, Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
            root->render(root_gfx);

            std_println("----DRAWING----");
            DrawSettings draw_settings = create_draw_settings(time);
            engine->push(draw_settings);

            std_println("----DONE----");
            delay_ms(1000);
        }
    }
    catch(const std::exception &err)
    {
        std_println("RUNTIME ERROR");
        std_println(err.what());
        ON_ERROR_BEHAVIOR;
    }

    ON_END_BEHAVIOR;
}
