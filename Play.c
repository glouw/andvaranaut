#include "Play.h"

#include "Hero.h"
#include "Portals.h"
#include "Sdl.h"
#include "Map.h"
#include "Sprites.h"
#include "Day.h"
#include "Util.h"

static int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    return event.type == SDL_QUIT
        || event.key.keysym.sym == SDLK_F1
        || event.key.keysym.sym == SDLK_ESCAPE;
}

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    Map map = open("start");
    Sprites sprites = wake("start");
    Hero hero = spawn();
    const Portals portals = populate("portals.cfg");
    Sdl sdl = setup(res, fps, "surfaces.cfg");
    Day day = begin();
    #ifdef PROFILE
    (void) done;
    for(int renders = 0; renders < 60; renders++)
    #else
    for(int renders = 0; !done(); renders++)
    #endif
    {
        hero = move(hero, map.walling);
        hero = spin(hero);
        hero = zoom(hero);
        hero = brighten(hero);
        hero = burn(hero);
        const int ch = handle(hero, map.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.name);
            hero = teleport(hero, portal);
            sprites = swap(sprites, portal.name);
        }
        const Sprites relative = arrange(sprites, hero);
        render(sdl, hero, relative, map, day);
        kill(relative);
        sdl = tick(sdl, renders);
        day = progress(day, sdl.ticks);
    }
    close(map);
    kill(sprites);
    destroy(portals);
    release(sdl);
}
