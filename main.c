#include "Hero.h"
#include "Map.h"
#include "util.h"

int main(const int argc, const char* const* const argv)
{
    Map map = open("maps/start.map");
    Hero hero = spawn("config/hero.cfg");
    const Portals portals = populate("config/portals.cfg");
    const int res = strtol(argv[1], NULL, 0);
    const Gpu gpu = setup(res, "config/surfaces.cfg");
    if(argc != 2)
        goto end;
    #if 1
    for(int frame = 0; frame < 60; frame++)
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
end:release(gpu);
    close(map);
    destroy(portals);
}
