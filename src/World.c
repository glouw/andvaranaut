#include "World.h"

#include "Tris.h"
#include "util.h"

static World append(World w, const Map map, const Sprites sprites)
{
    if(w.floor == w.floors)
        xbomb("World size limitation reached");
    w.map[w.floor] = map;
    w.sprites[w.floor] = sprites;
    w.floor++;
    return w;
}

static World wnew(const int floors)
{
    const World w = { xtoss(Map, floors), xtoss(Sprites, floors), 0, floors };
    return w;
}

// Builds all floors and carves out rooms.
static World build(World w)
{
    for(int i = 0; i < w.floors; i++)
        w = append(w, xtgen(i == 0 ? p_new(0) : w.map[i - 1].trapdoors), xsnew(128));
    return w;
}

// Applies themes to each room.
static void theme(const World w)
{
    for(int i = 0; i < w.floors; i++)
        m_themeate(w.map[i]);
}

// Links all floors with trapdoors.
static void attach(const World w)
{
    for(int i = 0; i < w.floors; i++)
        m_trapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);

    for(int i = 1; i < w.floors; i++)
        m_trapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
}

// Populates rooms with sprites based on room theme.
static void populate(const World w)
{
    for(int i = 0; i < w.floors; i++)
        w.sprites[i] = xspopulate(w.sprites[i], w.map[i]);
}

World xwinit(const int floors)
{
    World w = build(wnew(floors));
    theme(w);
    attach(w);
    populate(w);
    return w;
}
