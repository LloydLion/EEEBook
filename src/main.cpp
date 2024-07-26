#include "config.h"
#include "gui/drawing/screens/GxEPD_EInk_Screen.h"
// #include "ui.h"
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

uint8_t fill_pattern(s_cord_t a, s_cord_t b, cord_t a_size, cord_t b_size, UniversalParameters<GUI_PATTERN_PARAMETERS_SIZE> parameters)
{
    return 0;
}

int main()
{
    try
    {

        say_hello();
        /*

        init_display();

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
            engine->push(draw_settings);

            std_println("----DONE----");
            delay_ms(1000);
        }
        */

        DrawingContext::initialize();

        DrawingContext::instance().pattern_catalog->register_pattern(PatternFunction(&fill_pattern, "fill"));
        DrawingContext::instance().font_engine->register_font(&FreeMono12pt7b);

        BMP_File_Screen_ *screen = new BMP_File_Screen_(Size(300, 300));
        Size size = screen->full_viewport_size();

        screen->initialize();
        screen->begin();
        screen->clear();

        UniversalDrawer drawer = new UniversalDrawer_(screen);

        DrawOperation operation = DrawOperation::create_new();
        operation.area_type = DrawAreaType::Rect;
        operation.arguments.rect.thickness = 3;
        operation.bounds = Bounds(Vector(10, 10), Size(100, 100));
        operation.patterns[0].palette[0] = ColorMap::Black;
        drawer->draw(operation);

        operation = DrawOperation::create_new();
        operation.area_type = DrawAreaType::Text;
        operation.arguments.text.limit = -1;
        operation.arguments.text.text = "Hello world";
        operation.arguments.text.font = 0;
        operation.bounds = Bounds(Vector(10, 120), Size(1000, 1000));
        operation.patterns[0].palette[0] = ColorMap::Black;
        drawer->draw(operation);

        screen->send();
    }
    catch (const std::exception &err)
    {
        std_println("RUNTIME ERROR");
        std_println(err.what());
        ON_ERROR_BEHAVIOR;
    }

    ON_END_BEHAVIOR;
}
