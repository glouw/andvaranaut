#include "Display.h"
#include "SDL2/SDL.h"

int
main(const int argc, const char* const argv[])
{
    // <bin map>
    if(argc != 2) return 1;
    // Starts game
    Display_Boot();
    Map map = Map_Load(argv[1]);
    Hero hero = {
        .where = map.where,
        .theta = map.theta,
        .d0 = 0.10,
        .dy = 0.10,
        .dx = 0.10,
        .health = 100.0,
        .draw = 3.5
    };
    //while(hero.health > 0)
    for(int i = 0; i < 60; i++)
    {
        // Starts timer
        const int t0 = SDL_GetTicks();
        // Updates game data and renders one frame
        hero = Hero_Move(hero, map);
        Display_RenderFrame(hero, map);
        // Ends timer
        const int t1 = SDL_GetTicks();
        // Corrects FPS
        const int dt = t1 - t0;
        const int desiredFPS = 60;
        const int delay = (1000 / desiredFPS) - dt;
        const int cdelay = delay < 0 ? 0 : delay;
        SDL_Delay(cdelay);
        // Display corrected FPS
        const int correctedFPS = 1000 / (dt + cdelay);
        Display_ShowFPS(correctedFPS, 0, 0);
        // Update render to display
        Display_Update();
    }
    Display_Shutdown();
    Map_Unload(map);
    return 0;
}
