#include "Play.h"

#include "Hero.h"
#include "Sdl.h"
#include "Map.h"
#include "Sprites.h"

static int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    const int type = event.type;
    const int sym = event.key.keysym.sym;
    return type == SDL_QUIT || sym == SDLK_F1;
}

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    Map map = open("start");
    Sprites sprites = wake("start");
    Hero hero = spawn();
    Sdl sdl = setup(res, fps);
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(int renders = 0; res == 128 ? renders < fps : !done(); renders++)
    {
        SDL_PumpEvents();
        rest(sprites);
        hero = console(hero, key);
        if(hero.consoling)
            hero = type(hero, key);
        else
        {
            hero = sustain(hero, map, key);
            grab(hero, sprites, key);
            edit(hero, map, key);
            sprites = place(hero, sprites, key);
        }
        const Sprites relatives = arrange(sprites, hero);
        render(sdl, hero, relatives, map);
        kill(relatives);
        sdl = tick(sdl, renders);
    }
    release(sdl);
    close(map);
    kill(sprites);
}
