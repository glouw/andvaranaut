#include "Sprites.h"

#include "Field.h"
#include "Found.h"
#include "util.h"

#include <SDL2/SDL.h>
#include <ctype.h>
#include <math.h>

// TODO: Arrays are fine as is. Make a STATE for nulled (where the sprites go red).
// On a map reload, these sprites will be removed.

static Sprite born(const Point where)
{
    Sprite sprite;
    xzero(sprite);
    sprite.where = where;
    sprite.last = where;
    sprite.state = IDLE;
    sprite.scent = 0.5;
    sprite.health = 1000.0;
    return sprite;
}

// Flower.
static Sprite _a_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'a';
    sprite.scent = 0.0;
    sprite.width = 0.60;
    return sprite;
}

// Outlaw.
static Sprite _b_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'b';
    sprite.speed = 0.033;
    sprite.acceleration = 0.0025;
    sprite.width = 0.66;
    return sprite;
}

// Loot bag.
static Sprite _d_(const Point where)
{
    Sprite sprite = born(where);
    sprite.ascii = 'd';
    sprite.width = 1.00;
    sprite.health = 1.0; // Breaks very easy.
    return sprite;
}

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        default:
        case 'a': return _a_(where);
        case 'b': return _b_(where);
        case 'd': return _d_(where);
    }
}

Sprites xsgen()
{
    Sprites sprites;
    sprites.count = 0;
    sprites.max = 32;
    sprites.sprite = xtoss(Sprite, sprites.max);
    return sprites;
}

void xkill(const Sprites sprites)
{
    free(sprites.sprite);
}

// Appends a new sprite to the sprite list. Resizes sprite list of need be.
static Sprites append(Sprites sprites, const Sprite sprite)
{
    if(sprites.count >= sprites.max)
        xretoss(sprites.sprite, Sprite, sprites.max *= 2);
    sprites.sprite[sprites.count++] = sprite;
    return sprites;
}

Sprites xlay(Sprites sprites, const Map map, const Overview ov)
{
    // Out of bounds check.
    if(xout(map, ov.where)) return sprites;
    // Ascii sprite check.
    const int ascii = ov.selected + ' ';
    if(xissprite(ascii))
    {
        // If the sprite list is empty, resize to one big.
        if(sprites.count == 0)
            xretoss(sprites.sprite, Sprite, sprites.max = 1);
        // If the new sprite cannot fit in the sprite list, resize twice as big.
        sprites = append(sprites, registrar(ascii, ov.where));
    }
    return sprites;
}

// Subtracts the hero's position from all sprite positions
static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xsub(sprite->where, hero.where);
    }
}

// Adds the hero's position to all sprite positions.
static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xadd(sprite->where, hero.where);
    }
}

// Quick sort comparator for sorting sprites furthest to nearest.
static int backwards(const void* a, const void* b)
{
    const Point pa = *(const Point*) a;
    const Point pb = *(const Point*) b;
    return xmag(pa) < xmag(pb) ? +1 : xmag(pa) > xmag(pb) ? -1 : 0;
}

// Quick sort comparator for sorting sprites nearest to furthest.
static int forewards(const void* a, const void* b)
{
    const Point pa = *(const Point*) a;
    const Point pb = *(const Point*) b;
    return xmag(pa) < xmag(pb) ? -1 : xmag(pa) > xmag(pb) ? +1 : 0;
}

// Quick sort.
typedef int (*const Direction)(const void*, const void*);

static void sort(const Sprites sprites, const Direction direction)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), direction);
}

// Produces a new copy of all sprites on the heap. Don't forget to free it.
static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = xtoss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

// Rotates all sprites around (0,0) by some theta.
static void turn(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = xtrn(sprite->where, -hero.theta);
    }
}

Sprites xorient(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    pull(copied, hero);
    turn(copied, hero);
    sort(copied, backwards);
    // The sprites are not pushed back as the sprite renderer
    // pretends the hero is always centered at (0,0).
    return copied;
}

// Sorts sprites closest to furthest. Useful for knowing which
// sprite is closest (eg, for talking, attacking, grabbing).
static void arrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, forewards);
    push(sprites, hero);
}

// Moves a single sprite. Updates the sprites last position.
// Must always be used for moving sprites.
static void place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

// Grabs the closest sprite when <hands> are equipped.
static void grab(const Sprites sprites, const Hero hero, const Input input)
{
    if(!input.l)
        return;
    if(hero.wep != HANDS)
        return;
    const Point hand = xtouch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xisdead(sprite->state))
            continue;
        if(xeql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            xzero(sprite->velocity);
            place(sprite, hand);
            // Only one sprite is grabbed at a time.
            return;
        }
    }
}

// Shoves the closest sprite away when holding a grabbed sprite.
static void shove(const Sprites sprites)
{
    // Find the grabbed sprite.
    Sprite* const grabbed = find(xgrabbed, sprites);
    if(!grabbed) return;
    // Use the grabbed sprite to shove other sprites.
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const other = &sprites.sprite[i];
        // Ensure the other sprite is not the sprite in hand.
        if(other == grabbed) continue;
        // Shove using the largest width of either the grabbed sprite or the other sprite.
        const float width = xmax(other->width, grabbed->width);
        if(xeql(other->where, grabbed->where, width))
        {
            const Point delta = xsub(other->where, grabbed->where);
            place(other, xadd(other->where, delta));
        }
    }
}

