#include "World.h"

#include "Tris.h"
#include "Frame.h"
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
        w = append(w, t_generate(i == 0 ? p_new(0) : w.map[i - 1].trapdoors), s_spawn(128));
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
        m_set_trapdoors(w.map[i], w.map[i - 0].trapdoors, FLORING);

    for(int i = 1; i < w.floors; i++)
        m_set_trapdoors(w.map[i], w.map[i - 1].trapdoors, CEILING);
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

static Hero damage_hero_health(Hero hero, const Sprites sprites, const Input in, const Timer tm)
{
    const Item equipped = i_get_equipped(hero.inventory);

    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(h_close_enough(hero, sprite->where))
        {
            // Take damage from sprite.
            if(s_impulse(sprite, tm))
                hero = h_struck(hero, sprite->state, sprite->damage);

            // Block sprite for a stun.
            if(i_successful_block(equipped, in, tm))
                s_parried(sprite, hero.attack.velocity, tm);
        }
    }
    return hero;
}

static Hero damage_hero_mana(Hero hero, const Sprites sprites, const Timer tm)
{
    (void) sprites;
    (void) tm;
    return hero;
}

static Hero damage_hero_fatigue(Hero hero, const Sprites sprites, const Timer tm)
{
    (void) tm;
    (void) sprites;

    hero.fatigue = (hero.gauge.max - hero.gauge.count) / hero.gauge.divisor;
    if(g_fizzled(hero.gauge, tm))
        hero.fatigue = 0;

    return hero;
}

// @:
// To resolve a circular dependency between Hero and Sprites, this function
// must be placed here even though the World type is not used.
Hero w_interact(Hero hero, const Sprites sprites, const Input in, const Timer tm)
{
    hero = damage_hero_health(hero, sprites, in, tm);
    hero = damage_hero_mana(hero, sprites, tm);
    hero = damage_hero_fatigue(hero, sprites, tm);
    return hero;
}
