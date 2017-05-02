#include "Sprites.h"

#include "Point.h"
#include "Util.h"
#include "String.h"

void prints(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        const int ascii = sprites.sprite[i].ascii;
        const int state = sprites.sprite[i].state;
        const Point where = sprites.sprite[i].where;
        printf("%c,%d,%f,%f", ascii, state, where.x, where.y);
    }
}

Sprites wake(const char* const name)
{
    char* const path = concat("sprites/", name);
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    Sprite* const sprite = (Sprite*) malloc(count * sizeof(*sprite));
    for(int i = 0; i < count; i++)
    {
        Point where = { 0.0, 0.0 };
        char* const line = readln(file);
        char* const location = strtok(line, " ");
        char ascii = 0;
        int state = 0, transparent = 0;
        sscanf(location, "%c,%d,%f,%f,%d", &ascii, &state, &where.x, &where.y, &transparent);
        sprite[i].where = where;
        sprite[i].ascii = ascii;
        sprite[i].state = (State) state;
        sprite[i].transparent = transparent;
        free(line);
    }
    const Sprites sprites = { count, sprite };
    fclose(file);
    free(path);
    return sprites;
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

static Sprites copy(const Sprites sprites)
{
    const int count = sprites.count;
    Sprite* const temp = (Sprite*) malloc(sprites.count * sizeof(*sprites.sprite));
    const Sprites temps = { count, temp };
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

Sprites arrange(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    push(copied, hero);
    turn(copied, hero);
    sort(copied);
    return copied;
}
