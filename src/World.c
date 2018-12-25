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
    const World w = { u_toss(Map, max), u_toss(Sprites, max), 0, 0, max };
    return w;
}

static World carve(World w)
{
    while(r_themes_left())
    {
        printf("gen floor %d\n", w.index);
        const Points trapdoors = w.index == 0 ? p_new(0) : w.map[w.index - 1].trapdoors;
        const Map map = t_generate(trapdoors, 150, 250, 30, 3);
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

static void populate(const World w)
{
    for(int i = 0; i < w.index; i++)
        w.sprites[i] = s_populate(w.sprites[i], w.map[i]);
}

World w_make(const int max)
{
    World w = make(max);
    w = carve(w);
    theme(w);
    attach(w);
    populate(w);
    return w;
}

static Hero service_health(Hero hero, const Map map, const Sprites sprites, const Timer tm)
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
                hero = h_struck(hero, sprite->state, sprite->damage);

            if(i_successful_block(equipped, hero.gauge))
                s_parried(sprite, hero.attack.velocity, tm);
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

    if(hero.gauge.warning)
    {
        hero.gauge.warning = false;

        const char* const tireds[] = {
            "Exausted...",
            "So tired...",
            "Muscles aching...",
        };
        const int which = rand() % u_len(tireds);
        t_set_title(tm.renders, 120, false, tireds[which]);
    }

    hero.fatigue = g_fizzled(hero.gauge, tm) ? 0.0f : (hero.gauge.max - hero.gauge.count);

    return hero;
}

Hero w_interact(const World world, Hero hero, const Timer tm)
{
    const Sprites sprites = world.sprites[hero.floor];
    const Map map = world.map[hero.floor];

    hero = service_health (hero, map, sprites, tm);
    hero = service_mana   (hero, map, sprites, tm);
    hero = service_fatigue(hero, map, sprites, tm);

    return hero;
}
