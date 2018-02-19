#include "World.h"

#include "Tris.h"
#include "util.h"

World xwadd(World w, const Map map, const Sprites sprites)
{
    if(w.count == w.max)
        xbomb("World size limitation reached");
    w.map[w.count] = map;
    w.sprites[w.count] = sprites;
    w.count++;
    return w;
}

static World xwnew(const int max)
{
    const World w = { xtoss(Map, max), xtoss(Sprites, max), 0, max };
    return w;
}

World xwinit(const int max)
{
    World w = xwnew(max);
    w = xwadd(w, xtgen(xpsnew(0)), xsnew(32));
    for(int i = 1; i < max; i++)
        w = xwadd(w, xtgen(w.map[i - 1].trapdoors), xsnew(32));
    for(int i = 0; i < max; i++)
        xmtrapdoors(w.map[i - 0], w.map[i - 0].trapdoors, FLORING);
    for(int i = 1; i < max; i++)
        xmtrapdoors(w.map[i - 0], w.map[i - 1].trapdoors, CEILING);
    return w;
}

void xwclose(const World w)
{
    for(int i = 0; i < w.count; i++)
    {
        xmclose(w.map[i]);
        xkill(w.sprites[i]);
    }
}
