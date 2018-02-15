#include "Maps.h"

#include "Tris.h"

#include "util.h"

Maps xmsadd(Maps maps, const Map map)
{
    if(maps.count == maps.max)
        xbomb("Maps size limitation reached");
    maps.map[maps.count++] = map;
    return maps;
}

static Maps xmsnew(const int max)
{
    const Maps maps = { xtoss(Map, max), 0, max };
    return maps;
}

Maps xmsinit(const int floors)
{
    Maps maps = xmsnew(floors);
    // Generate the first floor.
    const Points none = xpsnew(0);
    maps = xmsadd(maps, xtgenerate(none));
    // Generate remaining floors.
    for(int i = 1; i < floors; i++)
        maps = xmsadd(maps, xtgenerate(maps.map[i - 1].trapdoors));
    // Place floor trapdoors.
    for(int i = 0; i < floors; i++)
        xtrapdoors(maps.map[i - 0], maps.map[i - 0].trapdoors, FLORING);
    // Place ceiling trapdoors.
    for(int i = 1; i < floors; i++)
        xtrapdoors(maps.map[i - 0], maps.map[i - 1].trapdoors, CEILING);
    return maps;
}

void xmsclose(const Maps maps)
{
    for(int i = 0; i < maps.count; i++)
        xclose(maps.map[i]);
}
