#include "Play.h"

#include "Hero.h"
#include "Portals.h"
#include "Sdl.h"
#include "Map.h"
#include "Sprites.h"
#include "Util.h"

static int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    const int type = event.type, sym = event.key.keysym.sym;
    return type == SDL_QUIT || sym == SDLK_F1 || sym == SDLK_ESCAPE;
}

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    Map map = open("start");
    Sprites sprites = wake("start");
    Hero hero = spawn("hero.cfg");
    Portals portals = populate("portals.cfg");
    Sdl sdl = setup(res, fps, "surfaces.cfg");
    #ifdef PROFILE
    for(int renders = 0; renders < 60; renders++)
    #else
    for(int renders = 0; !done(); renders++)
    #endif
    {
        const uint8_t* key = SDL_GetKeyboardState(NULL);
        SDL_PumpEvents();
        hero = touch(hero, map, key);
        const int ch = handle(hero, map.walling, key);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.name);
            hero = teleport(hero, portal);
            sprites = swap(sprites, portal.name);
        }
        const Sprites relative = arrange(sprites, hero);
        const World world = { hero, relative, map };
        render(sdl, world);
        kill(relative);
        sdl = tick(sdl, renders);
    }
    release(sdl);
    close(map);
    kill(sprites);
    destroy(portals);
}
