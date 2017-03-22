#include "Hero.h"
#include "util.h"

int main(const int argc, const char* const* const argv)
{
    Map map = open("maps/start.map");
    Hero hero = spawn();
    const Portals portals = populate("config/portals.cfg");
    if(argc != 2)
        goto end;
    const int res = strtol(argv[1], NULL, 0);
    const Gpu gpu = setup(res, "config/surfaces.cfg");
    while(!done())
    {
        hero = move(hero, map.blocks.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.blocks.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.blocks);
            hero = teleport(hero, portal, map);
        }
        render(hero, map.blocks, res, gpu);
    }
    release(gpu);
end:close(map);
    destroy(portals);
}
