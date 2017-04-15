#include "Hero.h"
#include "Portals.h"
#include "Gpu.h"
#include "Map.h"
#include "Sprites.h"
#include "Util.h"

void play(const char* argv[])
{
    const int res = strtol(argv[1], NULL, 0);
    const int fps = 60;
    Map map = open("start");
    Sprites sprites = wake("start");
    Hero hero = spawn("hero.cfg");
    const Portals portals = populate("portals.cfg");
    Gpu gpu = setup(res, fps, "surfaces.cfg");
    #ifdef PROFILE
    for(unsigned long long renders = 0; renders < 60; renders++)
    #else
    for(unsigned long long renders = 0; !done(); renders++)
    #endif
    {
        hero = move(hero, map.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.name);
            hero = teleport(hero, portal);
            sprites = swap(sprites, portal.name);
        }
        const Sprites updated = update(sprites, hero);
        render(gpu, hero, updated, map);
        kill(updated);
        gpu = tick(gpu, renders);
    }
    close(map);
    kill(sprites);
    destroy(portals);
    release(gpu);
}
