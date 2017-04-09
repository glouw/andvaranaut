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
        SDL_Surface* const surface = gpu.surfaces.surface[sprite.ascii - ' '];
        const int size = focal(hero.fov) * res / sprite.where.x;
        const int offset = (res / 2) * hero.fov.a.x * sprite.where.y / (float) sprite.where.x;
        // Wrap me up
        SDL_Rect dst;
        dst.y = (res - size) / 2;
        dst.h = size;
        int lb = 0;
        int rb = size;
        while(lb < rb)
        {
            dst.x = (res - size) / 2 + lb + offset;
            dst.w = rb - lb;
            if(sprite.where.x < impacts[dst.x].traceline.corrected.x)
                break;
            lb = lb + 1;
        }
        while(rb > lb)
        {
            dst.w = rb - lb;
            if(sprite.where.x < impacts[dst.x + dst.w].traceline.corrected.x)
                break;
            rb = rb - 1;
        }
        const float scale = (float) surface->w / size;
        const SDL_Rect src = {
            /* x */ scale * lb,
            /* y */ 0,
            /* w */ scale * dst.w,
            /* h */ surface->h
        };
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(gpu.renderer, surface);
        SDL_RenderCopy(gpu.renderer, texture, &src, &dst);
        SDL_DestroyTexture(texture);
    }
}
