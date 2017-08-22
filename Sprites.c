#include "Sprites.h"

#include "Point.h"
#include "Util.h"
#include "Hero.h"
#include "String.h"

static Sprite generic(const Point where)
{
    Sprite sprite;
    zero(sprite);
    sprite.where = where;
    sprite.ascii = 'o';
    sprite.state = IDLE;
    sprite.width = 0.66;
    sprite.health = 50.0;
    sprite.moveable = true;
    return sprite;
}

static Sprite _i_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'i';
    sprite.width = 0.3;
    sprite.moveable = false;
    return sprite;
}

static Sprite _j_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'j';
    sprite.width = 0.3;
    sprite.moveable = false;
    return sprite;
}

static Sprite _o_(const Point where)
{
    Sprite sprite = generic(where);
    sprite.ascii = 'o';
    sprite.width = 0.66;
    return sprite;
}

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o_(where);
        case 'i': return _i_(where);
        case 'j': return _j_(where);
    }
    return generic(where);
}

extern Sprites wake(const int level)
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

extern void freeze(const Sprites sprites, const int level)
{
    char which[MINTS];
    sprintf(which, "%d", level);
    char* const path = concat("sprites/lvl", which);
    remove(path);
    FILE* const file = fopen(path, "w");
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        fprintf(file, "%c,%f,%f\n", sprite->ascii, sprite->where.x, sprite->where.y);
    }
    fclose(file);
    free(path);
}

extern void kill(const Sprites sprites)
{
    free(sprites.sprite);
}

extern Sprites rewake(const Sprites sprites, const int level)
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

extern Sprites arrange(const Sprites sprites, const Hero hero)
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
static void rest(const Sprites sprites, const Sdl sdl)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->ticks < sdl.ticks)
            sprite->state = IDLE;
    }
}

static void surrender(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->health < 0.0)
            sprite->state = MERCY;
    }
}

extern int issprite(const int ascii)
{
    return isalpha(ascii);
}

static Compass needle(const Point vect)
{
    const float angle = atan2f(vect.y, vect.x);
    return (Compass) ((int) roundf(DIRS * angle / (2.0 * pi) + DIRS) % DIRS);
}

// Hurts all sprites within Area of Effect (AOE)
static void hurt(const Sprites sprites, const Hero hero, const Sdl sdl)
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
            sprite->ticks = sdl.ticks + 3;
            sprite->health -= hero.attack.power;
            sprite->state = (State) dir;
        }
    }
}

// Grabs the closest sprite when using hands
static void grab(const Sprites sprites, const Hero hero, const Input input)
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
            sprite->where = hand;
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
            sprite->where = add(sprite->where, delta);
        }
    }
}

// Puts a sprite in front of the hero if the sprite goes out of bounds
static void bound(const Sprites sprites, const Hero hero, const Map map)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(tile(sprite->where, map.walling))
            sprite->where = mid(hero.where);
    }
}

extern Sprites caretake(const Sprites sprites, const Hero hero, const Input input, const Sdl sdl, const Map map)
{
    rearrange(sprites, hero);
    rest(sprites, sdl);
    grab(sprites, hero, input);
    shove(sprites);
    hurt(sprites, hero, sdl);
    surrender(sprites);
    bound(sprites, hero, map);
    // Will be modified when sprites create other sprites
    return sprites;
}
