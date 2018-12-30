#include "Sprites.h"

#include "Field.h"
#include "Title.h"
#include "Compass.h"
#include "Frame.h"
#include "Speech.h"
#include "Inventory.h"
#include "util.h"

#include <ctype.h>
#include <math.h>

Sprites s_spawn(const int max)
{
    const Sprites sprites = { u_toss(Sprite, max), 0, max, NO_ATTACK };
    return sprites;
}

static Sprites append(Sprites sprites, const Sprite sprite)
{
    if(sprites.max == 0)
        u_retoss(sprites.sprite, Sprite, sprites.max = 1);

    if(sprites.count >= sprites.max)
        u_retoss(sprites.sprite, Sprite, sprites.max *= 2);

    sprites.sprite[sprites.count++] = sprite;
    return sprites;
}

Sprites s_lay(Sprites sprites, const Map map, const Overview ov, const Timer tm)
{
    if(m_out_of_bounds(map, ov.where))
        return sprites;

    const int ascii = ov.selected + ' ';
    if(s_sprite(ascii))
        sprites = append(sprites, s_register(ascii, ov.where, tm));

    return sprites;
}

void s_pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = p_sub(sprite->where, hero.where);
    }
}

void s_push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = p_add(sprite->where, hero.where);
    }
}

void s_sort(const Sprites sprites, Sorter sorter)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), sorter);
}

void s_turn(const Sprites sprites, const float yaw)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = p_turn(sprite->where, yaw);
    }
}

static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        static Point zero;

        // Stuck in a wall.
        if(p_tile(sprite->where, map.walling))
        {
            s_place(sprite, p_mid(sprite->last));
            sprite->velocity = zero;
        }

        // Stuck in water.
        if(p_char(sprite->where, map.floring) == ' ')
        {
            s_place(sprite, p_mid(sprite->last));
            sprite->velocity = zero;
        }
    }
}

static void move(const Sprites sprites, const Field field, const Point to, const Map map, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        const float slowdown = tm.slowmo ? tm.slowdown : 1.0f;
        const float speed = sprite->speed / slowdown;
        const float accel = sprite->acceleration / slowdown;

        if(s_stuck(sprite))
            continue;

        const Point dir = f_generate_force(field, sprite->where, to, map);

        // No force applied - slow down.
        if(dir.x == 0.0f && dir.y == 0.0f)
        {
            if(speed == 0.0f)
                continue;

            sprite->velocity = p_mul(sprite->velocity, 1.0f - accel / speed);
        }
        // Force applied - speed up.
        else
        {
            const Point acc = p_mul(dir, accel);
            sprite->velocity = p_add(sprite->velocity, acc);

            if(p_mag(sprite->velocity) > speed)
                sprite->velocity = p_mul(p_unit(sprite->velocity), speed);
        }

        s_place(sprite, p_add(sprite->where, sprite->velocity));

        if(s_busy(sprite, tm))
            continue;

        // Sprite will stop running if close to player.
        sprite->state = p_mag(sprite->velocity) > 0.005f ? CHASING : IDLE;
    }
}

static void scent_wall(const Field field, const Point where, const Map map, const float scent)
{
    const int t = field.res * where.y - field.aura; // Top.
    const int b = field.res * where.y + field.aura; // Bottom.
    const int l = field.res * where.x - field.aura; // Left.
    const int r = field.res * where.x + field.aura; // Right.

    for(int j = t; j <= b; j++)
    for(int i = l; i <= r; i++)
    {
        const int x = i / field.res;
        const int y = j / field.res;

        if(f_is_on(field, j, i))
        {
            // Scent walls.
            field.mesh[j][i] = map.walling[y][x] == ' ' ? 0.0f : -scent;

            // Scent water.
            if(map.floring[y][x] == ' ')
                field.mesh[j][i] = -scent;
        }
    }
}

static void scent_sprite(const Field field, const Sprites sprites, const float scent)
{
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];

        if(s_cosmetic(sprite->ascii))
            continue;

        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        for(int a = -field.res / 2; a <= field.res / 2; a++)
        for(int b = -field.res / 2; b <= field.res / 2; b++)
            if(f_is_on(field, j + a, i + b))
                field.mesh[j + a][i + b] -= scent;
    }
}

static void scent_hero(const Field field, const Point where, const float scent)
{
    const int j = field.res * where.y;
    const int i = field.res * where.x;
    field.mesh[j][i] = scent;
}

