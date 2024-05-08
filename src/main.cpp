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
    display.init(9600, true, 2, false);
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

    Serial.begin(9600);
    Serial.println();
    Serial.println();
    Serial.println("----RESTART----");
    Serial.println();
    delay(200);

    init_display();

    engine = create_graphics_engine();
    
    
#if IS_VIRTUAL_DISPLAY_USED
    delay(4000); //Time to connect VScreen to ESP
#endif
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);

    static char* text = new char[40];
    static cord_t y = 1;

    strcpy(text, "HO_HO_HO_HO_HO");

    Label label1 = new Label_(text);
    label1->foreground_color = color_t::White;
    label1->background_color = transparent_color();

    Rectangle recti1 = new Rectangle_(5);
    recti1->foreground_color = color_t::Black;
    recti1->background_color = transparent_color();

    DockPanel panel1 = new DockPanel_(std::vector<DockElement> { fit_into_dock(label1, Vector(0, 20 + y)), fit_into_dock(recti1, Vector(50, 50)) });
    panel1->foreground_color = panel1->background_color = transparent_color();
    panel1->padding = PaddingSize(13);

    root = panel1;

    GFX root_gfx(engine, Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
    root->render(root_gfx);

    DrawSettings draw_settings;
    draw_settings.background_color = color_t::Red;

    engine->push(draw_settings);

    delete label1;
    delete recti1;
    delete panel1;



    y += 8;

    delay(3000);
}
