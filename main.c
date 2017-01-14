#include "Display.h"
#include "Map.h"
#include "Hero.h"

int
main(const int argc, const char* const argv[])
{
    // <bin map>
    if(argc != 2) return 1;
    // Game start
    Display_Boot();
    Map map = Map_Load(argv[1]);
    Hero hero = {
        .where = { 2.5, 3.5 },
        .theta = 0.0,
        .d0 = 0.10,
        .dy = 0.10,
        .dx = 0.10,
        .health = 100.0
    };
    while(hero.health > 0)
    {
        hero = Hero_Move(hero, map);
        Display_RenderFrame(hero, map);
    }
    Display_Shutdown();
    Map_Unload(map);
    return 0;
}
