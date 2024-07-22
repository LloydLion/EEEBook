#include "config.h"
#include "gui/drawing/screens/GxEPD_EInk_Screen.h"
#include "ui.h"
#include "platform/platform.h"
#include "platform/time.h"
#include <stdexcept>

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

        Serial.begin(2000000);

        Screen screen = new GxEPD_EInk_Screen_();
        Size size = screen->full_viewport_size();

        screen->initialize();

        screen->begin();
        screen->clear();


/*
        for (cord_t x = 0; x < size.width(); x += 3)
            for (cord_t y = 0; y < size.height(); y++)
                if (!(y > 5 and y < 15))
                    screen->draw_pixel(Vector(x, y), ColorMap::Black);
*/


        screen->draw_vertical_line(Vector(10, 15), 10, ColorMap::Red);        

        screen->send();

        delete screen;
    }
    catch(const std::exception &err)
    {
        std_println("RUNTIME ERROR");
        std_println(err.what());
        ON_ERROR_BEHAVIOR;
    }

    ON_END_BEHAVIOR;
}
