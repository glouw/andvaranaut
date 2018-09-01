#include "Sprites.h"

#include "Field.h"
#include "Title.h"
#include "Inventory.h"
#include "util.h"

#include <ctype.h>
#include <math.h>

Sprites xzsprites(void)
{
    static Sprites sprites;
    return sprites;
}

Sprites xsnew(const int max)
{
    const Sprites sprites = { xtoss(Sprite, max), 0, max, NOATTACK };
    return sprites;
}

static Sprites append(Sprites sprites, const Sprite sprite)
{
    // Resize if no size.
    if(sprites.max == 0)
        xretoss(sprites.sprite, Sprite, sprites.max = 1);

    // Resize if capped.
    if(sprites.count >= sprites.max)
        xretoss(sprites.sprite, Sprite, sprites.max *= 2);

    // Append.
    sprites.sprite[sprites.count++] = sprite;

    return sprites;
}

Sprites xlay(Sprites sprites, const Map map, const Overview ov)
{
    if(xmout(map, ov.where))
        return sprites;

    const int ascii = ov.selected + ' ';

    if(xsissprite(ascii))
        sprites = append(sprites, xsregistrar(ascii, ov.where));

    return sprites;
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xsub(sprite->where, hero.where);
    }
}

static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xadd(sprite->where, hero.where);
    }
}

static int comparator(const void* a, const void* b)
{
    const Point pa = *(const Point*) a;
    const Point pb = *(const Point*) b;
    return xmag(pa) < xmag(pb) ? 1 : xmag(pa) > xmag(pb) ? -1 : 0;
}

static void sort(const Sprites sprites)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), comparator);
}

static void turn(const Sprites sprites, const float theta)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xtrn(sprite->where, theta);
    }
}

void xorient(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    turn(sprites, -hero.theta);
}

void xplback(const Sprites sprites, const Hero hero)
{
    turn(sprites, +hero.theta);
    push(sprites, hero);
}

static void arrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites);
    push(sprites, hero);
}

static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        // Stuck in a wall.
        if(xtile(sprite->where, map.walling))
        {
            xsplace(sprite, xmid(sprite->last));
            sprite->velocity = xzpoint();
        }

        // Stuck in water.
        if(xblok(sprite->where, map.floring) == ' ')
        {
            xsplace(sprite, xmid(sprite->last));
            sprite->velocity = xzpoint();
        }
    }
}

static void move(const Sprites sprites, const Field field, const Point to, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xisstuck(sprite))
            continue;
        const Point dir = xforce(field, sprite->where, to, map);

        // No force of direction...
        if(dir.x == 0.0f && dir.y == 0.0f)
        {
            // Some sprites do not move.
            if(sprite->speed == 0.0f)
                continue;

            // Slow down.
            sprite->velocity = xmul(sprite->velocity, 1.0f - sprite->acceleration / sprite->speed);
        }
        // Speed up.
        else
        {
            const Point acc = xmul(dir, sprite->acceleration);
            sprite->velocity = xadd(sprite->velocity, acc);

            // Check top speed.
            if(xmag(sprite->velocity) > sprite->speed)
                // And cap speed if the top speed is surpassed.
                sprite->velocity = xmul(xunt(sprite->velocity), sprite->speed);
        }
        sprite->state = xmag(sprite->velocity) > 0.005f ? CHASING : IDLE;
        xsplace(sprite, xadd(sprite->where, sprite->velocity));
    }
}

static void scentwall(const Field field, const Point where, const Map map, const float scent)
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

        if(xon(field, j, i))
        {
            // Walls.
            field.mesh[j][i] = map.walling[y][x] == ' ' ? 0.0f : -scent;

            // Water.
            if(map.floring[y][x] == ' ')
                field.mesh[j][i] = -scent;
        }
    }
}

static void scentsprite(const Field field, const Sprites sprites, const float scent)
{
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];

        if(xiscosmetic(sprite->ascii))
            continue;

        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        for(int a = -field.res / 2; a <= field.res / 2; a++)
        for(int b = -field.res / 2; b <= field.res / 2; b++)
            if(xon(field, j + a, i + b))
                field.mesh[j + a][i + b] -= scent;
    }
}

static void scenthero(const Field field, const Point where, const float scent)
{
    const int j = field.res * where.y;
    const int i = field.res * where.x;
    field.mesh[j][i] = scent;
}

static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    const float scent = 1000.0f;
    scentwall(field, hero.where, map, scent);
    scentsprite(field, sprites, scent);
    scenthero(field, hero.where, 100.0f * scent);
    xdiffuse(field, hero.where);
}

static Sprites dropit(Sprites sprites, const Attack attack, const Point where)
{
    const Point delta = xmul(xrag(attack.dir), 0.5f);
    return append(sprites, xsregistrar('d', xadd(where, delta)));
}

static void brokelb(const int ascii, const Inventory inv, const Timer tm)
{
    if(ascii == 'd')
        if(!xitsadd(inv.items, xitrand()))
            xttset(tm.renders, tm.renders + 120, false, "Inventory Full");
}

