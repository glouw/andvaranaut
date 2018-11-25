#include "World.h"

#include "Tris.h"
#include "Frame.h"
#include "util.h"

static World append(World w, const Map map, const Sprites sprites)
{
    if(w.floor == w.floors)
        u_bomb("World size limitation reached");
    w.map[w.floor] = map;
    w.sprites[w.floor] = sprites;
    w.floor++;
    return w;
}

static World make(const int floors)
{
    const World w = { u_toss(Map, floors), u_toss(Sprites, floors), 0, floors };
    return w;
}

static World carve(World w)
{
    for(int i = 0; i < w.floors; i++)
        w = append(w, t_generate(i == 0 ? p_new(0) : w.map[i - 1].trapdoors), s_spawn(128));
    return w;
}

static void theme(const World w)
{
    for(int i = 0; i < w.floors; i++)
        m_themeate(w.map[i]);
}

static void attach(const World w)
{
    for(int i = 0; i < w.floors; i++)
        m_set_trapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);

    for(int i = 1; i < w.floors; i++)
        m_set_trapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
}

static void populate(const World w)
{
    for(int i = 0; i < w.floors; i++)
        w.sprites[i] = s_populate(w.sprites[i], w.map[i]);
}

World w_make(const int floors)
{
    World w = make(floors);
    w = carve(w);
    theme(w);
    attach(w);
    populate(w);
    return w;
}

static Hero service_health(Hero hero, const Map map, const Sprites sprites, const Timer tm, const Input in)
{
    (void) map;
    (void) sprites;
    (void) tm;

    const Item equipped = i_get_equipped(hero.inventory);

    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(h_close_enough(hero, sprite->where))
        {
            if(s_impulse(sprite, tm))
            {
                if(i_successful_block(equipped, in, tm))
                    s_parried(sprite, hero.attack.velocity, tm);
                else
                    hero = h_struck(hero, sprite->state, sprite->damage);
            }
        }
    }
    return hero;
}

static Hero service_mana(Hero hero, const Map map, const Sprites sprites, const Timer tm)
{
    (void) map;
    (void) sprites;
    (void) tm;

    return hero;
}

static Hero service_fatigue(Hero hero, const Map map, const Sprites sprites, const Timer tm)
{
    (void) map;
    (void) sprites;

    hero.fatigue = (hero.gauge.max - hero.gauge.count) / hero.gauge.divisor;
    if(g_fizzled(hero.gauge, tm))
        hero.fatigue = 0;

    return hero;
}

Hero w_interact(const World world, Hero hero, const Input in, const Timer tm)
{
    const Sprites sprites = world.sprites[hero.floor];
    const Map map = world.map[hero.floor];

    hero = service_health (hero, map, sprites, tm, in);
    hero = service_mana   (hero, map, sprites, tm);
    hero = service_fatigue(hero, map, sprites, tm);

    return hero;
}
