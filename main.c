#include "Geom.h"
#include "Precalc.h"
#include "Display.h"
#include "Map.h"
#include "Hero.h"

#include <SDL2/SDL.h>

int
main(const int argc, const char* const argv[])
{
    if(argc != 2) return 1;
    // Precalc
    Precalc_Optimize();
    atexit(Precalc_Shutdown);
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
        const int t0 = SDL_GetTicks();
        // Hero
        hero = Hero_Move(hero);
        // Display
        Display_RenderFrame(hero);
        // FPS
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int fps = 60;
        const int delay = 1000 / fps - dt;
        SDL_Delay(delay < 0 ? 0 : delay);
    }
}
