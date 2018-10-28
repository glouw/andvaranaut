#include "Sprites.h"

#include "Field.h"
#include "Title.h"
#include "Speech.h"
#include "Inventory.h"
#include "util.h"

#include <ctype.h>
#include <math.h>

Sprites xsnew(const int max)
{
    const Sprites sprites = { xtoss(Sprite, max), 0, max, NO_ATTACK };
    return sprites;
}

static Sprites append(Sprites sprites, const Sprite sprite)
{
    if(sprites.max == 0)
        xretoss(sprites.sprite, Sprite, sprites.max = 1);

    if(sprites.count >= sprites.max)
        xretoss(sprites.sprite, Sprite, sprites.max *= 2);

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

static int comparator(const void* const a, const void* const b)
{
    const Sprite* const sa = (const Sprite*) a;
    const Sprite* const sb = (const Sprite*) b;
    return
        xmag(sa->where) < xmag(sb->where) ? +1 :
        xmag(sa->where) > xmag(sb->where) ? -1 : 0;
}

static void sort(const Sprites sprites)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), comparator);
}

static void turn(const Sprites sprites, const float yaw)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xtrn(sprite->where, yaw);
    }
}

void xorient(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    turn(sprites, -hero.yaw);
}

void xplback(const Sprites sprites, const Hero hero)
{
    turn(sprites, +hero.yaw);
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

        // No force applied - slow down.
        if(dir.x == 0.0f && dir.y == 0.0f)
        {
            if(sprite->speed == 0.0f)
                continue;

            sprite->velocity = xmul(sprite->velocity, 1.0f - sprite->acceleration / sprite->speed);
        }
        // Force applied - speed up.
        else
        {
            const Point acc = xmul(dir, sprite->acceleration);
            sprite->velocity = xadd(sprite->velocity, acc);

            if(xmag(sprite->velocity) > sprite->speed)
                sprite->velocity = xmul(xunt(sprite->velocity), sprite->speed);
        }

        // Sprite will stop running if close to player.
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
            // Scent walls.
            field.mesh[j][i] = map.walling[y][x] == ' ' ? 0.0f : -scent;

            // Scent water.
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
    {
        const Item item = xitrand();
        const int fit = xitsadd(inv.items, item);
        if(fit)
            xttset(tm.renders, tm.renders + 120, false, "Picked up %s", item.cstr);
        else
            xttset(tm.renders, tm.renders + 120, false, "Inventory Full");
    }
}

static Sprites hurt(Sprites sprites, Sprite* const sprite, const Attack attack, const Inventory inv, const Timer tm)
{
    const int side = fabsf(attack.dir.x) > fabsf(attack.dir.y);

    sprite->health -= attack.power;

    // Dead.
    if(sprite->health <= 0.0f)
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? DEAD_W : DEAD_E):
            (attack.dir.y > 0.0f ? DEAD_N : DEAD_S);

        brokelb(sprite->ascii, inv, tm);

        // Lootbags count as sprites, so killing a lootbag may drop another lootbag.
        if(xd10() == 0)
            return dropit(sprites, attack, sprite->where);
    }
    else // Just hurt, not killed.
    {
        sprite->state = side ?
            (attack.dir.x > 0.0f ? HURT_W : HURT_E):
            (attack.dir.y > 0.0f ? HURT_N : HURT_S);

        xstun(sprite, tm);

        // Hurt a good sprite, make 'em angry.
        if(!sprite->evil)
        {
            sprite->evil = true;
            sprite->speech = xspzero();
        }
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

Sprites xhurt(Sprites sprites, const Attack attack, const Hero hero, const Input in, const Inventory inv, const Timer tm)
{
    if(in.lu)
    {
        sprites.last = attack.method;

        if(attack.method == MELEE)
            return hmelee(sprites, attack, inv, tm, hero);

        if(attack.method == RANGE)
            return hrange(sprites, attack, inv, tm);

        if(attack.method == MAGIC)
            return hmagic(sprites, attack, inv, tm, hero);
    }
    sprites.last = NO_ATTACK;

    return sprites;
}

static void idle(const Sprites sprites, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(xisdead(sprite->state))
            continue;

        if(!xisstun(sprite, tm))
            sprite->state = IDLE;
    }
}

