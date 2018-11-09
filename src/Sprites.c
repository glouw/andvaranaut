#include "Sprites.h"

#include "Field.h"
#include "Title.h"
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

Sprites s_lay(Sprites sprites, const Map map, const Overview ov)
{
    if(m_out(map, ov.where))
        return sprites;

    const int ascii = ov.selected + ' ';
    if(s_sprite(ascii))
        sprites = append(sprites, s_register(ascii, ov.where));

    return sprites;
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = p_sub(sprite->where, hero.where);
    }
}

static void push(const Sprites sprites, const Hero hero)
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

static void turn(const Sprites sprites, const float yaw)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = p_turn(sprite->where, yaw);
    }
}

void s_orient(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    turn(sprites, -hero.yaw);
}

void s_place_back(const Sprites sprites, const Hero hero)
{
    turn(sprites, +hero.yaw);
    push(sprites, hero);
}

static void arrange(const Sprites sprites, const Hero hero, Sorter dir)
{
    pull(sprites, hero);
    s_sort(sprites, dir);
    push(sprites, hero);
}

static void bound(const Sprites sprites, const Map map)
{
    static Point zero;
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        // Stuck in a wall.
        if(p_tile(sprite->where, map.walling))
        {
            s_place(sprite, p_mid(sprite->last));
            sprite->velocity = zero;
        }

        // Stuck in water.
        if(p_block(sprite->where, map.floring) == ' ')
        {
            s_place(sprite, p_mid(sprite->last));
            sprite->velocity = zero;
        }
    }
}

static void move(const Sprites sprites, const Field field, const Point to, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(s_stuck(sprite))
            continue;
        const Point dir = f_force(field, sprite->where, to, map);

        // No force applied - slow down.
        if(dir.x == 0.0f && dir.y == 0.0f)
        {
            if(sprite->speed == 0.0f)
                continue;

            sprite->velocity = p_mul(sprite->velocity, 1.0f - sprite->acceleration / sprite->speed);
        }
        // Force applied - speed up.
        else
        {
            const Point acc = p_mul(dir, sprite->acceleration);
            sprite->velocity = p_add(sprite->velocity, acc);

            if(p_mag(sprite->velocity) > sprite->speed)
                sprite->velocity = p_mul(p_unit(sprite->velocity), sprite->speed);
        }

        // Sprite will stop running if close to player.
        sprite->state = p_mag(sprite->velocity) > 0.005f ? CHASING : IDLE;
        s_place(sprite, p_add(sprite->where, sprite->velocity));
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

        if(f_on(field, j, i))
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
            if(f_on(field, j + a, i + b))
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

static Sprites drop_item(Sprites sprites, const Attack attack, const Point where)
{
    const Point delta = p_mul(p_rot90(attack.dir), 0.5f);
    return append(sprites, s_register('d', p_add(where, delta)));
}

static void broke_lootbag(const int ascii, const Inventory inv, const Timer tm)
{
    if(ascii == 'd')
    {
        const Item item = i_rand();
        const int fit = i_add(inv.items, item);
        if(fit)
            t_set(tm.renders, tm.renders + 120, false, "Picked up %s", item.cstr);
        else
            t_set(tm.renders, tm.renders + 120, false, "Inventory Full");
    }
}

static Sprites harm(Sprites sprites, Sprite* const sprite, const Attack attack, const Inventory inv, const Timer tm)
{
    const int side = fabsf(attack.dir.x) > fabsf(attack.dir.y);

    sprite->health -= attack.power;

    // Dead.
    if(sprite->health <= 0.0f)
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? DEAD_W : DEAD_E):
            (attack.dir.y > 0.0f ? DEAD_N : DEAD_S);

        broke_lootbag(sprite->ascii, inv, tm);

        // Lootbags count as sprites, so killing a lootbag may drop another lootbag.
        if(u_d10() == 0)
            return drop_item(sprites, attack, sprite->where);
    }
    else // Just hurt, not killed.
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? HURT_W : HURT_E):
            (attack.dir.y > 0.0f ? HURT_N : HURT_S);

        s_stun(sprite, tm);

        // Hurt a good sprite, make 'em angry.
        if(!sprite->evil)
        {
            sprite->evil = true;
            static Speech zero;
            sprite->speech = zero;
        }
    }
    return sprites;
}

