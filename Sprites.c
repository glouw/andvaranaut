#include "Sprites.h"

#include "Point.h"
#include "Hero.h"
#include "util.h"
#include "lib/SDL2/SDL.h"

#include <ctype.h>
#include <math.h>

static Sprite generic(const Point where)
{
    Sprite sprite;
    zero(sprite);
    sprite.where = where;
    sprite.ascii = 'o';
    sprite.state = IDLE;
    sprite.width = 0.66;
    sprite.health = 50.0;
    return sprite;
}

static Sprite _o_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'o';
    sprite.width = 0.66;
    sprite.moveable = true;
    return sprite;
}

static Sprite _t_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 't';
    sprite.width = 0.66;
    return sprite;
}

static Sprite _l_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'l';
    sprite.width = 1.00;
    return sprite;
}

// For testing "Missing" animation
static Sprite _Z_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'Z';
    sprite.width = 0.66;
    sprite.moveable = true;
    return sprite;
}

// Sprite factory
static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o_(where);
        case 't': return _t_(where);
        case 'l': return _l_(where);
        // Tests the "Missing" animation for now
        case 'Z': return _Z_(where);
    }
    return generic(where);
}

Sprites wake(const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = concat("sprites/lvl", which);
    FILE* const file = fopen(path, "r");
    Sprites sprites;
    sprites.count = lns(file);
    sprites.sprite = toss(Sprite, sprites.count);
    sprites.max = sprites.count;
    for(int i = 0; i < sprites.count; i++)
    {
        char* const line = readln(file);
        char ascii = 0;
        Point where = { 0.0, 0.0 };
        sscanf(line, "%c,%f,%f\n", &ascii, &where.x, &where.y);
        sprites.sprite[i] = registrar(ascii, where);
        free(line);
    }
    fclose(file);
    free(path);
    return sprites;
}

void kill(const Sprites sprites)
{
    free(sprites.sprite);
}

Sprites rewake(const Sprites sprites, const int level)
{
    kill(sprites);
    return wake(level);
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = sub(sprite->where, hero.where);
    }
}

static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = add(sprite->where, hero.where);
    }
}

static int backwards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return mag(pa) < mag(pb) ? +1 : mag(pa) > mag(pb) ? -1 : 0;
}

static int forewards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return mag(pa) < mag(pb) ? -1 : mag(pa) > mag(pb) ? +1 : 0;
}

static void sort(const Sprites sprites, const int foreward)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), foreward ? forewards : backwards);
}

static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = toss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

static void turn(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->where = trn(sprite->where, -hero.theta);
    }
}

Sprites arrange(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    pull(copied, hero);
    turn(copied, hero);
    sort(copied, false);
    return copied;
}

static void rearrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, true);
    push(sprites, hero);
}

// Sprites will only ever rest after their ticks have
// exceeded the universal tick counter
static void rest(const Sprites sprites, const int ticks)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->ticks < ticks)
            sprite->state = IDLE;
    }
}

static void surrender(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->health < 10.0)
            sprite->state = MERCY;
    }
}

bool issprite(const int ascii)
{
    return isalpha(ascii);
}

static Compass needle(const Point vect)
{
    const float angle = atan2f(vect.y, vect.x);
    return (Compass) ((int) roundf(DIRS * angle / (2.0 * pi) + DIRS) % DIRS);
}

// Hurts all sprites within Area of Effect (AOE)
static void hurt(const Sprites sprites, const Hero hero, const int ticks)
{
    // Note: all attack types must go here
    if(!hero.attack.type.swing)
        return;
    const Compass dir = needle(hero.attack.vect);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        const float aoe = max(hero.attack.area, sprite->width);
        if(eql(hero.attack.where, sprite->where, aoe))
        {
            sprite->ticks = ticks + 3;
            sprite->health -= hero.attack.power;
            sprite->state = (State) dir;
        }
    }
}

// Sprites moved with this function have their last location
// logged before they are moved. This is useful for when sprites are shoved
// or grabbed out of map bounds
static void place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

// Grabs the closest sprite when using hands
static void grab(const Sprites sprites, const Hero hero, const Input input, const int ticks)
{
    if(hero.weapon != HANDS)
        return;
    if(!(input.key[SDL_SCANCODE_J] || input.l))
        return;
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(!sprite->moveable)
            continue;
        if(eql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            // No time
            sprite->ticks = ticks;
            place(sprite, hand);
            return;
        }
    }
}

static Sprite* find(const Sprites sprites, const State state)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->state == state)
            return sprite;
    }
    return NULL;
}

// Shoves the closest sprite away if a sprite is grabbed
static void shove(const Sprites sprites)
{
    Sprite* const grabbed = find(sprites, GRABBED);
    if(!grabbed)
        return;
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Do not count the sprite that is currently grabbed
        if(sprite == grabbed)
            continue;
        // Do not shove a sprite that is not moveable
        if(!sprite->moveable)
            continue;
        const float width = max(sprite->width, grabbed->width);
        if(eql(sprite->where, grabbed->where, width))
        {
            const Point delta = sub(sprite->where, grabbed->where);
            place(sprite, add(sprite->where, delta));
        }
    }
}

// Puts a sprite in front of the hero if the sprite goes out of bounds
static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(tile(sprite->where, map.walling))
            sprite->where = mid(sprite->last);
    }
}

// Sprite chases hero if hero gets close enough. Sprite will get bored and stop chasing
static void chase(const Sprites sprites, const Hero hero, const int ticks)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Non-movable sprites cannot move!
        if(!sprite->moveable)
            continue;
        // Sprites already in a timed state must pass
        if(sprite->ticks > ticks)
            continue;
        if(mag(sub(sprite->where, hero.where)) < 4.0)
        {
            sprite->state = CHASING;
            sprite->ticks = ticks + 30;
        }
    }
}

static void move(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->state == CHASING)
        {
            const Point dir = mul(unt(sub(sprite->where, hero.where)), -2e-2);
            place(sprite, add(sprite->where, dir));
        }
    }
}

Sprites caretake(const Sprites sprites, const Hero hero, const Input input, const Map map, const int ticks)
{
    rearrange(sprites, hero);
    // Sprite states - lowest to highest priority - rest always occur after state timeout
    rest(sprites, ticks);
    chase(sprites, hero, ticks);
    hurt(sprites, hero, ticks);
    grab(sprites, hero, input, ticks);
    surrender(sprites);
    // Sprite moving
    move(sprites, hero);
    shove(sprites);
    bound(sprites, map);
    // Will be modified when sprites create other sprites
    return sprites;
}
