#include "Sdl.h"
#include "Display.h"
#include "Boundary.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Wall.h"
#include "Util.h"

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
    for(int i = 0; i < sdl.surfaces.count; i++)
        SDL_FreeSurface(sdl.surfaces.surface[i]);
    free(sdl.surfaces.surface);
}

Sdl tick(const Sdl sdl, const int renders)
{
    Sdl temp = sdl;
    temp.renders = renders;
    temp.ticks = renders / (sdl.fps / 4);
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
    // Clip from the left
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= res) continue;
        // Stop clipping if the sprite is seen
        if(where.x < lowers[x].x) break;
    }
    // Clip from the right
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= res) continue;
        // Stop clipping if the sprite is seen - Increments width to avoid blank vertical line
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
        const int slide = (sdl.res / 2) * hero.fov.a.x * sprite.where.y / sprite.where.x;
        const SDL_Rect target = { corner + slide, corner, size, size };
        // Moves onto the next sprite if this sprite is off screen
        if(target.x + target.w < 0 || target.x >= sdl.res) continue;
        // Selects sprite
        SDL_Surface* const surface = sdl.surfaces.surface[sprite.ascii - ' '];
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        const SDL_Rect image = { w * (sdl.ticks % FRAMES), h * sprite.state, w, h };
        // GCC loves to optimize away this SDL_Rect for some reason
        const volatile SDL_Rect seen = clip(target, sprite.where, sdl.res, lowers);
        // Moves onto the next sprite if this sprite totally behind a wall
        if(seen.w <= 0) continue;
        // Applies sprite lighting
        const int mod = hero.torch / (sprite.where.x * sprite.where.x);
        const int clamp = mod > 0xFF ? 0xFF : mod;
        SDL_SetTextureColorMod(texture, clamp, clamp, clamp);
        // Renders the sprite
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        SDL_DestroyTexture(texture);
    }
}

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.angle.theta);
    const Display display = lock(sdl);
    // Precomputations
    float* const party = (float*) malloc(sdl.res * sizeof(*party));
    const int m = sdl.res / 2, l = sdl.res;
    for(int x = 0; x < m; x++) party[x] = fcast(hero.fov, sdl.res, x);
    for(int x = m; x < l; x++) party[x] = ccast(hero.fov, sdl.res, x);
    // Preallocations
    Point* const lowers = (Point*) malloc(sdl.res * sizeof(*lowers));
    Point* const wheres = (Point*) malloc(sdl.res * sizeof(*wheres));
    for(int y = 0; y < sdl.res; y++)
    {
        const Point column = lerp(camera, y / (float) sdl.res);
        const Scanline scanline = { sdl, display, y };
        // Samples several upper walls for seamless indoor/outdoor transitions
        for(int max = 5, hits = max; hits > 0; hits--)
        {
            const Impact upper = march(hero, map.ceiling, column, sdl.res, hits);
            const Boundary boundary = { scanline, raise(upper.wall, sdl.res) };
            if(hits == max) srend(boundary, hero.angle.percent, sdl.ticks);
            wrend(boundary, upper.hit);
        }
        const Impact lower = march(hero, map.walling, column, sdl.res, 1);
        const Boundary boundary = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party };
        wrend(boundary, lower.hit);
        frend(boundary, wheres, map.floring, tracery);
        crend(boundary, wheres, map.ceiling);
        // Saves lower wall hit for the sprite renderer
        lowers[y] = lower.traceline.corrected;
    }
    // Buffers screen with wall/ceiling/flooring casts
    unlock(sdl);
    churn(sdl);
    // Buffers sprites using lower and upper wall hits
    paste(sdl, sprites, lowers, hero);
    present(sdl);
    // Deallocations
    free(wheres);
    free(lowers);
    free(party);
    // Locks refresh to 60 frames per second
    const int t1 = SDL_GetTicks();
    const int ms = 1000.0 / sdl.fps - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
