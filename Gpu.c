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
    if(window == NULL) puts("Why are you in the console? Start X11 or something...");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = concat("config/", name);
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

static void paste(const Gpu gpu, const Sprites sprites, const Point* const corrects, const Hero hero)
{
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Move onto next sprite if this sprite is behind player
        if(sprite.where.x < 0) continue;
        // Calculate sprite size
        const float size = focal(hero.fov) * gpu.res / sprite.where.x;
        const float corner = (gpu.res - size) / 2.0;
        const float slide = (gpu.res / 2) * hero.fov.a.x * sprite.where.y / sprite.where.x;
        const SDL_Rect frame = { fl(corner) + slide, fl(corner), cl(size), cl(size) };
        // Trim from the left
        SDL_Rect scope = frame;
        for(; scope.x < scope.x + scope.w; scope.x++, scope.w--)
        {
            const int index = scope.x;
            if(index < 0 || index >= gpu.res) break;
            // Stop trimming if the sprite is seen
            if(sprite.where.x < corrects[index].x) break;
        }
        // Trim from the right
        for(; scope.x > scope.x - scope.w; scope.w--)
        {
            const int index = scope.x + scope.w;
            if(index < 0 || index >= gpu.res) break;
            // Stop trimming if the sprite is seen
            if(sprite.where.x < corrects[index].x) break;
        }
        // Move onto next sprite if this sprite is totally behind a wall
        if(scope.w <= 0) continue;
        // Move onto the next sprite if this sprite is off screen
        if(scope.x > gpu.res || scope.x + scope.w < 0) continue;
        // Trim source sprite
        const float dx = (scope.x - frame.x) / (float) frame.w;
        const float dw = (frame.w - scope.w) / (float) frame.w;
        SDL_Surface* const surface = gpu.surfaces.surface[sprite.ascii - ' '];
        const int x = dx == 0.0 ? 0.0 : rnd(dx * surface->w);
        const int w = rnd(surface->w * (1.0 - dw));
        const SDL_Rect image = { x, 0, w, surface->h };
        // Get sprite on screen
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(gpu.renderer, surface);
        SDL_RenderCopy(gpu.renderer, texture, &image, &scope);
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
