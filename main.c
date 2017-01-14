#include "Display.h"
#include "SDL2/SDL.h"

int
main(const int argc, const char* const argv[])
{
    // <bin map>
    if(argc != 2) return 1;
    // Game start
    Display_Boot();
    Map map = Map_Load(argv[1]);
    Hero hero = {
        .where = map.where,
        .theta = map.theta,
        .d0 = 0.10,
        .dy = 0.10,
        .dx = 0.10,
        .health = 100.0
    };
    while(hero.health > 0)
    {
        const int t0 = SDL_GetTicks();
        // World Data
        hero = Hero_Move(hero, map);
        // Display
        Display_RenderFrame(hero, map);
        // FPS
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        // FPS Correction
        const int desiredFPS = 60;
        const int delay = 1000 / desiredFPS - dt;
        SDL_Delay(delay < 0 ? 0 : delay);
    }
    Display_Shutdown();
    Map_Unload(map);
    return 0;
}
