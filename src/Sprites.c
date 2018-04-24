#include "Sprites.h"

#include "Field.h"
#include "Direction.h"
#include "Inventory.h"
#include "util.h"

#include <ctype.h>
#include <math.h>

Sprites xzsprites()
{
    static Sprites sprites;
    return sprites;
}

Sprites xsnew(const int max)
{
    const Sprites sprites = { xtoss(Sprite, max), 0, max };
    return sprites;
}

void xkill(const Sprites sprites)
{
    free(sprites.sprite);
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

static void sort(const Sprites sprites, const Direction direction)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), direction);
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
    sort(sprites, comparator);
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
            // TODO: Add a dizzy animation.
        }
        // Stuck in water.
        if(xblok(sprite->where, map.floring) == ' ')
        {
            xsplace(sprite, xmid(sprite->last));
            sprite->velocity = xzpoint();
            // TODO: Add a scared animation animation.
        }
    }
}

static void move(const Sprites sprites, const Field field, const Point to, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xishurt(sprite->state))
            continue;
        if(xisdead(sprite->state))
            continue;
        const Point dir = xforce(field, sprite->where, to, map);
        // No force of direction...
        if(dir.x == 0.0f && dir.y == 0.0f)
        {
            // Some sprites do not move.
            if(sprite->speed == 0.0f)
                continue;
            // Then slow down.
            sprite->velocity = xmul(sprite->velocity, 1.0f - sprite->acceleration / sprite->speed);
        }
        // Otherwise speed up.
        else
        {
            const Point acc = xmul(dir, sprite->acceleration);
            sprite->velocity = xadd(sprite->velocity, acc);
            // And then check top speed...
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
        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        for(int a = -field.res / 2; a <= field.res / 2; a++)
        for(int b = -field.res / 2; b <= field.res / 2; b++)
            if(xon(field, j + a, i + b))
                // Sprite scents stack.
                field.mesh[j + a][i + b] -= scent;
    }
}

static void scenthero(const Field field, const Point where, const float scent)
{
    const int j = field.res * where.y;
    const int i = field.res * where.x;
    field.mesh[j][i] = scent;
}

// Collaborative diffusion with various object scents.
static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    const float scent = 1000.0f;
    scentwall(field, hero.where, map, scent);
    scentsprite(field, sprites, scent);
    scenthero(field, hero.where, 100.0f * scent); // Why this much more scent?
    xdiffuse(field, hero.where);
}

static Sprites drop(Sprites sprites, const Attack attack, const Point where)
{
    const Point delta = xmul(xrag(attack.dir), 0.5f);
    return append(sprites, xsregistrar('d', xadd(where, delta)));
}

static Sprites hmelee(Sprites sprites, const Attack attack, const Hero hero, const Inventory inv, const int ticks)
{
    const Point hand = xtouch(hero, hero.arm);
    const int side = fabsf(attack.dir.x) > fabsf(attack.dir.y);
    const Item it = inv.items.item[inv.selected];
    // Hurt counter indicates how many sprites can be hurt at once. Different weapons have different hurt counters.
    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xisdead(sprite->state))
            continue;
        if(xsiscosmetic(sprite))
            continue;
        // Sprite hurt?
        if(xeql(hand, sprite->where, 2.0f))
        {
            // Hurt direction.
            sprite->state =
                side ?
                (attack.dir.x > 0.0f ? HURTW : HURTE):
                (attack.dir.y > 0.0f ? HURTN : HURTS);
            sprite->health -= attack.power;
            // Sprite dead?
            if(sprite->health <= 0.0f)
            {
                // Dead direction.
                sprite->state =
                    side ?
                    (attack.dir.x > 0.0f ? DEADW : DEADE):
                    (attack.dir.y > 0.0f ? DEADN : DEADS);
                // Broke a lootbag?
                if(sprite->ascii == 'd')
                    // Add an item to the inventory.
                    if(!xitsadd(inv.items, xitrand()))
                        // Make this a log message in the future.
                        printf("Inventory full!\n");
                // If a sprite is dead, the hurt counter resets, so this function is called again.
                sprites = hmelee(sprites, attack, hero, inv, ticks);
                // Chance sprite will drop loot bag.
                return xd10() == 0 ? drop(sprites, attack, sprite->where) : sprites;
            }
            // Timer for idle reset.
            sprite->ticks = ticks + 5;
            if(++hurts == 3)
                return sprites;
        }
    }
    return sprites;
}

static Sprites hrange(Sprites sprites, const Attack attack, const Hero hero, const Inventory inv, const int ticks)
{
    // Remember that attack direction was overrided with a point.
    //const SDL_Point point = {
    //    (int) attack.dir.x,
    //    (int) attack.dir.y,
    //};
    const Item it = inv.items.item[inv.selected];
    // Go through all sprites.
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // If the attack point is within a sprite hitbox then the sprite is hurt.
        //const int inside = SDL_PointInRect(&point, &sprite->seen);
        //if(inside)
        //{
        //    sprite->state = HURTN;
        //    sprite->ticks = ticks + 5;
        //}

    }
    return sprites;
}

static Sprites hmagic(Sprites sprites, const Attack attack, const Hero hero, const Inventory inv, const int ticks)
{
    const Item it = inv.items.item[inv.selected];
    return sprites;
}

// Hurts the closest sprite.
Sprites xhurt(Sprites sprites, const Attack attack, const Hero hero, const Input in, const Inventory inv, const int ticks)
{
    if(!in.lu)
        return sprites;
    switch(attack.method)
    {
    case MELEE: return hmelee(sprites, attack, hero, inv, ticks);
    case RANGE: return hrange(sprites, attack, hero, inv, ticks);
    case MAGIC: return hmagic(sprites, attack, hero, inv, ticks);
    // For NOATTACK
    default:
        return sprites;
    }
}

static void idle(const Sprites sprites, const int ticks)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xisdead(sprite->state))
            continue;
        if(ticks > sprite->ticks)
            sprite->state = IDLE;
    }
}

static Hero damage(Hero hero, const Sprites sprites, const int ticks)
{
    const float wave = 0.5f * (sinf(3.1416f * ticks / 60.0f) + 1.0f);
    hero.hps = hero.hpsmax * wave;
    hero.mna = hero.mnamax * wave;
    hero.ftg = hero.ftgmax * wave;
    return hero;
}

Hero xcaretake(const Sprites sprites, const Hero hero, const Map map, const Field field, const int ticks)
{
    arrange(sprites, hero);
    idle(sprites, ticks);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where, map);
    bound(sprites, map);
    // The hero must be damaged here since the sprites
    // header cannot be included in the hero header.
    return damage(hero, sprites, ticks);
}