static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    const float scent = 1000.0f;
    scent_wall(field, hero.where, map, scent);
    scent_sprite(field, sprites, scent);
    scent_hero(field, hero.where, 100.0f * scent);
    f_diffuse(field, hero.where);
}

static Sprites drop_item(Sprites sprites, const Attack attack, const Point where, const Timer tm)
{
    const Point delta = p_mul(p_rot90(attack.velocity), 0.5f);
    return append(sprites, s_register('d', p_add(where, delta), tm));
}

static void broke_lootbag(const int ascii, const Inventory inv, const Timer tm)
{
    if(ascii == 'd')
    {
        const Item item = i_rand();
        const int fit = i_add(inv.items, item);
        if(fit)
            t_set_title(tm.renders, 120, false, "Picked up %s", item.cstr);
        else
            t_set_title(tm.renders, 120, false, "Inventory Full");
    }
}

static Sprites damage(Sprites sprites, Sprite* const sprite, const Attack attack, const Inventory inv, const Timer tm)
{
    // Check attack speed.
    static Point zero;
    if(p_same(attack.velocity, zero))
    {
        t_set_title(tm.renders, 60, false, "Too slow!");
        return sprites;
    }

    // Sprite is now angry.
    if(sprite->evil == false)
    {
        static Speech none;
        sprite->evil = true;
        sprite->speech = none;
    }

    // Sprite attempts to Block.
    const Point vel = p_unit(attack.velocity);
    const int s = p_south(vel);
    const int n = p_north(vel);
    const int e = p_east (vel);
    const int w = p_west (vel);
    if((sprite->state == BLOCK_N && s)
    || (sprite->state == BLOCK_S && n)
    || (sprite->state == BLOCK_W && e)
    || (sprite->state == BLOCK_E && w))
        t_set_title(tm.renders, 60, false, "Blocked!");
    // If the block was not successful Sprite is now either hurt or dead.
    else
    {
        sprite->health -= attack.item.damage;

        // Hurt.
        if(sprite->health <= 0.0f)
        {
            sprite->state = s ? DEAD_N : n ? DEAD_S : e ? DEAD_W : DEAD_E;
            broke_lootbag(sprite->ascii, inv, tm);
            if(u_d10() == 0)
                return drop_item(sprites, attack, sprite->where, tm);
        }
        // Dead.
        else
        {
            const State hurt = s ? HURT_N : n ? HURT_S : e ? HURT_W : HURT_E;
            // TODO: Different sprites have different stun times.
            s_go_busy(sprite, tm, 3 * FRAMES, hurt);
        }
    }
    return sprites;
}

