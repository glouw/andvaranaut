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

static void attach(const World w)
{
    for(int i = 0; i < w.max; i++) xmtrapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);
    for(int i = 1; i < w.max; i++) xmtrapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
}

World xwinit(const int max)
{
    World w = xwnew(max);

    for(int i = 0; i < w.max; i++)
    {
        w = xwadd(w, xtgen(i == 0 ? xpsnew(0) : w.map[i - 1].trapdoors), xsnew(32));
        xmthemeate(w.map[i]);
        w.sprites[i] = xspopulate(w.sprites[i], w.map[i]);
    }
    attach(w);

    return w;
}
