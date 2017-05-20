#include "Sprites.h"

#include "Point.h"
#include "Util.h"
#include "String.h"

static Sprites copy(const Sprites sprites)
{
    Sprites temps;
    temps.count = sprites.count;
    temps.max = sprites.max;
    temps.sprite = toss(Sprite, sprites.count);
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(Sprite));
    return temps;
}

static void pull(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
        sprites.sprite[i].where = sub(sprites.sprite[i].where, hero.where);
}

static void push(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
        sprites.sprite[i].where = add(sprites.sprite[i].where, hero.where);
}

static void turn(const Sprites sprites, const Hero hero)
{
    for(int i = 0; i < sprites.count; i++)
        sprites.sprite[i].where = trn(sprites.sprite[i].where, -hero.theta);
}

static int backwards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    if(mag(pa) < mag(pb)) return +1; else if(mag(pa) > mag(pb)) return -1;
    return 0;
}

static int forewards(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    if(mag(pa) < mag(pb)) return -1; else if(mag(pa) > mag(pb)) return +1;
    return 0;
}

static void sort(const Sprites sprites, const bool foreward)
{
    qsort(sprites.sprite, sprites.count, sizeof(Sprite), foreward ? forewards : backwards);
}

int count(const Sprites sprites, const State state)
{
    int occurances = 0;
    for(int i = 0; i < sprites.count; i++)
        if(sprites.sprite[i].state == state)
            occurances++;
    return occurances;
}

void prints(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        const Sprite sprite = sprites.sprite[i];
        printf("%c,%f,%f\n", sprite.ascii, sprite.where.x, sprite.where.y);
    }
}

Sprites wake(const char* const name)
{
    char* const path = concat("sprites/", name);
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

void entomb(const Sprites sprites, const char* const name)
{
    char* const path = concat("sprites/", name);
    remove(path);
    FILE* const file = fopen(path, "w");
    for(int i = 0; i < sprites.count; i++)
        fprintf(file, "%c,%f,%f\n",
            sprites.sprite[i].ascii,
            sprites.sprite[i].where.x,
            sprites.sprite[i].where.y);
    fclose(file);
    free(path);
}

void kill(const Sprites sprites)
{
    free(sprites.sprite);
}

Sprites swap(const Sprites sprites, const char* const name)
{
    kill(sprites);
    return wake(name);
}

Sprites arrange(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    pull(copied, hero);
    turn(copied, hero);
    sort(copied, false);
    return copied;
}

void rearrange(const Sprites sprites, const Hero hero)
{
    pull(sprites, hero);
    sort(sprites, true);
    push(sprites, hero);
}

void rest(const Sprites sprites, const State state)
{
    for(int i = 0; i < sprites.count; i++)
        if(sprites.sprite[i].state == state)
            sprites.sprite[i].state = IDLE;
}

static Sprite birth(const Point where)
{
    Sprite sprite;
    sprite.where = where;
    sprite.ascii = 'o';
    sprite.state = IDLE;
    sprite.transparent = false;
    sprite.width = 1.00;
    return sprite;
}

static Sprite _o(const Point where)
{
    Sprite sprite = birth(where);
    sprite.ascii = 'o';
    sprite.width = 0.88;
    return sprite;
}

static Sprite _g(const Point where)
{
    Sprite sprite = birth(where);
    sprite.ascii = 'g';
    sprite.transparent = true;
    sprite.width = 0.66;
    return sprite;
}

static Sprite _z(const Point where)
{
    Sprite sprite = birth(where);
    sprite.ascii = 'z';
    sprite.width = 0.33;
    return sprite;
}

Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'o': return _o(where);
        case 'g': return _g(where);
        case 'z': return _z(where);
    }
    return birth(where);
}
