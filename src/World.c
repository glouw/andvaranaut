#include "World.h"

#include "Tris.h"
#include "util.h"

// Appends a new floor to the world map.
static World append(World w, const Map map, const Sprites sprites)
{
    if(w.floor == w.floors)
        u_bomb("World size limitation reached");
    w.map[w.floor] = map;
    w.sprites[w.floor] = sprites;
    w.floor++;
    return w;
}

// Allocates memory for all floors.
static World make(const int floors)
{
    const World w = { u_toss(Map, floors), u_toss(Sprites, floors), 0, floors };
    return w;
}

// Carves rooms into each floor.
static World carve(World w)
{
    for(int i = 0; i < w.floors; i++)
        w = append(w, t_gen(i == 0 ? p_new(0) : w.map[i - 1].trapdoors), s_spawn(128));
    return w;
}

// Applies theme to each room of each floor.
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
        w.sprites[i] = s_populate(w.sprites[i], w.map[i]);
}

World w_make(const int floors)
{
    World w = carve(make(floors));
    theme(w);
    attach(w);
    populate(w);
    return w;
}
