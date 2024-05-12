#include "config.h"
#include "gui/elements/Grid.h"
#include "gui/elements/Rectangle.h"
#include "gui/elements/Label.h"
#include "gui/GraphicsEngine.h"
#include "gui/elements/DockPanel.h"

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

Grid root;
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
    
    Label label1 = new Label_("Hello");
    label1->name = "Label 1(0:0)";
    label1->foreground_color(color_t::White);
    label1->background_color(color_t::Black);
    label1->set_font(engine->get_default_font());
    
    Label label2 = new Label_("World");
    label2->name = "Label 2(2:0)";
    label2->foreground_color(color_t::White);
    label2->background_color(color_t::Black);
    label2->set_font(engine->get_default_font());

    Rectangle recti1 = new Rectangle_(5);
    recti1->name = "Rect 1(0:1)";
    recti1->foreground_color(color_t::Black);
    recti1->background_color(transparent_color());

    Rectangle recti2 = new Rectangle_(5);
    recti2->name = "Rect 2(1:0)";
    recti2->foreground_color(color_t::Black);
    recti2->background_color(transparent_color());
    
    Rectangle recti3 = new Rectangle_(5);
    recti3->name = "Rect 3(1:1)";
    recti3->foreground_color(color_t::Black);
    recti3->background_color(transparent_color());
    
    Rectangle recti4 = new Rectangle_(5);
    recti4->name = "Rect 4(2:1)";
    recti4->foreground_color(color_t::Black);
    recti4->background_color(transparent_color());

    Grid grid = new Grid_(
        { define_grid_rc(Auto), define_grid_rc(Proportional, 1), define_grid_rc(Proportional, 1) },
        { define_grid_rc(Auto), define_grid_rc(Proportional, 1) },
        { fit_into_grid(label1, 0, 0), fit_into_grid(recti1, 0, 1), fit_into_grid(recti2, 1, 0), fit_into_grid(recti3, 1, 1), fit_into_grid(label2, 2, 0), fit_into_grid(recti4, 2, 1) }
    );
    grid->name = "Grid";
    grid->padding(PaddingSize(1));
    grid->foreground_color(transparent_color());
    grid->background_color(transparent_color());

    root = grid;

    
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


        root->margin(MarginSize(time));


        Serial.println("----RENDER----");

        GFX root_gfx(engine, Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
        root->render(root_gfx);

        Serial.println("----DRAWING----");

        DrawSettings draw_settings;
        draw_settings.background_color = color_t::Red;

        engine->push(draw_settings);

        time += 1;

        Serial.println("----DONE----");

        delay(10000);
    }
    catch (const std::runtime_error &err)
    {
        Serial.println();
        Serial.println();
        Serial.println("----CRITICAL ERROR----");
        Serial.println("Manual restart required");
        Serial.println();

        while (1) delay(100);
    }
}
