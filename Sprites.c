#include "Sprites.h"

#include "Path.h"
#include "util.h"

#include "SDL2/SDL.h"
#include <ctype.h>
#include <math.h>

static Sprite generic(const Point where)
{
    Sprite sprite;
    zero(sprite);
    sprite.where = where;
    sprite.ascii = 'Z';
    sprite.speed = 0.06;
    sprite.acceleration = 0.004;
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
    qsort(sprites.sprite, sprites.count, sizeof(Sprite),
        foreward ? forewards : backwards);
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

static void idle(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        sprite->state = IDLE;
    }
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
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Cannot move immovable sprites
        if(!sprite->moveable)
            continue;
        if(eql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            zero(sprite->velocity);
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

// Puts a sprite in the last good spot if it goes out of bounds
static void bound(const Sprites sprites, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(tile(sprite->where, map.walling))
            sprite->where = mid(sprite->last);
    }
}

// Moves sprite along path towards the hero player scent
static void move(const Sprites sprites, const Path path, const Map map, const Point where)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        const Point dir = force(path.field, where, sprite->where);
        const Point acc = mul(dir, sprite->acceleration);
        printf("%f %f\n", acc.x, acc.y);
        if(acc.x == 0.0 && acc.y == 0.0)
        {
            puts("slowing down");
            sprite->velocity = mul(sprite->velocity, 1.0 - sprite->acceleration / sprite->speed);
        }
        else
        {
            puts("\tspeeding up");
            // Accelerate...
            sprite->velocity = add(sprite->velocity, acc);
            // And then check top speed...
            if(mag(sprite->velocity) > sprite->speed)
                // And cap speed if the top speed is surpassed
                sprite->velocity = mul(unt(sprite->velocity), sprite->speed);
        }
        // Place the sprite at their new location...
        place(sprite, add(sprite->where, sprite->velocity));
        // If the sprite is out of bounds then set velocity to zero
        // and place the sprite at their last good position
        if(tile(sprite->where, map.walling))
        {
            sprite->where = sprite->last;
            zero(sprite->velocity);
        }
    }
}

// Collaborative diffusion
static void route(const Sprites sprites, const Path path, const Map map, const Hero hero)
{
    // Wall anti-objects
    for(int j = 0; j < path.rows; j++)
    for(int i = 0; i < path.cols; i++)
        if(map.walling[j][i] != ' ') path.field[j][i] = -1.0;
    // Sprite anti-objects
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        const int y = sprite->where.y;
        const int x = sprite->where.x;
        path.field[y][x] -= sprite->width;
    }
    // Place a scent at the hero location
    const int y = hero.where.y;
    const int x = hero.where.x;
    path.field[y][x] = hero.scent;
    // Diffuse the scent across the path towards the anti-objects
    diffuse(path, y, x);
    // Anti-objects now follow the scent
    move(sprites, path, map, hero.where);
}

void caretake(const Sprites sprites, const Hero hero, const Input input, const Map map)
{
    rearrange(sprites, hero);
    // Path finding and movement
    const Path path = prepare(map);
    route(sprites, path, map, hero);
    ruin(path);
    // Sprite states - lowest to highest priority for preemption.
    idle(sprites);
    grab(sprites, hero, input);
    // Sprite placement
    shove(sprites);
    bound(sprites, map);
}
