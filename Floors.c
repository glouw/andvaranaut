#include "Floors.h"

#include "Tris.h"

#include "util.h"

Floors xfsadd(Floors fs, const Map map)
{
    if(fs.count == fs.max)
        xbomb("Floor size limitation reached");
    fs.map[fs.count++] = map;
    return fs;
}

static Floors xfsnew(const int max)
{
    const Floors fs = { xtoss(Map, max), 0, max };
    return fs;
}

Floors xfsinit(const int max)
{
    Floors fs = xfsnew(max);
    // Generate the first floor.
    const Points none = xpsnew(0);
    fs = xfsadd(fs, xtgen(none));
    // Generate remaining floors.
    for(int i = 1; i < max; i++)
        fs = xfsadd(fs, xtgen(fs.map[i - 1].trapdoors));
    // Place floor trapdoors.
    for(int i = 0; i < max; i++)
        xtrapdoors(fs.map[i - 0], fs.map[i - 0].trapdoors, FLORING);
    // Place ceiling trapdoors.
    for(int i = 1; i < max; i++)
        xtrapdoors(fs.map[i - 0], fs.map[i - 1].trapdoors, CEILING);
    return fs;
}

void xfsclose(const Floors fs)
{
    for(int i = 0; i < fs.count; i++)
        xclose(fs.map[i]);
}
