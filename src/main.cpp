#include "config.h"
#include "gui/drawing/screens/GxEPD_EInk_Screen.h"
#include "ui.h"
#include "platform/stdout.h"
#include "platform/platform.h"
#include "platform/time.h"
#include <stdexcept>
#include "gui/drawing/UniversalDrawer.h"
#include "gui/drawing/screens/BMP_File_Screen.h"
#include "gui/drawing/builtinPatternFunctions.h"
#include "gui/drawing/fonts/buildin/FreeMono12pt7b.h"

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

#define ON_ERROR_BEHAVIOR                \
    while (true)                         \
    {                                    \
        digitalWrite(LED_BUILTIN, HIGH); \
        delay(100);                      \
        digitalWrite(LED_BUILTIN, LOW);  \
        delay(100);                      \
    }
#define ON_END_BEHAVIOR                  \
    while (true)                         \
    {                                    \
        digitalWrite(LED_BUILTIN, HIGH); \
        delay(100);                      \
        digitalWrite(LED_BUILTIN, LOW);  \
        delay(1000);                     \
    }

int main();
void setup() { main(); }
void loop() {}

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

#pragma region Screen specific
#if SCREEN_TYPE == SCREEN_BMP
Screen create_screen()
{
    return new BMP_File_Screen_(Size(BMP_SCREEN_WIDTH, BMP_SCREEN_HEIGHT));
}
#elif SCREEN_TYPE == SCREEN_GXEPD
Screen create_screen()
{
    return new GxEPD_EInk_Screen();
}
#else
    #error "Unknown screen"
#endif
#pragma endregion

int main()
{
    try
    {
        say_hello();

        DrawingContext::initialize();

        builtin_pattern_functions::register_all();

        DrawingContext::instance().font_engine->register_font(&FreeMono12pt7b);

        Screen screen = create_screen();
        screen->initialize();
        DrawOperationQueue queue = new DrawOperationQueue_();
        UniversalDrawer drawer = new UniversalDrawer_(screen);

        UIElement root = setup_ui();

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
            GFX root_gfx(queue, screen->full_viewport_size());
            //root->render(root_gfx);

            Pattern p(builtin_pattern_functions::Chess);
            p.palette[0] = ColorMap::Black;
            p.palette[1] = ColorMap::Green;
            p.palette[2] = ColorMap::Blue;
            p.parameters.set(builtin_pattern_functions::ChessFlags::UseAB);
            p.parameters.data[1] = 1;
            p.parameters.data[2] = 1;
            p.parameters.data[3] = 0;
            p.parameters.data[4] = 0;
            p.interpretation_options = Pattern::UsePathBasedCoordinateSystem;
            p.decoration_options.flags = (Pattern::DecorationFlags)(Pattern::TransposeCoordinates);
            p.decoration_options.a_tiling_size = 5;
            p.decoration_options.b_tiling_size = 3;

            root_gfx.draw_line(LocalVector(50, 50), SignedVector(1, -1), 30, p, 20);
            root_gfx.draw_line(LocalVector(100, 50), SignedVector(1, -1), 40, p, 20);
            root_gfx.draw_line(LocalVector(150, 50), SignedVector(1, -1), 60, p, 20);
            root_gfx.draw_line(LocalVector(200, 50), SignedVector(1, -1), 100, p, 20);
            root_gfx.draw_line(LocalVector(250, 50), SignedVector(1, -1), 140, p, 20);
            root_gfx.print_text(Vector(0,0), "Test", ColorMap::Green, -1, Font());
            std_println("----DRAWING----");
            screen->begin();
            screen->clear();

            drawer->draw(queue);

            screen->send();

            std_println("----DONE----");

            delay_ms(1000);
        }
    }
    catch (const std::exception &err)
    {
        std_println("RUNTIME ERROR");
        std_println(err.what());
        ON_ERROR_BEHAVIOR;
    }

    ON_END_BEHAVIOR;
}