static Sprites damage_melee(Sprites sprites, const Timer tm, const Hero hero)
{
    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(i_can_block(hero.attack.item))
            continue;

        if(h_close_enough(hero, sprite->where))
        {
            sprites = damage(sprites, sprite, hero.attack, hero.inventory, tm);
            if(++hurts == hero.attack.item.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites damage_range(Sprites sprites, const Timer tm, const Hero hero)
{
    const SDL_Point point = {
        (int) hero.attack.reticule.x,
        (int) hero.attack.reticule.y,
    };
    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(SDL_PointInRect(&point, &sprite->seen))
        {
            sprites = damage(sprites, sprite, hero.attack, hero.inventory, tm);
            if(++hurts == hero.attack.item.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites damage_magic(Sprites sprites, const Timer tm, const Hero hero)
{
    // TODO
    // Casting magic scrolls will spawn new sprites.
    // These sprites will do something like heal the hero, teleport the hero, or be something like fire
    // which hurts, heals, or teleports other sprites.
    (void) hero;
    (void) tm;
    return sprites;
}

Sprites s_hero_damage_sprites(Sprites sprites, const Hero hero, const Timer tm)
{
    if(hero.gauge.ready)
    {
        sprites.last = hero.attack.method;

        if(hero.attack.method == MELEE) return damage_melee(sprites, tm, hero);
        if(hero.attack.method == RANGE) return damage_range(sprites, tm, hero);
        if(hero.attack.method == MAGIC) return damage_magic(sprites, tm, hero);
    }
    sprites.last = NO_ATTACK;

    return sprites;
}

static void idle(const Sprites sprites, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_dead(sprite->state))
            continue;

        if(s_busy(sprite, tm))
            continue;

        sprite->state = IDLE;
    }
}

static void rage(const Sprites sprites, const Hero hero, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(s_will_rage(sprite, tm) && h_close_enough(hero, sprite->where))
        {
            const Compass direction = (Compass) (rand() % DIRS);
            const State attack = (State) ((int) direction + (int) ATTACK_N);
            s_go_busy(sprite, tm, 1 * FRAMES, attack);
        }
    }
}

static void block(const Sprites sprites, const Hero hero, const Timer tm)
{
    if(hero.gauge.count > 0)
    {
        for(int i = 0; i < sprites.count; i++)
        {
            Sprite* const sprite = &sprites.sprite[i];

            if(s_useless(sprite))
                continue;

            if(s_busy(sprite, tm))
                continue;

            if(i_can_block(hero.attack.item))
                continue;

            if(s_inanimate(sprite->ascii))
                continue;

            if(h_close_enough(hero, sprite->where))
            {
                // TODO: Subtract sprite dependent value from gauge count to have sprites react slower.
                const Point block = g_sum(hero.gauge, hero.gauge.count);
                if(p_north(block)) sprite->state = BLOCK_N;
                if(p_east (block)) sprite->state = BLOCK_E;
                if(p_south(block)) sprite->state = BLOCK_S;
                if(p_west (block)) sprite->state = BLOCK_W;
                if(tm.fall
                && sprite->evil
                && tm.ticks > sprite->block_time + sprite->block_start)
                {
                    sprite->state = IDLE;
                    sprite->block_start = tm.ticks;
                }
            }
            else sprite->block_start = tm.ticks;
        }
    }
}

static void speak(const Sprites sprites, const Hero hero, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_dead(sprite->state))
            continue;

        if(s_muted(sprite))
            continue;

        if(h_close_enough(hero, sprite->where))
        {
            const int speed = 8; // How fast sprite talks (arbitrary pick).
            const int ticks = tm.ticks - sprite->speech.ticks;

            sprite->speech.index = (ticks / speed) % sprite->speech.count;

            const int index = sprite->speech.index;
            const char* const sentence = sprite->speech.sentences[index];
            if(strlen(sentence) > 0)
                sprite->state = SPEAKING;
        }
        else sprite->speech.ticks = tm.ticks;
    }
}

static void track(const Sprites sprites, const Fire fire)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_firey(sprite->ascii))
        {
            const int x = sprite->where.x;
            const int y = sprite->where.y;
            fire.embers[y][x] = e_append(fire.embers[y][x], sprite);
        }
    }
}

static void burn(const Sprites sprites, const Fire fire)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_firey(sprite->ascii))
            continue;

        const int x = sprite->where.x;
        const int y = sprite->where.y;
        const Embers embers = fire.embers[y][x];

        // TODO fire must hurt sprites.
        for(int j = 0; j < embers.count; j++)
            embers.ember[j]->state = ATTACK_N;
    }
}

Sprites s_spread_fire(Sprites sprites, const Fire fire, const Map map, const Timer tm)
{
    if(tm.rise)
    {
        // Do not want growing sprite count.
        const int count = sprites.count;

        for(int i = 0; i < count; i++)
        {
            Sprite* const sprite = &sprites.sprite[i];

            if(s_must_spread(sprite, map.floring))
            {
                const Point dir = {
                    2.0f * (u_frand() - 0.5f),
                    2.0f * (u_frand() - 0.5f),
                };
                const Point where = p_add(sprite->where, dir);

                // Do no burn into walls.
                if(p_char(where, map.walling) == ' ')
                {
                    const int xx = where.x;
                    const int yy = where.y;
                    if(fire.embers[yy][xx].count < 2)
                    {
                        sprites = append(sprites, s_register('c', where, tm));
                        Sprite* const ember = &sprites.sprite[sprites.count - 1];
                        fire.embers[yy][xx] = e_append(fire.embers[yy][xx], ember);
                    }
                }
            }
        }
    }
    return sprites;
}

static Hero service_hero_health(Hero hero, const Map map, const Sprites sprites, const Timer tm)
{
    (void) map;
    (void) sprites;
    (void) tm;

    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(h_close_enough(hero, sprite->where))
        {
            if(s_impulse(sprite, tm))
                hero = h_struck(hero, sprite->state, sprite->damage);

            if(i_successful_block(hero.attack.item, hero.gauge))
                s_parried(sprite, hero.attack.velocity, tm);
        }
    }
    return hero;
}