static Sprites hurt_melee(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm, const Hero hero)
{
    const Point hand = h_touch(hero);
    const Item it = inv.items.item[inv.selected];

    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        if(p_eql(hand, sprite->where, 2.0f))
        {
            sprites = harm(sprites, sprite, attack, inv, tm);
            if(++hurts == it.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites hurt_range(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm)
{
    const SDL_Point point = {
        (int) attack.reticule.x,
        (int) attack.reticule.y,
    };
    const Item it = inv.items.item[inv.selected];

    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        const int hit = SDL_PointInRect(&point, &sprite->seen);

        if(hit)
        {
            sprites = harm(sprites, sprite, attack, inv, tm);
            if(++hurts == it.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites hurt_magic(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm, const Hero hero)
{
    // TODO
    // Casting magic scrolls will spawn new sprites.
    // These sprites will do something like heal the hero, teleport the hero, or be something like fire
    // which hurts, heals, or teleports other sprites.
    const Item it = inv.items.item[inv.selected];

    (void) attack;
    (void) hero;
    (void) inv;
    (void) tm;
    (void) it;

    return sprites;
}

Sprites s_hurt(Sprites sprites, const Attack attack, const Hero hero, const Input in, const Inventory inv, const Timer tm)
{
    if(in.lu)
    {
        sprites.last = attack.method;

        if(attack.method == MELEE)
            return hurt_melee(sprites, attack, inv, tm, hero);

        if(attack.method == RANGE)
            return hurt_range(sprites, attack, inv, tm);

        if(attack.method == MAGIC)
            return hurt_magic(sprites, attack, inv, tm, hero);
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

        if(!s_stunned(sprite, tm))
            sprite->state = IDLE;
    }
}

static Hero damage_hps(Hero hero, const Sprites sprites, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(s_useless(sprite))
            continue;

        // TODO: Maybe sprites have different attack ranges.
        if(sprite->evil
        && sprite->state == IDLE
        && p_eql(hero.where, sprite->where, 2.2f))
        {
            // Will need another timer to expire before a new attack direction.
            sprite->state = ATTACK_N;

            if(s_impulse(sprite, tm))
            {
                // TODO: Hero defense lessens sprite damage?
                hero.hps -= sprite->damage;

                // TODO: More recoil for larger damage taken. Use compass for direction.
                hero.dpitch = -0.025f;
            }
        }
    }
    return hero;
}

static Hero damage_mna(Hero hero, const Sprites sprites, const Timer tm)
{
    (void) sprites;
    (void) tm;
    return hero;
}

static Hero damage_ftg(Hero hero, const Timer tm, const Gauge gauge)
{
    (void) tm;

    hero.ftg = (gauge.max - gauge.count) / gauge.divisor;

    if(g_fizzled(gauge, tm))
        hero.ftg = 0;

    return hero;
}

static void block(const Sprites sprites, const Hero hero, const Gauge gauge)
{
    if(gauge.count > 0)
    {
        const Point where = gauge.points[gauge.count - 1];
        const Point unit = p_unit(where);

        for(int i = 0; i < sprites.count; i++)
        {
            Sprite* const sprite = &sprites.sprite[i];

            if(s_useless(sprite))
                continue;

            if(p_eql(hero.where, sprite->where, 2.5f))
            {
                if(unit.y < 0 && fabs(unit.y) > fabs(unit.x)) sprite->state = BLOCK_N;
                if(unit.y > 0 && fabs(unit.y) > fabs(unit.x)) sprite->state = BLOCK_S;
                if(unit.x > 0 && fabs(unit.x) > fabs(unit.y)) sprite->state = BLOCK_E;
                if(unit.x < 0 && fabs(unit.x) > fabs(unit.y)) sprite->state = BLOCK_W;
            }
        }
    }
}

static Hero damage(Hero hero, const Sprites sprites, const Gauge gauge, const Timer tm)
{
    hero = damage_hps(hero, sprites, tm);
    hero = damage_mna(hero, sprites, tm);
    hero = damage_ftg(hero, tm, gauge);
    return hero;
}

static void speak(const Sprites sprites, const Hero hero, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        // Mute check very important else segfault.
        if(s_dead(sprite->state) || s_muted(sprite))
            continue;

        if(p_eql(hero.where, sprite->where, 2.5f))
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

        for(int j = 0; j < embers.count; j++)
            embers.ember[j]->state = ATTACK_N;
    }
}

Hero s_caretake(const Sprites sprites, const Hero hero, const Map map, const Field field, const Gauge gauge, const Fire fire, const Timer tm)
{
    arrange(sprites, hero, s_nearest_first);
    idle(sprites, tm);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where, map);
    bound(sprites, map);
    speak(sprites, hero, tm);
    block(sprites, hero, gauge);
    track(sprites, fire);
    burn(sprites, fire);
    return damage(hero, sprites, gauge, tm);
}

static Point avail(const Point center, const Map map)
{
    const Point where = p_add(center, p_rand(map.grid));

    // Available tile space is no wall and not above water.
    return p_block(where, map.walling) == ' '
        && p_block(where, map.floring) != ' ' ? where : avail(center, map);
}

static Point seek(const Point center, const Map map, const int ascii)
{
    const Point where = avail(center, map);
    return p_block(where, map.floring) == ascii ? where : seek(center, map, ascii);
}

static Sprites lay_nice_garden(Sprites sprites, const Map map, const Point center)
{
    const int flowers = 256;
    for(int i = 0; i < flowers; i++)
        sprites = append(sprites, s_register('a', seek(center, map, '(')));

    // Gardener.
    sprites = append(sprites, s_register('b', avail(center, map)));
    return sprites;
}

static Sprites place_dummy(const Sprites sprites, const Map map, const Point center)
{
    return append(sprites, s_register('b', avail(center, map)));
}

Sprites s_populate(Sprites sprites, const Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point center = map.rooms.wheres[i];

        switch(map.rooms.themes[i])
        {
        case NICE_GARDEN:
            sprites = lay_nice_garden(sprites, map, center);
            break;

        // TODO: TEMP. Just puts a guy in a room for now so that each room has some sort of placeholder.
        default:
            sprites = place_dummy(sprites, map, center);
            break;
        }
    }
    return sprites;
}

Map s_count(const Sprites sprites, Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        map.rooms.agents[i] = 0;
        for(int s = 0; s < sprites.count; s++)
        {
            Sprite* const sprite = &sprites.sprite[s];

            if(s_no_count(sprite))
                continue;

            if(p_eql(sprite->where, map.rooms.wheres[i], map.grid))
                map.rooms.agents[i]++;
        }
    }
    return map;
}
