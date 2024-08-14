#include "config.h"
#include "gui/drawing/screens/GxEPD_EInk_Screen.h"
#include "ui.h"
#include "platform/stdout.h"
#include "platform/platform.h"
#include "platform/time.h"
#include <stdexcept>
#include "gui/drawing/UniversalDrawer.h"
#include "gui/drawing/screens/BMP_File_Screen.h"
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

uint8_t fill_pattern(s_cord_t a, s_cord_t b, cord_t a_size, cord_t b_size, UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters) { return 0; }

//void draw_subline(Vector start, SignedVector direction, cord_t offset, cord_t length, Screen output, bool c = true);

void draw_subline(Vector start, SignedVector direction, cord_t length, Screen output, int cornered_mode);
void draw_thick_line(Vector start, Vector end, cord_t thinkness, Screen output);

int main()
{
    try
    {
        say_hello();

        DrawingContext::initialize();

        DrawingContext::instance().pattern_catalog->register_pattern(PatternFunction(&fill_pattern, "fill"));
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
            root->render(root_gfx);

            std_println("----DRAWING----");
            screen->begin();
            screen->clear();
            //drawer->draw(queue);
            //draw_line(Vector(130,80), Vector(1,1), Pattern(), screen, 0, 0);

            //execute_along_ray(Vector(20, 30), Vector(21, 32), screen->full_viewport_size(), -1, function, screen);
            draw_thick_line(Vector(20, 100), Vector(20 + 20, 100 + 10), 8, screen);
            draw_thick_line(Vector(50, 100), Vector(50 + 10, 100 + 20), 8, screen);
            draw_thick_line(Vector(80, 100), Vector(80 - 10, 100 + 20), 8, screen);
            draw_thick_line(Vector(110, 100), Vector(110 - 20, 100 + 10), 8, screen);
            draw_thick_line(Vector(140, 100), Vector(140 - 20, 100 - 10), 8, screen);
            draw_thick_line(Vector(170, 100), Vector(170 - 10, 100 - 20), 8, screen);
            draw_thick_line(Vector(200, 100), Vector(200 + 10, 100 - 20), 8, screen);
            draw_thick_line(Vector(230, 100), Vector(230 + 20, 100 - 10), 8, screen);

            draw_subline(Vector(20, 50), SignedVector(1, 2), 20, screen, 0);

            draw_subline(Vector(23, 50), SignedVector(1, 2), 20, screen, 1);

            draw_subline(Vector(26, 50), SignedVector(1, 2), 20, screen, -1);



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