// Damage hero health points.
static Hero dhps(Hero hero, const Sprites sprites, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        if(xisuseless(sprite))
            continue;

        // TODO: Maybe sprites have different attack ranges.
        if(sprite->evil && sprite->state == IDLE && xeql(hero.where, sprite->where, 2.2f))
        {
            sprite->state = ATTACK_N;

            if(xisimpulse(sprite, tm))
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

// Damage hero mana (enemy sprites can sap mana, etc...)
static Hero dmna(Hero hero, const Sprites sprites, const Timer tm)
{
    (void) sprites;
    (void) tm;
    return hero;
}

// Damage hero fatigue (enemy sprites can sap fatigue, etc...).
static Hero dftg(Hero hero, const Sprites sprites, const Timer tm)
{
    (void) sprites;
    (void) tm;
    return hero;
}

static void block(const Sprites sprites, const Hero hero, const Timer tm)
{
}

static Hero damage(Hero hero, const Sprites sprites, const Timer tm)
{
    hero = dhps(hero, sprites, tm);
    hero = dmna(hero, sprites, tm);
    hero = dftg(hero, sprites, tm);
    return hero;
}

static void speak(const Sprites sprites, const Hero hero, const Timer tm)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];

        // Mute check very important else segfault.
        if(xisdead(sprite->state) || xismute(sprite))
            continue;

        if(xeql(hero.where, sprite->where, 2.5f))
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

Hero xcaretake(const Sprites sprites, const Hero hero, const Map map, const Field field, const Timer tm)
{
    arrange(sprites, hero);
    idle(sprites, tm);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where, map);
    bound(sprites, map);
    //speak(sprites, hero, tm);
    block(sprites, hero, tm);
    //return damage(hero, sprites, tm);
    return hero;
}

static Point avail(const Point center, const Map map)
{
    const Point where = xrand(center, map.grid);

    // Available tile space is no wall and not above water.
    return xblok(where, map.walling) == ' '
        && xblok(where, map.floring) != ' ' ? where : avail(center, map);
}

static Point seek(const Point center, const Map map, const int ascii)
{
    const Point where = avail(center, map);
    return xblok(where, map.floring) == ascii ? where : seek(center, map, ascii);
}

static Sprites pngarden(Sprites sprites, const Map map, const Point center)
{
    const int flowers = 256;
    for(int i = 0; i < flowers; i++)
        sprites = append(sprites, xsregistrar('a', seek(center, map, '(')));

    // Gardener.
    sprites = append(sprites, xsregistrar('b', avail(center, map)));
    return sprites;
}

static Sprites dummy(const Sprites sprites, const Map map, const Point center)
{
    return append(sprites, xsregistrar('b', avail(center, map)));
}

Sprites xspopulate(Sprites sprites, const Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        const Point center = map.rooms.wheres[i];

        switch(map.rooms.themes[i])
        {
        case NICE_GARDEN:
            sprites = pngarden(sprites, map, center);
            break;

        // TODO: TEMP. Just puts a guy in a room for now so that each room has some sort of placeholder.
        default:
            sprites = dummy(sprites, map, center);
            break;
        }
    }
    return sprites;
}

Map xscount(const Sprites sprites, Map map)
{
    for(int i = 0; i < map.rooms.count; i++)
    {
        map.rooms.agents[i] = 0;
        for(int s = 0; s < sprites.count; s++)
        {
            Sprite* const sprite = &sprites.sprite[s];

            if(xnocount(sprite))
                continue;

            if(xeql(sprite->where, map.rooms.wheres[i], map.grid))
                map.rooms.agents[i]++;
        }
    }
    return map;
}
