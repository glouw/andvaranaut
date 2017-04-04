#include "Hero.h"
#include "Map.h"
#include "util.h"

void play(const char* argv[])
{
    Map map = open("maps/start.map");
    Hero hero = spawn("config/hero.cfg");
    const Portals portals = populate("config/portals.cfg");
    const int res = strtol(argv[1], NULL, 0);
    const Gpu gpu = setup(res, "config/surfaces.cfg");
    #if 0
    for(int i = 0; i < 60; i++)
    #else
    while(!done())
    #endif
    {
        hero = move(hero, map.blocks.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.blocks.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.blocks);
            hero = teleport(hero, portal);
        }
        render(hero, map.blocks, res, gpu);
    }
    release(gpu);
    close(map);
    destroy(portals);
}
