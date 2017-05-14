#include "Sdl.h"

#include "Display.h"
#include "Boundary.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Util.h"
#include "String.h"
#include "Light.h"
#include "Textures.h"
#include "Ruler.h"
#include "Ttf.h"

Sdl setup(const int res, const int fps, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(window == NULL)
        bomb("Could not open window\n");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
        bomb("No renderer available\n");
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = concat("config/", name);
    const Surfaces surfaces = pull(path, format);
    const Textures textures = cache(surfaces, renderer);
    const int renders = 0;
    const int ticks = 0;
    TTF_Font* const font = TTF_OpenFont("fonts/alterebro-pixel-font.ttf", res / 8);
    const Sdl sdl = { res, fps, surfaces, textures, window, renderer, texture, renders, ticks, font };
    free(path);
    return sdl;
}

void release(const Sdl sdl)
{
    clean(sdl.surfaces);
    purge(sdl.textures);
    SDL_DestroyTexture(sdl.texture);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
    TTF_CloseFont(sdl.font);
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

// Stop clipping if the sprite is seen
static SDL_Rect clip(const SDL_Rect frame, const Point where, const int res, Point* const lowers)
{
    SDL_Rect seen = frame;
    // Clips sprite from the left
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= res) continue;
        if(where.x < lowers[x].x) break;
    }
    // Clips sprite from the right
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= res) continue;
        if(where.x < lowers[x].x) { seen.w = seen.w + 1; break; }
    }
    return seen;
}

static void paste(const Sdl sdl, const Sprites sprites, Point* const lowers, const Hero hero)
{
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Moves onto the next sprite if this sprite is behind the player
        if(sprite.where.x < 0)
            continue;
        // Calculates sprite size
        const int size = focal(hero.fov) * sdl.res / sprite.where.x;
        const int corner = (sdl.res - size) / 2;
        const int slide = (sdl.res / 2) * ratio(hero.fov) * sprite.where.y / sprite.where.x;
        const SDL_Rect target = { corner + slide, corner, size, size };
        // Moves onto the next sprite if this sprite is off screen
        if(target.x + target.w < 0 || target.x >= sdl.res)
            continue;
        // Selects sprite
        const int selected = sprite.ascii - ' ';
        SDL_Surface* const surface = sdl.surfaces.surface[selected];
        SDL_Texture* const texture = sdl.textures.texture[selected];
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        const SDL_Rect image = { w * (sdl.ticks % FRAMES), h * sprite.state, w, h };
        // Clips sprites and prevents dangerous gcc optimizations
        const volatile SDL_Rect seen = clip(target, sprite.where, sdl.res, lowers);
        // Moves onto the next sprite if this sprite totally behind a wall
        if(seen.w <= 0)
            continue;
        // Applies lighting to the sprite
        const int modding = illuminate(hero.light, sprite.where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Applies transperancy to the sprite
        if(sprite.transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Buffers the sprite
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        // Removes transperancy from the sprite
        if(sprite.transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }
}

static void gui(const Sdl sdl, const Hero hero, const Sprites sprites)
{
    const int height = 0;
    const int margin = sdl.res / 32;
    Ruler ruler = { sdl, margin, height };
    ruler = selection(ruler, hero);
    ruler = countings(ruler, sprites);
    if(hero.consoling)
        insertion(ruler);
}

void print(const Sdl sdl, const int x, const int y, char* const text)
{
    const Ttf inner = { text, { 0xFF, 0xFF, 0x00, 0x00 }, 0 };
    const Ttf outer = { text, { 0x00, 0x00, 0x00, 0x00 }, 1 };
    scribble(outer, x, y, sdl);
    scribble(inner, x, y, sdl);
}

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map)
{
    const int t0 = SDL_GetTicks();
    // Precomputations
    float* const party = toss(float, sdl.res);
    const int m = sdl.res / 2;
    const int l = sdl.res / 1;
    for(int x = 0; x < m; x++) party[x] = fcast(hero.fov, sdl.res, x);
    for(int x = m; x < l; x++) party[x] = ccast(hero.fov, sdl.res, x);
    // Preallocations for render computations
    Point* wheres = toss(Point, sdl.res);
    int* moddings = toss(int, sdl.res);
    Point* const lowers = toss(Point, sdl.res);
    // Raycaster: buffers with lighting walls, ceilings, floors, and sprites
    const Line camera = rotate(hero.fov, hero.theta);
    const Display display = lock(sdl);
    for(int y = 0; y < sdl.res; y++)
    {
        const Point column = lerp(camera, y / (float) sdl.res);
        const Scanline scanline = { sdl, display, y };
        // Just a single hit for the lower walls
        const Range range =  { map.walling, column };
        const Impact lower = march(hero, range, sdl.res);
        const Boundary boundary = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party, hero.light };
        const int modding = illuminate(hero.light, lower.traceline.corrected.x);
        wrend(boundary, lower.hit, modding);
        const Calc calc = { wheres, moddings };
        frend(boundary, map.floring, calc, tracery);
        crend(boundary, map.ceiling, calc);
        lowers[y] = lower.traceline.corrected;
    }
    unlock(sdl);
    churn(sdl);
    paste(sdl, sprites, lowers, hero);
    gui(sdl, hero, sprites);
    // Presents buffer
    present(sdl);
    // Clean up all computations
    free(party);
    free(lowers);
    free(wheres);
    free(moddings);
    // Locks refresh rate
    const int t1 = SDL_GetTicks();
    const int ms = 1000.0 / sdl.fps - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
