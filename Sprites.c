#include "Sprites.h"

#include "Point.h"
#include "Util.h"
#include "String.h"

static Sprites copy(const Sprites sprites)
{
    const int count = sprites.count;
    const int max = sprites.max;
    Sprite* const temp = toss(Sprite, count);
    const Sprites temps = { count, max, temp };
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
        return 1;
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
    const int count = lns(file);
    Sprite* const sprite = toss(Sprite, count);
    for(int i = 0; i < count; i++)
    {
        Point where = { 0.0, 0.0 };
        char* const line = readln(file);
        char* const location = strtok(line, " ");
        char ascii = 0;
        int state = 0;
        int transparent = 0;
        float width = 0.0;
        sscanf(location, "%c,%d,%f,%f,%d,%f",
            &ascii, &state, &where.x, &where.y, &transparent, &width);
        sprite[i].where = where;
        sprite[i].ascii = ascii;
        sprite[i].state = (State) state;
        sprite[i].transparent = (bool) transparent;
        sprite[i].width = width;
        free(line);
    }
    const int max = count;
    const Sprites sprites = { count, max, sprite };
    fclose(file);
    free(path);
    return sprites;
}

static Sprite oddish(const Point where)
{
    Sprite sprite;
    sprite.where = where;
    sprite.ascii = 'A';
    sprite.state = IDLE;
    sprite.transparent = false;
    sprite.width = 0.75;
    return sprite;
}

static Sprite torch(const Point where)
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
        case 'A': return oddish(where);
        case 'B': return torch(where);
    }
    return oddish(where);
}
