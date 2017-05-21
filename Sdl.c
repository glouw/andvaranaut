#include "Sdl.h"

#include "Display.h"
#include "Sliver.h"
#include "Sprites.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Util.h"
#include "String.h"
#include "Torch.h"
#include "Textures.h"
#include "Gui.h"

static void churn(const Sdl sdl)
{
    SDL_RenderCopyEx(sdl.renderer, sdl.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

static void present(const Sdl sdl)
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
        const int modding = illuminate(hero.torch, sprite.where.x);
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

extern Sdl setup(const int res, const int fps)
{
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Sdl sdl;
    sdl.window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    sdl.texture = SDL_CreateTexture(sdl.renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    sdl.surfaces = pull(format);
    sdl.textures = cache(sdl.surfaces, sdl.renderer);
    sdl.renders = 0;
    sdl.ticks = 0;
    sdl.res = res;
    sdl.fps = fps;
    sdl.font = TTF_OpenFont("fonts/alterebro-pixel-font.ttf", res / 8);
    return sdl;
}

extern void release(const Sdl sdl)
{
    clean(sdl.surfaces);
    purge(sdl.textures);
    SDL_DestroyTexture(sdl.texture);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
    TTF_CloseFont(sdl.font);
}

extern Sdl tick(const Sdl sdl, const int renders)
{
    Sdl temp = sdl;
    temp.renders = renders;
    temp.ticks = renders / (sdl.fps / 5);
    return temp;
}

extern void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map)
{
    // Sprite location relative to player
    const Sprites relatives = arrange(sprites, hero);
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
        const Trajectory trajectory =  { map.walling, column };
        // Once upon a time this engine supported upper and lower wall rendering.
        // The upper wall rendering has been removed, but the lower wall renderer,
        // that which is eye level with player, remained and kept name
        const Impact lower = march(hero, trajectory, sdl.res);
        const Sliver sliver = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party, hero.torch };
        const int modding = illuminate(hero.torch, lower.traceline.corrected.x);
        wrend(sliver, lower.hit, modding);
        frend(sliver, map.floring, wheres, moddings, tracery);
        crend(sliver, map.ceiling, wheres, moddings);
        lowers[y] = lower.traceline.corrected;
    }
    unlock(sdl);
    churn(sdl);
    paste(sdl, relatives, lowers, hero);
    gui(sdl, hero, relatives);
    present(sdl);
    free(party);
    free(lowers);
    free(wheres);
    free(moddings);
    kill(relatives);
}
