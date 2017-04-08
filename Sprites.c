#include "Sprites.h"
#include "Point.h"
#include "Hero.h"
#include "Util.h"
#include <stdlib.h>
#include <string.h>

void prints(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        const Point where = sprites.sprite[i].where;
        const int ascii = sprites.sprite[i].ascii;
        printf("%f,%f: %c\n", where.x, where.y, ascii);
    }
}

Sprites wake(const char* const name)
{
    char* const path = strcon("sprites/", name);
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    Sprite* const sprite = (Sprite*) calloc(count, sizeof(*sprite));
    for(int i = 0; i < count; i++)
    {
        Point where = { 0.0, 0.0 };
        char* const line = readln(file);
        char* const location = strtok(line, " ");
        sscanf(location, "%f,%f", &where.x, &where.y);
        sprite[i].where = where;
        char* const ascii = strtok(NULL, " #");
        sprite[i].ascii = ascii[0];
        free(line);
    }
    const Sprites sprites = { count, sprite };
    fclose(file);
    free(path);
    return sprites;
}

void sleep(const Sprites sprites)
{
    free(sprites.sprite);
}

Sprites swap(const Sprites sprites, const char* const name)
{
    sleep(sprites);
    return wake(name);
}

static Sprites copy(const Sprites sprites)
{
    const int count = sprites.count;
    Sprite* const temp = (Sprite*) calloc(sprites.count, sizeof(*sprites.sprite));
    const Sprites temps = { count, temp };
    memcpy(temps.sprite, sprites.sprite, sprites.count * sizeof(*sprites.sprite));
    return temps;
}

static void turn(const Sprites copied, const Hero hero)
{
    for(int i = 0; i < copied.count; i++)
    {
        const Point basing = sub(copied.sprite[i].where, hero.where);
        const Point turned = trn(basing, hero.angle.theta);
        const Point backed = add(turned, hero.where);
        copied.sprite[i].where = backed;
    }
}

static void sort(const Sprites copied, const Hero hero)
{
    (void) copied, (void) hero;
}

Sprites update(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    turn(copied, hero);
    sort(copied, hero);
    return copied;
}

SDL_Surface* paste(const Sprites sprites)
{
    (void) sprites;
    return NULL;
}
