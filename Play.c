#include "Play.h"

#include "Hero.h"
#include "Sdl.h"
#include "Map.h"
#include "Sprites.h"

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    const char* const start = "start";
    Map map = open(start);
    Sprites sprites = wake(start);
    Hero hero = spawn(start);
    Sdl sdl = setup(res, fps);
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(int renders = 0; res == 128 ? renders < fps : !key[SDL_SCANCODE_F1]; renders++)
    {
        const int t0 = SDL_GetTicks();
        SDL_PumpEvents();
        rest(sprites);
        #ifdef CONSOLE
        hero = console(hero, key);
        if(hero.consoling)
            hero = type(hero, key);
        else
        #endif
        {
            hero = sustain(hero, sprites, map, key);
            #ifdef CONSOLE
            edit(hero, map, key);
            sprites = place(hero, sprites, key);
            hero = save(hero, map, sprites, key);
            #endif
        }
        render(sdl, hero, sprites, map);
        sdl = tick(sdl, renders);
        const int t1 = SDL_GetTicks();
        const int ms = 1000.0 / sdl.fps - (t1 - t0);
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    release(sdl);
    close(map);
    kill(sprites);
}
