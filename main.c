#include "Hit.h"
#include "Line.h"
#include "Point.h"
#include "Map.h"
#include "Gpu.h"
#include "Display.h"
#include "Wall.h"
#include "Hero.h"
#include "Portals.h"
#include "Frame.h"
#include "util.h"

#include <SDL2/SDL.h>

static int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT
    || event.key.keysym.sym == SDLK_F1
    || event.key.keysym.sym == SDLK_ESCAPE)
        return 1;
    return 0;
}

int main(const int argc, const char* const* const argv)
{
    Map map = open("maps/start.map");
    Hero hero = {
        .inside = 1,
        .where = { 1.5, 5.5 },
        .velocity  = { 0.0, 0.0 },
        .acceleration = 0.015,
        .speed = 0.12,
        .theta = 0.0,
        .fov = {
            { +1.0, -1.0 },
            { +1.0, +1.0 }
        }
    };
    Portals portals = populate("maps.cfg");
    if(argc != 2) goto end;
    const int res = atoi(argv[1]);
    const Gpu gpu = setup(res);
    while(!done())
    {
        hero = move(hero, map.blocks.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.blocks.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.blocks);
            hero.where = portal.where;
            hero.inside = map.meta.inside;
        }
        render(hero, map.blocks, res, gpu);
    }
    release(gpu);
end:close(map);
    destroy(portals);
}
