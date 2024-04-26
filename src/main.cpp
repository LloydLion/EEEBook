#include "config.h"
#include "gui/elements/Grid.h"
#include "gui/elements/Rectangle.h"
#include "gui/elements/Label.h"
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

    init_display();

    engine = create_graphics_engine();
    
    Serial.begin(9600);
    
    char* text = new char[13];
    strcpy(text, "Hello World!");

    Label label1 = new Label_(text);
    Rectangle rect1 = new Rectangle_(5, 5);
    Rectangle rect2 = new Rectangle_(5, 5);
    
    Rectangle recti1 = new Rectangle_(5, 5);
    Rectangle recti2 = new Rectangle_(5, 5);
    Rectangle recti3 = new Rectangle_(5, 5);
    Rectangle recti4 = new Rectangle_(5, 5);

    Grid inner_grid = new Grid_(
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1)},
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 1) },
        std::vector<GridElement> { fit_in_grid(recti1, 0, 0), fit_in_grid(recti2, 0, 1), fit_in_grid(recti3, 1, 0), fit_in_grid(recti4, 1, 1) }
    );

    root = new Grid_(
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 2), define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Fixed, 50) },
        std::vector<GridRCDefinition> { define_grid_rc(GridRCSizeType::Proportional, 1), define_grid_rc(GridRCSizeType::Proportional, 2) },
        std::vector<GridElement> { fit_in_grid(label1, 0, 0), fit_in_grid(inner_grid, 0, 1), fit_in_grid(rect1, 1, 0), fit_in_grid(rect2, 1, 1) }
    );

#if IS_VIRTUAL_DISPLAY_USED
    delay(4000); //Time to connect VScreen to ESP
#endif
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);

    GFX root_gfx(engine, create_size(300, 400));

    root->render(root_gfx);

    delay(10000);
}
