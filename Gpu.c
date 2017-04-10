#include "Gpu.h"
#include "Display.h"
#include "Scanline.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Wall.h"
#include "Util.h"

Gpu setup(const int res, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(window == NULL)
        puts("Why are you in the console? Start X11 or something...");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = strcon("config/", name);
    const Surfaces surfaces = pull(path, format);
    const Gpu gpu = { res, surfaces, window, renderer, texture };
    free(path);
    return gpu;
}

void release(const Gpu gpu)
{
    SDL_DestroyTexture(gpu.texture);
    SDL_Quit();
    SDL_DestroyWindow(gpu.window);
    SDL_DestroyRenderer(gpu.renderer);
    for(int i = 0; i < gpu.surfaces.count; i++)
        SDL_FreeSurface(gpu.surfaces.surface[i]);
    free(gpu.surfaces.surface);
}

void churn(const Gpu gpu)
{
    SDL_RenderCopyEx(gpu.renderer, gpu.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

void present(const Gpu gpu)
{
    SDL_RenderPresent(gpu.renderer);
}

typedef struct
{
    int lb;
    int rb;
    SDL_Rect rect;
}
Frame;

typedef struct
{
    int offset;
    int res;
    int size;
}
Dimensions;

static Frame hang(const Point where, const Point* const corrects, const Dimensions dim)
{
    SDL_Rect rect = { 0, (dim.res - dim.size) / 2, 0, dim.size };
    // Trim destination sprite from the left
    int lb = 0;
    int rb = dim.size;
    for(; lb < rb; lb++)
    {
        rect.x = (dim.res - dim.size) / 2 + lb + dim.offset;
        rect.w = rb - lb;
        const int index = rect.x;
        if(index < 0 || index > dim.res)
            break;
        // Stop trimming of the sprite is seen
        if(where.x < corrects[index].x)
            break;
    }
    // Trim destination sprite from the right
    for(; rb > lb; rb--)
    {
        rect.w = rb - lb;
        const int index = rect.x + rect.w;
        if(index < 0 || index > dim.res)
            break;
        // Stop trimming of the sprite is seen
        if(where.x < corrects[index].x)
            break;
    }
    const Frame frame = { lb, rb, rect };
    return frame;
}

static void paste(const Gpu gpu, const Sprites sprites, const Point* const corrects, const Hero hero)
{
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Move onto the next sprite if this sprite is behind the player
        if(sprite.where.x < 0)
            continue;
        // Move onto the next sprite if this sprite is behind the player
        const int offset = (gpu.res / 2) * hero.fov.a.x * sprite.where.y / sprite.where.x;
        const int size = focal(hero.fov) * gpu.res / sprite.where.x;
        const int location = offset + gpu.res / 2;
        const struct { int min, max; } boundry = { -size / 2, gpu.res + size / 2 };
        if(location < boundry.min || location > boundry.max)
            continue;
        // Calculate the sprite perimeter frame for the screen
        const Dimensions dimensions = { offset, gpu.res, size };
        const Frame frame = hang(sprite.where, corrects, dimensions);
        // Move onto the next sprite if this sprite is not seen
        if(frame.rb <= frame.lb)
            continue;
        // Calculate the sprite source dimensions to fit the sprite frame
        SDL_Surface* const surface = gpu.surfaces.surface[sprite.ascii - ' '];
        const float scale = surface->w / (float) size;
        const SDL_Rect rect = {
            rnd(scale * frame.lb), 0, rnd(scale * frame.rect.w), surface->h
        };
        // Paste to renderer
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(gpu.renderer, surface);;
        SDL_RenderCopy(gpu.renderer, texture, &rect, &frame.rect);
        SDL_DestroyTexture(texture);
    }
}

void render(const Gpu gpu, const Hero hero, const Sprites sprites, const Map map)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.angle.theta);
    const Display display = lock(gpu);
    // Precomputes floor and ceiling casts
    float* const party = (float*) calloc(gpu.res, sizeof(*party));
    for(int x = 0; x < gpu.res; x++)
        party[x] = x < gpu.res / 2 ? fcast(hero.fov, gpu.res, x) : ccast(hero.fov, gpu.res, x);
    // Saves corrected ray casts for sprite renderer
    Point* const corrects = (Point*) calloc(gpu.res, sizeof(*corrects));
    for(int y = 0; y < gpu.res; y++)
    {
        const Point column = lerp(camera, y / (float) gpu.res);
        const Scanline scanline = { gpu, display, y };
        srend(scanline, hero.angle.percent);
        // Five upper walls are rendered for seamless indoor/outdoor transitions
        const int uppers = 5;
        for(int hits = uppers; hits > 0; hits--)
        {
            const Impact upper = march(hero, map.ceiling, column, gpu.res, hits);
            wrend(scanline, raise(upper.wall, gpu.res), upper.hit);
        }
        const Impact lower = march(hero, map.walling, column, gpu.res, 1);
        corrects[y] = lower.traceline.corrected;
        wrend(scanline, lower.wall, lower.hit);
        // Saves computations from floorcasting for ceiling casting
        Point* const wheres = frend(scanline, lower.wall, lower.traceline, map.floring, party);
        crend(scanline, lower.wall, wheres, map.ceiling);
        free(wheres);
    }
    unlock(gpu);
    churn(gpu);
    paste(gpu, sprites, corrects, hero);
    present(gpu);
    free(corrects);
    free(party);
    const int t1 = SDL_GetTicks();
    const int ms = 15 - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
