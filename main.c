#include "Geom.h"
#include "Display.h"
#include "Map.h"
#include "Hero.h"

#include <stdlib.h>

int
main(const int argc, const char* const argv[])
{
    // <bin map>
    if(argc != 2) return 1;
    // Display
    Display_Boot();
    atexit(Display_Shutdown);
    // Map
    Map_Load(argv[1]);
    atexit(Map_Shutdown);
    // Hero
    Hero hero = {
        .where = { 2.5, 3.5 },
        .theta = 0.0,
        .d0 = 0.10,
        .dy = 0.10,
        .dx = 0.10
    };
    // Loop
    for(;;)
    {
        hero = Hero_Move(hero);
        Display_RenderFrame(hero);
    }
}
