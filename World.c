#include "World.h"

#include "Tris.h"
#include "util.h"

World xwadd(World w, const Map map)
{
    if(w.count == w.max)
        xbomb("World size limitation reached");
    w.map[w.count++] = map;
    return w;
}

static World xwnew(const int max)
{
    const World w = { xtoss(Map, max), 0, max };
    return w;
}

World xwinit(const int max)
{
    World w = xwnew(max);
    // Generate the first floor.
    const Points none = xpsnew(0);
    w = xwadd(w, xtgen(none));
    // Generate remaining floors.
    for(int i = 1; i < max; i++)
        w = xwadd(w, xtgen(w.map[i - 1].trapdoors));
    // Place floor trapdoors.
    for(int i = 0; i < max; i++)
        xmtrapdoors(w.map[i - 0], w.map[i - 0].trapdoors, FLORING);
    // Place ceiling trapdoors.
    for(int i = 1; i < max; i++)
        xmtrapdoors(w.map[i - 0], w.map[i - 1].trapdoors, CEILING);
    return w;
}

void xwclose(const World w)
{
    for(int i = 0; i < w.count; i++)
        xmclose(w.map[i]);
}
