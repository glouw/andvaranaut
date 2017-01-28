// Copyright (C) 2016-2017 Gustav Louw
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Display.h"
#include "SDL2/SDL.h"

int
main(const int argc, const char* const argv[])
{
    // <bin map>
    if(argc != 2) return 1;
    // Starts the game
    Map map = Map_Load(argv[1]);
    Hero hero = {
        .where = map.where,
        .theta = map.theta,
        .d0 = 0.1f,
        .dy = 0.1f,
        .dx = 0.1f,
        .health = 100.f,
        .torch = 6.f
    };
    Display_Boot();
    #ifdef PROFILE
    const int frames = 60;
    for(int frame = 0; frame < frames; frame++)
    #else
    while(hero.health > 0.f)
    #endif
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
        Display_FPS(correctedFPS, 0, 0);
        // Update render to display
        Display_Update();
    }
    Display_Shutdown();
    Map_Unload(map);
    return 0;
}
