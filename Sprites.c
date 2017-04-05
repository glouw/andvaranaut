#include "Sprites.h"

#include "util.h"

#include <stdlib.h>
#include <string.h>

static void print(const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
        printf("%f,%f: %c\n",
            sprites.sprite[i].where.x,
            sprites.sprite[i].where.y,
            sprites.sprite[i].ascii);
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
    print(sprites);
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
