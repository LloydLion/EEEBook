#include "config.h"
#include "gui/elements/Grid.h"
#include "gui/elements/Rectangle.h"
#include "gui/GraphicsEngine.h"


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

DISPLAY_TYPE display(DISPLAY_DRIVER(DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_BUSY_PIN));

void init_display()
{
    display.init(9600, true, 2, false);
    display.setRotation(1);
    display.clearScreen();
}

GraphicsEngine create_graphics_engine()
{
    return new GxEPD_GraphicsEngine(&display);
}

#endif

Grid grid;
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

    init_display();

    engine = create_graphics_engine();
    
    Serial.begin(9600);
    
    Rectangle rect1 = new Rectangle_(5, 5);
    Rectangle rect2 = new Rectangle_(5, 5);
    Rectangle rect3 = new Rectangle_(5, 5);
    Rectangle rect4 = new Rectangle_(5, 5);

    grid = new Grid_(
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 2), define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Fixed, 50) },
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 2) },
        std::vector<GridElement> { fit_in_grid(rect1, 0, 0), fit_in_grid(rect2, 0, 1), fit_in_grid(rect3, 1, 0), fit_in_grid(rect4, 1, 1) }
    );
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);

    GFX root_gfx(engine, create_size(300, 400));

    grid->render(root_gfx);

    delay(10000);
}
