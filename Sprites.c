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
    sprite.transparent = false;
    sprite.width = 0.66;
    sprite.health = 0.66;
    return sprite;
}

static Sprite _o(const Point where)
{
    Sprite sprite = generic(where);
    sprite.width = 0.66;
    sprite.ascii = 'o';
    sprite.health = 100.0;
    return sprite;
}

static Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o(where);
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

extern void entomb(const Sprites sprites, const int level)
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

static void sort(const Sprites sprites, const bool foreward)
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

extern void rest(const Sprites sprites, const State state)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(sprite->state == state)
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

extern bool issprite(const int ascii)
{
    return isalpha(ascii);
}

// Hurts all sprites within Area of Effect (AOE).
// Weapon AOE is overriden by sprite width if larger than weapon AOE.
// Thus, a large AOE weapon can hurt many small sprites at once with great ease,
// but a small AOE weapon can hurt one small sprite with great difficulty,
// and a small AOE weapon can hurt one large sprite with great ease
static void hurt(const Sprites sprites, const Hero hero)
{
    if(hero.inserting)
        return;
    if(hero.consoling)
        return;
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        const float aoe = max(hero.attack.area, sprite->width);
        if(eql(hero.attack.tip, sprite->where, aoe))
        {
            sprite->health -= hero.attack.power;
        }
    }
}

static Sprites place(const Sprites sprites, const Hero hero, const Input input)
{
    if(hero.inserting)
        return sprites;
    if(!hero.consoling)
        return sprites;
    if(!(input.key[SDL_SCANCODE_K] || input.r))
        return sprites;
    if(!issprite(hero.surface))
        return sprites;
    Sprites temp = sprites;
    if(temp.count == 0)
        retoss(temp.sprite, Sprite, temp.max = 1);
    if(temp.count >= temp.max)
        retoss(temp.sprite, Sprite, temp.max *= 2);
    const Point hand = touch(hero, hero.arm);
        temp.sprite[temp.count++] = registrar(hero.surface, hand);
    return temp;
}

// Grabs the closest sprite to move the sprite.
// Only applicable within console mode.
// Note that this is not a gameplay feature.
// Great for map design
static void grab(const Sprites sprites, const Hero hero, const Input input)
{
    if(hero.inserting)
        return;
    if(!hero.consoling)
        return;
    if(!(input.key[SDL_SCANCODE_J] || input.l))
        return;
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(eql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            sprite->where = hand;
            return;
        }
    }
}

extern Sprites caretake(const Sprites sprites, const Hero hero, const Input input)
{
    rearrange(sprites, hero);
    rest(sprites, GRABBED);
    grab(sprites, hero, input);
    hurt(sprites, hero);
    surrender(sprites);
    return place(sprites, hero, input);
}
