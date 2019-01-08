#include "World.h"

#include "Title.h"
#include "Tris.h"
#include "Frame.h"
#include "util.h"

static World append(World w, const Map map, const Sprites sprites)
{
    if(w.index == w.max)
        u_bomb("World size limitation reached");
    w.map[w.index] = map;
    w.sprites[w.index] = sprites;
    w.index++;
    return w;
}

static World make(const int max)
{
    const World w = { u_toss(Map, max), u_toss(Sprites, max), 0, max };
    return w;
}

static World carve(World w)
{
    while(r_themes_left())
    {
        const Points trapdoors = w.index == 0 ? p_new(0) : w.map[w.index - 1].trapdoors;

        const Map map = t_generate(trapdoors, 150, 250, 30, 3, w.index);

        const Sprites sprites = s_spawn(128);

        w = append(w, map, sprites);
    }
    return w;
}

static void theme(const World w)
{
    for(int i = 0; i < w.index; i++)
        m_themeate(w.map[i]);
}

static void attach(const World w)
{
    for(int i = 0; i < w.index - 1; i++)
        m_set_trapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);

    for(int i = 1; i < w.index - 0; i++)
        m_set_trapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
}

static void populate(const World w, const Timer tm)
{
    for(int i = 0; i < w.index; i++)
        w.sprites[i] = s_populate(w.sprites[i], w.map[i], tm);
}

World w_make(const int max, const Timer tm)
{
    World w = make(max);
    w = carve(w);
    theme(w);
    attach(w);
    populate(w, tm);
    return w;
}

Hero w_transport(const World world, const Hero hero, const Sdl sdl, const Text red, const Text yel, const Input in)
{
    const int numer = i_get_numer_key(in);
    const int alpha = i_get_alpha_key(in);

    const int flor = numer == EOF ? hero.floor : numer - '0';
    const int room = alpha == EOF ? EOF : alpha - 'A';

    const Rooms rooms = world.map[flor].rooms;

    if(flor < world.index)
    {
        s_draw_room_lookup(sdl, yel, red, rooms, flor, room);

        if(room != EOF
        && room < rooms.count)
            return h_place(hero, rooms.room[room].where, flor, hero.height);
    }
    return hero;
}

Room w_get_starting_room(const World world)
{
    for(int j = 0; j < world.index; j++)
    {
        const Map map = world.map[j];

        for(int i = 0; i < map.rooms.count; i++)
        {
            const Room room = map.rooms.room[i];
            puts(t_get_name(room.theme));
            if(room.theme == STARTING_ROOM)
            {
                puts("... MATCH");
                return room;
            }
        }
    }
    static Room none;
    return none;
}
