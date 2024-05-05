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
    DockPanel panel1 = new DockPanel_(std::vector<DockElement> { fit_into_dock(label1, Vector(0, 20+y)) });
    panel1->padding = PaddingSize(13);

    //Rectangle recti1 = new Rectangle_(5, 5);
    //Rectangle recti2 = new Rectangle_(5, 5);
    //Rectangle recti3 = new Rectangle_(5, 5);
    //Rectangle recti4 = new Rectangle_(5, 5);

    //Grid inner_grid = new Grid_(
    //    std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1)},
    //    std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1) },
    //    std::vector<GridElement> { fit_in_grid(recti1, 0, 0), fit_in_grid(recti2, 0, 1), fit_in_grid(recti3, 1, 0), fit_in_grid(recti4, 1, 1) }
    //);

    root = panel1;

    GFX root_gfx(engine, Size(DISPLAY_WIDTH, DISPLAY_HEIGHT));
    root->render(root_gfx);
    engine->push();

    delete label1;
    //delete rect1;
    delete panel1;



    y += 8;

    delay(3000);
}
