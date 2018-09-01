#include "World.h"

#include "Tris.h"
#include "util.h"

World xzworld()
{
    static World world;
    return world;
}

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
    // Ceiling trapdoor connection starts from floor 1 because floor 0 has a sky and skies do not have trapdoors.
    for(int i = 0; i < w.max; i++) xmtrapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);
    for(int i = 1; i < w.max; i++) xmtrapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
}

World xwinit(const int max)
{
    World w = xwnew(max);

    for(int i = 0; i < w.max; i++)
    {
        w = xwadd(w,
            // The zeroth floor does not take any extra trapdoor points because there are no trapdoors in the sky.
            xtgen(i == 0 ? xpsnew(0) : w.map[i - 1].trapdoors),
            // Allocate an arbitrary space for some number of sprites. Sprite array is dynamic and can grow.
            xsnew(32));

        // Apply room tiles according to theme.
        xmthemeate(w.map[i]);

        // Populate room sprites according to theme.
        w.sprites[i] = xspopulate(w.sprites[i], w.map[i]);
    }

    // Attach all floors by their trapdoors.
    attach(w);

    return w;
}
