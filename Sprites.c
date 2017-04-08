#include "Sprites.h"
#include "Point.h"
#include "Line.h"
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
        char* const ascii = strtok(NULL, " "); // No comments allowed after
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

static void push(const Sprites copied, const Hero hero)
{
    for(int i = 0; i < copied.count; i++)
        copied.sprite[i].where = sub(copied.sprite[i].where, hero.where);
}

static void turn(const Sprites copied, const Hero hero)
{
    for(int i = 0; i < copied.count; i++)
        copied.sprite[i].where = trn(copied.sprite[i].where, -hero.angle.theta);
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

Sprites update(const Sprites sprites, const Hero hero)
{
    const Sprites copied = copy(sprites);
    push(copied, hero);
    turn(copied, hero);
    sort(copied);
    return copied;
}

void paste(const Sprites sprites, const Gpu gpu, Impact* const impacts, const Hero hero, const int res)
{
    for(int i = 0; i < sprites.count; i++)
    {
        const Sprite sprite = sprites.sprite[i];
        const int index = sprite.ascii - ' ';
        SDL_Surface* const surface = gpu.surfaces.surface[index];
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(gpu.renderer, surface);
        const int height = focal(hero.fov) * res / sprite.where.x;
        const int mid = res / 2 - height / 2;
        const int offset = res / 2 * sprite.where.y / (float) sprite.where.x;
        SDL_Rect dst = { mid + offset, mid, height, height };
        SDL_Rect src = { 0, 0, surface->w, surface->h };
        SDL_RenderCopy(gpu.renderer, texture, &src, &dst);
        SDL_DestroyTexture(texture);
    }
}