static Hero service_hero_mana(Hero hero, const Map map, const Sprites sprites, const Timer tm)
{
    (void) map;
    (void) sprites;
    (void) tm;

    return hero;
}

static Hero service_hero_fatigue(Hero hero, const Map map, const Sprites sprites, const Timer tm)
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

static Hero sprites_damage_hero(const Sprites sprites, Hero hero, const Map map, const Timer tm)
{
    hero = service_hero_health (hero, map, sprites, tm);
    hero = service_hero_mana   (hero, map, sprites, tm);
    hero = service_hero_fatigue(hero, map, sprites, tm);

    return hero;
}

static Hero trade(const Sprites sprites, Hero hero, const Input in, const Timer tm)
{
    if(hero.inventory.trade.id.clas != NONE)
    {
        for(int i = 0; i < sprites.count; i++)
        {
            Sprite* const sprite = &sprites.sprite[i];

            if(s_useless(sprite))
                continue;

            const SDL_Point to = { in.x, in.y };

            if(SDL_PointInRect(&to, &sprite->seen))
            {
                if(i_same_id(sprite->wants, hero.inventory.trade.id)) // Wants.
                {
                    sprite->speech = s_use_grateful(tm);
                    sprite->wants.clas = NONE;
                    sprite->evil = false;
                }
                else // Does not want.
                {
                    sprite->speech = s_use_unwanted(tm);
                    i_add(hero.inventory.items, hero.inventory.trade); // Put back.
                }
                break;
            }
        }
    }
    hero.inventory.trade = i_none();
    return hero;
}

Hero s_caretake(const Sprites sprites, Hero hero, const Map map, const Field field, const Fire fire, const Timer tm, const Input in)
{
    s_pull(sprites, hero);
    s_sort(sprites, s_nearest_sprite_first);
    s_push(sprites, hero);

    idle(sprites, tm);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where, map, tm);
    bound(sprites, map);
    speak(sprites, hero, tm);
    block(sprites, hero, tm);
    track(sprites, fire);
    burn(sprites, fire);
    rage(sprites, hero, tm);

    hero = trade(sprites, hero, in, tm);

    return sprites_damage_hero(sprites, hero, map, tm);
}

static Point avail(const Point center, const Map map)
{
    const Point random = {
        rand() % map.grid - map.grid / 2.0f,
        rand() % map.grid - map.grid / 2.0f,
    };
    const Point where = p_add(center, random);

    // Available tile space is no wall and not above water.
    return p_char(where, map.walling) == ' '
        && p_char(where, map.floring) != ' ' ? where : avail(center, map);
}

static Point seek(const Point center, const Map map, const int ascii)
{
    const Point where = avail(center, map);
    return p_char(where, map.floring) == ascii ? where : seek(center, map, ascii);
}

static Sprites lay_nice_garden(Sprites sprites, const Map map, const Point center, const Timer tm)
{
    const int flowers = 256;
    for(int i = 0; i < flowers; i++)
        sprites = append(sprites, s_register('a', seek(center, map, '('), tm));

    // Gardener.
    sprites = append(sprites, s_register('b', avail(center, map), tm));
    return sprites;
}

static Sprites place_dummy(const Sprites sprites, const Map map, const Point center, const Timer tm)
{
    return append(sprites, s_register('b', avail(center, map), tm));
}

Sprites s_populate(Sprites sprites, const Map map, const Timer tm)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point center = map.rooms.wheres[i];

        switch(map.rooms.themes[i])
        {
        case AN_EMPTY_ROOM:
            sprites = place_dummy(sprites, map, center, tm);
            break;

        case A_NICE_GARDEN:
            sprites = lay_nice_garden(sprites, map, center, tm);
            break;

        case A_WELL_OF_WATER:
            sprites = place_dummy(sprites, map, center, tm);
            break;

        case NO_THEME:
        case THEMES:
            break;
        }
    }
    return sprites;
}

Map s_count_agents(const Sprites sprites, Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        map.rooms.agents[i] = 0;
        for(int s = 0; s < sprites.count; s++)
        {
            Sprite* const sprite = &sprites.sprite[s];

            if(s_not_agent(sprite))
                continue;

            if(p_eql(sprite->where, map.rooms.wheres[i], map.grid))
                map.rooms.agents[i]++;
        }
    }
    return map;
}