static Sprites hurt(Sprites sprites, Sprite* const sprite, const Attack attack, const Inventory inv, const Timer tm)
{
    const int side = fabsf(attack.dir.x) > fabsf(attack.dir.y);

    sprite->health -= attack.power;

    if(sprite->health <= 0.0f)
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? DEADW : DEADE):
            (attack.dir.y > 0.0f ? DEADN : DEADS);

        brokelb(sprite->ascii, inv, tm);

        // Lootbags count as sprites, so killing a lootbag may drop another lootbag.
        if(xd10() == 0)
            return dropit(sprites, attack, sprite->where);
    }
    else
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? HURTW : HURTE):
            (attack.dir.y > 0.0f ? HURTN : HURTS);

        sprite->ticks = tm.ticks + 5;
    }
    return sprites;
}

static Sprites hmelee(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm, const Hero hero)
{
    const Point hand = xtouch(hero);
    const Item it = inv.items.item[inv.selected];

    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(xisuseless(sprite))
            continue;

        if(xeql(hand, sprite->where, 2.0f))
        {
            sprites = hurt(sprites, sprite, attack, inv, tm);
            if(++hurts == it.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites hrange(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm)
{
    const SDL_Point point = {
        (int) attack.reticule.x,
        (int) attack.reticule.y,
    };
    const Item it = inv.items.item[inv.selected];

    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(xisuseless(sprite))
            continue;

        const int hit = SDL_PointInRect(&point, &sprite->seen);

        if(hit)
        {
            sprites = hurt(sprites, sprite, attack, inv, tm);
            if(++hurts == it.hurts)
                return sprites;
        }
    }
    return sprites;
}

static Sprites hmagic(Sprites sprites, const Attack attack, const Inventory inv, const Timer tm, const Hero hero)
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

// Hurts the closest sprite(s) based on weapon "hurt" value.
// Eg. A weapon with a hurt value of 3 will hurt 3 sprites at most with a single attack.
Sprites xhurt(Sprites sprites, const Attack attack, const Hero hero, const Input in, const Inventory inv, const Timer tm)
{
    if(in.lu)
    {
        // The last attack method is saved within the sprites container for other objects to know
        // how the sprites were last impacted.
        // For example, the hero yaw may get a recoil bump when a bow is shot.
        sprites.last = attack.method;

        // Do the attack.
        if(attack.method == MELEE)
            return hmelee(sprites, attack, inv, tm, hero);

        if(attack.method == RANGE)
            return hrange(sprites, attack, inv, tm);

        if(attack.method == MAGIC)
            return hmagic(sprites, attack, inv, tm, hero);
    }

    // Of course, if no attack happened, then the sprites were not harmed.
    sprites.last = NOATTACK;

    return sprites;
}

static void idle(const Sprites sprites, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(xisdead(sprite->state))
            continue;

        if(tm.ticks > sprite->ticks)
            sprite->state = IDLE;
    }
}

static Hero damage(Hero hero, const Sprites sprites, const Timer tm)
{
    const float wave = 0.5f * (sinf(FPI * tm.ticks / 60.0f) + 1.0f);

    hero.hps = hero.hpsmax * wave;
    hero.mna = hero.mnamax * wave;
    hero.ftg = hero.ftgmax * wave;

    // TODO:
    // Water and food sprites replenish the hero's fatigue.
    // Healing wizard sprites heal the hero instead of damaging the hero.
    (void) sprites;

    return hero;
}

Hero xcaretake(const Sprites sprites, const Hero hero, const Map map, const Field field, const Timer tm)
{
    arrange(sprites, hero);

    idle(sprites, tm);

    route(sprites, field, map, hero);

    move(sprites, field, hero.where, map);

    bound(sprites, map);

    return damage(hero, sprites, tm);
}

static Point freerand(const Point mid, const Map m)
{
    const Point where = xrand(mid, m.grid / 1.5);
    const int x = where.x;
    const int y = where.y;

    // No wall. Must have floor.
    return m.walling[y][x] == ' '
        && m.floring[y][x] != ' ' ? where : freerand(mid, m);
}

// Populates a room with a nice garden of sprites.
static Sprites pngarden(Sprites sprites, const Map m, const Point mid)
{
    const int flowers = 128;
    for(int i = 0; i < flowers; i++)
    {
        const Point where = freerand(mid, m);
        sprites = append(sprites, xsregistrar('a', where));
    }

    // Caretaker.
    const Point where = freerand(mid, m);
    sprites = append(sprites, xsregistrar('b', where));

    return sprites;
}

Sprites xspopulate(Sprites sprites, const Map m)
{
    for(int i = 0; i < m.rooms.count; i++)
    {
        const Point mid = m.rooms.wheres[i];

        switch(m.rooms.themes[i])
        {
        case NICE_GARDEN:
            sprites = pngarden(sprites, m, mid);
            break;

        // TODO: TEMP. Just puts a guy in a room for now so that each room has some sort of placeholder.
        default:
            sprites = append(sprites, xsregistrar('b', freerand(mid, m)));
            break;
        }
    }
    return sprites;
}

Map xscount(const Sprites sprites, Map m)
{
    for(int i = 0; i < m.rooms.count; i++)
    {
        m.rooms.agents[i] = 0;
        for(int s = 0; s < sprites.count; s++)
        {
            Sprite* const sprite = &sprites.sprite[s];

            if(xiscosmetic(sprite->ascii))
                continue;

            if(xisinanimate(sprite->ascii))
                continue;

            if(xisdead(sprite->state))
                continue;

            if(xeql(sprite->where, m.rooms.wheres[i], m.grid))
                m.rooms.agents[i]++;
        }
    }
    return m;
}
