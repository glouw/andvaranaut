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
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(*sprites.sprite));
    return temps;
}

static void push(const Sprites copied, const Hero hero)
{
    for(int i = 0; i < copied.count; i++)
        copied.sprite[i].where = sub(copied.sprite[i].where, hero.where);
}

static void turn(const Sprites copied, const Hero hero)
{
    for(int i = 0; i < copied.count; i++)
        copied.sprite[i].where = trn(copied.sprite[i].where, -hero.theta);
}

static int comparator(const void *a, const void* b)
{
    const Point pa = *(const Point *) a;
    const Point pb = *(const Point *) b;
    if(mag(pa) < mag(pb))
        return +1;
    else
    if(mag(pa) > mag(pb))
        return -1;
    return 0;
}

static void sort(const Sprites copied)
{
    qsort(copied.sprite, copied.count, sizeof(*copied.sprite), comparator);
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
        int state = 0;
        Point where = { 0.0, 0.0 };
        int transparent = 0;
        float width = 0.0;
        sscanf(line, "%c,%d,%f,%f,%d,%f\n",
            &ascii,
            &state,
            &where.x,
            &where.y,
            &transparent,
            &width);
        sprites.sprite[i].where = where;
        sprites.sprite[i].ascii = ascii;
        sprites.sprite[i].state = (State) state;
        sprites.sprite[i].transparent = (bool) transparent;
        sprites.sprite[i].width = width;
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
        fprintf(file, "%c,%d,%f,%f,%d,%f\n",
            sprites.sprite[i].ascii,
            sprites.sprite[i].state,
            sprites.sprite[i].where.x,
            sprites.sprite[i].where.y,
            sprites.sprite[i].transparent,
            sprites.sprite[i].width);
    fclose(file);
    free(path);
}

static Sprite _A(const Point where)
{
    Sprite sprite;
    sprite.where = where;
    sprite.ascii = 'A';
    sprite.state = IDLE;
    sprite.transparent = false;
    sprite.width = 0.75;
    return sprite;
}

static Sprite _B(const Point where)
{
    Sprite sprite;
    sprite.where = where;
    sprite.ascii = 'A';
    sprite.state = IDLE;
    sprite.transparent = true;
    sprite.width = 0.75;
    return sprite;
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
    push(copied, hero);
    turn(copied, hero);
    sort(copied);
    return copied;
}

void rest(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
        sprites.sprite[i].state = IDLE;
}

Sprite registrar(const int ascii, const Point where)
{
    switch(ascii)
    {
        case 'A': return _A(where);
        case 'B': return _B(where);
    }
    return _A(where);
}
