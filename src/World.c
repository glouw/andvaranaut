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
    // The zeroth floor does not take any extra trapdoor arguments because there are no trapdoors in the sky.
    for(int i = 0; i < max; i++)
        w = xwadd(w,
            xtgen(i == 0 ? xpsnew(0) : w.map[i - 1].trapdoors),
            xsnew(32));
    // Ceiling trapdoor connection starts from floor 1 because floor 0 has a sky and skies do not have trapdoors.
    for(int i = 0; i < max; i++) xmtrapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);
    for(int i = 1; i < max; i++) xmtrapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
    // Am I being too cheeky?
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