// Puts a sprite in the last good spot if the sprite goes into a wall.
// TODO: The sprite is stunned and put into a dizzy state if the sprite went into a wall.
// TODO: The sprite is stunned and put into a scared state if the sprite went into the water.
static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Stuck in a wall.
        if(xtile(sprite->where, map.walling))
        {
            place(sprite, xmid(sprite->last));
            xzero(sprite->velocity);
            // TODO: add a dizzy animation.
        }
        // Stuck in water.
        if(xblok(sprite->where, map.floring) == ' ')
        {
            place(sprite, xmid(sprite->last));
            xzero(sprite->velocity);
            // TODO: add a scared animation animation.
        }
    }
}

// Moves sprite along force field towards the hero player scent.
static void move(const Sprites sprites, const Field field, const Point to)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xishurt(sprite->state))
            continue;
        if(xisdead(sprite->state))
            continue;
        // Do not move the sprite if the sprite is immovable.
        if(sprite->speed == 0.0)
        {
            sprite->state = IDLE;
            continue;
        }
        const Point dir = xforce(field, sprite->where, to);
        // No force of direction...
        if(dir.x == 0.0 && dir.y == 0.0)
            // Then slow down
            sprite->velocity = xmul(sprite->velocity, 1.0 - sprite->acceleration / sprite->speed);
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
        // If the sprite is fast enough they will animate chase.
        sprite->state = xmag(sprite->velocity) > 0.005 ? CHASING : IDLE;
        // Place the sprite at their new location...
        place(sprite, xadd(sprite->where, sprite->velocity));
    }
}

// Collaborative diffusion with various scents.
static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    const float scent = 1e3;
    // Walls and pit water repel sprites.
    for(int j = 0; j < field.rows; j++)
    for(int i = 0; i < field.cols; i++)
    {
        const int y = j / field.res;
        const int x = i / field.res;
        // Walls.
        if(map.walling[y][x] != ' ') field.mesh[j][i] = -scent;
        // Water.
        if(map.floring[y][x] == ' ') field.mesh[j][i] = -scent;
    }
    // Sprite scents stack on one another.
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        for(int a = -field.res / 2; a <= field.res / 2; a++)
        for(int b = -field.res / 2; b <= field.res / 2; b++)
            // Out of bounds check.
            if(xon(field, j + a, i + b))
                field.mesh[j + a][i + b] -= sprite->scent;
    }
    // Hero scent attracts but a much larger magnitude than the walls and water.
    const int j = field.res * hero.where.y;
    const int i = field.res * hero.where.x;
    field.mesh[j][i] = 1000.0 * scent;
    // Diffuse the culminated scent across the field.
    xdiffuse(field, hero.where);
}

int xissprite(const int ascii)
{
    return isalpha(ascii);
}

static Sprites drop(Sprites sprites, const Attack attack, const Point where)
{
    const Point delta = xmul(xrag(attack.dir), 0.5);
    return append(sprites, registrar('d', xadd(where, delta)));
}

static int iscosmetic(const Sprite* sprite)
{
    return sprite->ascii == 'a';
}

// Hurts the closest sprite, or sprites, depending on the weapon.
static Sprites hurt(Sprites sprites, const Attack attack, const Hero hero, const Input input, const int ticks)
{
    if(!input.lu)
        return sprites;
    if(hero.wep == HANDS)
        return sprites;
    const Point hand = xtouch(hero, hero.arm);
    const int side = fabs(attack.dir.x) > fabs(attack.dir.y);
    for(int i = 0, hurts = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Cannot hurt dead sprites.
        if(xisdead(sprite->state))
            continue;
        // Cannot hurt cosmetic sprites (flowers and such)
        if(iscosmetic(sprite))
            continue;
        // Contact was made.
        if(xeql(hand, sprite->where, 2.0))
        {
            // Hurt direction.
            sprite->state =
                side ?
                (attack.dir.x > 0.0 ? HURTW : HURTE):
                (attack.dir.y > 0.0 ? HURTN : HURTS);
            sprite->health -= attack.power;
            // Sprite dead?
            if(sprite->health <= 0.0)
            {
                // Dead direction.
                sprite->state =
                    side ?
                    (attack.dir.x > 0.0 ? DEADW : DEADE):
                    (attack.dir.y > 0.0 ? DEADN : DEADS);
                // If a sprite is dead, the hurt counter resets.
                // That is to say, if a greatsword can hurt 3 enemies at once, and one sprite
                // dies, three more may be hurt with the same swing.
                sprites = hurt(sprites, attack, hero, input, ticks);
                // 10% chance something will drop.
                // This means that anything can drop a loot bag when it dies, even
                // a loot bag will have a chance of dropping a loot bag when it "dies".
                return rand() % 10 == 0 ? drop(sprites, attack, sprite->where) : sprites;
            }
            // Timer for idle reset.
            sprite->ticks = ticks + 5;
            // Depending on the held weapon, only a certain number of enemies
            // can be hurt. For instance, a dagger can at most hurt one enemy.
            // A greatsword or warhammer can hurt more than one enemy.
            if(++hurts == 3) return sprites;
        }
    }
    return sprites;
}

// For timer expirey.
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

Sprites xcaretake(Sprites sprites, const Hero hero, const Input input, const Map map, const Attack attack, const int ticks)
{
    // Sprites need to be arrange closest to hero first.
    arrange(sprites, hero);
    idle(sprites, ticks);
    // Sprite path finding and movement.
    const Field field = xprepare(map, hero.aura);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where);
    xruin(field);
    sprites = hurt(sprites, attack, hero, input, ticks);
    // Sprite placement - interactive and out of bounds.
    grab(sprites, hero, input);
    shove(sprites);
    bound(sprites, map);
    return sprites;
}
