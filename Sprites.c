#include "Sprites.h"

#include "Field.h"
#include "util.h"

#include "SDL2/SDL.h"
#include <ctype.h>
#include <math.h>

static Sprite generic(const Point where)
{
    Sprite sprite;
    xzero(sprite);
    sprite.where = where;
    sprite.ascii = 'Z';
    sprite.speed = 0.035;
    sprite.acceleration = 0.002;
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

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o_(where);
    }
    return generic(where);
}

Sprites xwake(const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = xconcat("sprites/lvl", which);
    FILE* const file = fopen(path, "r");
    Sprites sprites;
    sprites.count = xlns(file);
    sprites.sprite = xtoss(Sprite, sprites.count);
    sprites.max = sprites.count;
    for(int i = 0; i < sprites.count; i++)
    {
        char* const line = xreadln(file);
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

void xkill(const Sprites sprites)
{
    free(sprites.sprite);
}

Sprites xrewake(const Sprites sprites, const int level)
{
    xkill(sprites);
    return xwake(level);
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

static int backwards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return xmag(pa) < xmag(pb) ? +1 : xmag(pa) > xmag(pb) ? -1 : 0;
}

static int forewards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    return xmag(pa) < xmag(pb) ? -1 : xmag(pa) > xmag(pb) ? +1 : 0;
}

static void sort(const Sprites sprites, const int foreward)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite),
        foreward ? forewards : backwards);
}

static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = xtoss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

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
    sort(copied, false);
    return copied;
}

static void arrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, true);
    push(sprites, hero);
}

static void place(Sprite* const sprite, const Point to)
{
    sprite->last = sprite->where;
    sprite->where = to;
}

// Grabs the closest sprite when using hands
static void grab(const Sprites sprites, const Hero hero, const Input input)
{
    if(!input.l)
        return;
    const Point hand = xtouch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Cannot move immovable sprites
        if(!sprite->moveable)
            continue;
        if(xeql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            xzero(sprite->velocity);
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

static void idle(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->state = IDLE;
    }
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
        const float width = xmax(sprite->width, grabbed->width);
        if(xeql(sprite->where, grabbed->where, width))
        {
            const Point delta = xsub(sprite->where, grabbed->where);
            place(sprite, xadd(sprite->where, delta));
        }
    }
}

// Puts a sprite in the last good spot if it goes out of bounds
static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(xtile(sprite->where, map.walling))
        {
            place(sprite, xmid(sprite->last));
            xzero(sprite->velocity);
            // TODO: add a dizzy animation
        }
    }
}

// Moves sprite along path towards the hero player scent
static void move(const Sprites sprites, const Field field, const Point to)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Do not move the sprite if the sprite is immovable
        if(!sprite->moveable)
            continue;
        const Point dir = xforce(field, sprite->where, to);
        // No force of direction...
        if(dir.x == 0.0 && dir.y == 0.0)
            // Then slow down
            sprite->velocity = xmul(sprite->velocity, 1.0 - sprite->acceleration / sprite->speed);
        // Otherwise speed up
        else
        {
            const Point acc = xmul(dir, sprite->acceleration);
            sprite->velocity = xadd(sprite->velocity, acc);
            // And then check top speed...
            if(xmag(sprite->velocity) > sprite->speed)
                // And cap speed if the top speed is surpassed
                sprite->velocity = xmul(xunt(sprite->velocity), sprite->speed);
        }
        // If the sprite is fast enough they will animate chase
        if(xmag(sprite->velocity) > 0.01) sprite->state = CHASING;
        // Place the sprite at their new location...
        place(sprite, xadd(sprite->where, sprite->velocity));
    }
}

// Collaborative diffusion with various scents
static void route(const Sprites sprites, const Field field, const Map map, const Hero hero)
{
    // Wall scents are repel
    for(int j = 0; j < field.rows; j++)
    for(int i = 0; i < field.cols; i++)
    {
        const int y = j / field.res;
        const int x = i / field.res;
        if(map.walling[y][x] != ' ') field.mesh[j][i] = -1e3;
    }
    // Sprite scents stack on one another
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int j = field.res * sprite->where.y;
        const int i = field.res * sprite->where.x;
        // If the tile is already scented then a sprite is already there
        if(field.mesh[j][i] < 0.0) xzero(sprite->velocity);
        field.mesh[j][i] -= sprite->width;
    }
    // Hero scent
    const int j = field.res * hero.where.y;
    const int i = field.res * hero.where.x;
    field.mesh[j][i] = hero.scent;
    // Diffuse the culminated scent across the field
    xdiffuse(field, hero.where);
}

void xcaretake(const Sprites sprites, const Hero hero, const Input input, const Map map)
{
    // Sprites need to be arrange closest to hero first
    arrange(sprites, hero);
    // Default state for all sprites per iteration is idle - this will be overwritten
    idle(sprites);
    // Sprite path finding and movement
    const Field field = xprepare(map);
    route(sprites, field, map, hero);
    move(sprites, field, hero.where);
    xruin(field);
    // Sprite placement - interactive and out of bounds
    grab(sprites, hero, input);
    shove(sprites);
    bound(sprites, map);
}
