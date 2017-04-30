#include "Sdl.h"

#include "Display.h"
#include "Boundary.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Light.h"

Sdl setup(const int res, const int fps, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(window == NULL) puts("Could not open window");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = concat("config/", name);
    const Surfaces surfaces = pull(path, format);
    const int renders = 0;
    const int ticks = 0;
    const Sdl sdl = { res, fps, surfaces, window, renderer, texture, renders, ticks };
    free(path);
    return sdl;
}

void release(const Sdl sdl)
{
    SDL_DestroyTexture(sdl.texture);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
    for(int i = 0; i < sdl.surfaces.count; i++) SDL_FreeSurface(sdl.surfaces.surface[i]);
    free(sdl.surfaces.surface);
}

Sdl tick(const Sdl sdl, const int renders)
{
    Sdl temp = sdl;
    temp.renders = renders;
    temp.ticks = renders / (sdl.fps / 6);
    return temp;
}

void churn(const Sdl sdl)
{
    SDL_RenderCopyEx(sdl.renderer, sdl.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

void present(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static SDL_Rect clip(const SDL_Rect frame, const Point where, const int res, Point* const lowers)
{
    SDL_Rect seen = frame;
    // Clips sprite from the left
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= res) continue;
        // Stop clipping if the sprite is seen
        if(where.x < lowers[x].x) break;
    }
    // Clips sprite from the right
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= res) continue;
        // Stops clipping if the sprite is seen - Increments width to avoid blank vertical line
        if(where.x < lowers[x].x) { seen.w++; break; }
    }
    return seen;
}

static void paste(const Sdl sdl, const Sprites sprites, Point* const lowers, const Hero hero)
{
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Moves onto the next sprite if this sprite is behind the player
        if(sprite.where.x < 0) continue;
        // Calculates sprite size
        const int size = focal(hero.fov) * sdl.res / sprite.where.x;
        const int corner = (sdl.res - size) / 2;
        const int slide = (sdl.res / 2) * ratio(hero.fov) * sprite.where.y / sprite.where.x;
        const SDL_Rect target = { corner + slide, corner, size, size };
        // Moves onto the next sprite if this sprite is off screen
        if(target.x + target.w < 0 || target.x >= sdl.res) continue;
        // Selects sprite
        SDL_Surface* const surface = sdl.surfaces.surface[sprite.ascii - ' '];
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        const SDL_Rect image = { w * (sdl.ticks % FRAMES), h * sprite.state, w, h };
        // Clips sprites and prevents dangerous gcc optimizations
        const volatile SDL_Rect seen = clip(target, sprite.where, sdl.res, lowers);
        // Moves onto the next sprite if this sprite totally behind a wall
        if(seen.w <= 0) continue;
        // Applies lighting to the sprite
        const int modding = illuminate(hero.light, sprite.where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Applies transperancy to the sprite
        if(sprite.transparent) SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Buffers the sprite
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        SDL_DestroyTexture(texture);
    }
}

void render(const Sdl sdl, const World world)
{
    const int t0 = SDL_GetTicks();
    // Precomputations
    float* const party = (float*) malloc(sdl.res * sizeof(*party));
    const int m = sdl.res / 2, l = sdl.res;
    for(int x = 0; x < m; x++) party[x] = fcast(world.hero.fov, sdl.res, x);
    for(int x = m; x < l; x++) party[x] = ccast(world.hero.fov, sdl.res, x);
    // Preallocations
    Point* const lowers = (Point*) malloc(sdl.res * sizeof(*lowers));
    Point* const wheres = (Point*) malloc(sdl.res * sizeof(*wheres));
    int* const moddings = (int*) malloc(sdl.res * sizeof(*moddings));
    // Raycaster: buffers with lighting walls, ceilings, floors, and sprites
    const Line camera = rotate(world.hero.fov, world.hero.angle.theta);
    const Display display = lock(sdl);
    for(int y = 0; y < sdl.res; y++)
    {
        const Point column = lerp(camera, y / (float) sdl.res);
        const Scanline scanline = { sdl, display, y };
        for(int max = 5, hits = max; hits > 0; hits--)
        {
            const Impact upper = march(world.hero, world.map.ceiling, column, sdl.res, hits);
            const Boundary boundary = { scanline, raise(upper.wall, sdl.res) };
            if(hits == max) srend(boundary, world.day);
            const int modding = illuminate(world.hero.light, upper.traceline.corrected.x);
            wrend(boundary, upper.hit, modding);
        }
        const Impact lower = march(world.hero, world.map.walling, column, sdl.res, 1);
        const Boundary boundary = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party, world.hero.light };
        const int modding = illuminate(world.hero.light, lower.traceline.corrected.x);
        wrend(boundary, lower.hit, modding);
        frend(boundary, wheres, world.map.floring, moddings, tracery);
        crend(boundary, wheres, world.map.ceiling, moddings);
        lowers[y] = lower.traceline.corrected;
    }
    unlock(sdl);
    churn(sdl);
    paste(sdl, world.sprites, lowers, world.hero);
    // Presents buffer and cleans up
    present(sdl);
    free(wheres);
    free(lowers);
    free(party);
    free(moddings);
    // Locks refresh rate
    const int t1 = SDL_GetTicks();
    const int ms = 1000.0 / sdl.fps - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
