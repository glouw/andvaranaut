#include "Gpu.h"
#include "Display.h"
#include "Boundary.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Wall.h"
#include "Util.h"

Gpu setup(const int res, const int fps, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(window == NULL) puts("Could not open window");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = concat("config/", name);
    const Surfaces surfaces = pull(path, format);
    const unsigned long long renders = 0ull;
    const unsigned long long ticks = 0ull;
    const Gpu gpu = { res, fps, surfaces, window, renderer, texture, renders, ticks };
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

Gpu tick(const Gpu gpu, const unsigned long long renders)
{
    Gpu temp = gpu;
    temp.renders = renders;
    temp.ticks = renders / (gpu.fps / 4);
    return temp;
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
        const int slide = (gpu.res / 2) * hero.fov.a.x * sprite.where.y / sprite.where.x;
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
        // Select state
        const int states = 1;
        const int height = surface->h / states;
        const int state = height * sprite.state;
        // Select framing
        const int frames = 2;
        const int width = surface->w / frames;
        const int framing = width * (gpu.ticks % frames);
        // Get sprite on screen
        const float x = dx == 0.0 ? 0.0 : dx * width;
        const float w = width * (1.0 - dw);
        const SDL_Rect image = { fl(x) + framing, state, cl(w), height };
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
    float* const party = (float*) malloc(gpu.res * sizeof(*party));
    const int m = gpu.res / 2, l = gpu.res;
    for(int x = 0; x < m; x++) party[x] = fcast(hero.fov, gpu.res, x);
    for(int x = m; x < l; x++) party[x] = ccast(hero.fov, gpu.res, x);
    // Saves corrected ray casts for sprite renderer
    Point* const corrects = (Point*) malloc(gpu.res * sizeof(*corrects));
    Point* const wheres = (Point*) malloc(gpu.res * sizeof(*wheres));
    for(int y = 0; y < gpu.res; y++)
    {
        const Point column = lerp(camera, y / (float) gpu.res);
        const Scanline scanline = { gpu, display, y };
        // Several upper walls are rendered for seamless indoor/outdoor transitions.
        // Maps are required to have the outer most wall thickness _as many_ chars wide
        for(int uppers = 5, hits = uppers; hits > 0; hits--)
        {
            const Impact upper = march(hero, map.ceiling, column, gpu.res, hits);
            const Boundary boundary = { scanline, raise(upper.wall, gpu.res) };
            if(hits == uppers) srend(boundary, hero);
            wrend(boundary, upper.hit);
        }
        const Impact lower = march(hero, map.walling, column, gpu.res, 1);
        const Boundary boundary = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party };
        wrend(boundary, lower.hit);
        frend(boundary, wheres, map.floring, tracery);
        crend(boundary, wheres, map.ceiling);
        // Save lower wall hits for the sprite renderer
        corrects[y] = lower.traceline.corrected;
    }
    unlock(gpu);
    churn(gpu);
    paste(gpu, sprites, corrects, hero);
    present(gpu);
    free(wheres);
    free(corrects);
    free(party);
    const int t1 = SDL_GetTicks();
    const int ms = 1000.0 / gpu.fps - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
