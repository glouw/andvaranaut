#include "Sdl.h"

#include "Frame.h"
#include "Scanline.h"
#include "util.h"

static void churn(const Sdl sdl)
{
    const SDL_Rect dst = {
        (sdl.xres - sdl.yres) / 2,
        (sdl.yres - sdl.xres) / 2,
        sdl.yres,
        sdl.xres
    };
    SDL_RenderCopyEx(sdl.renderer, sdl.texture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

static void present(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static SDL_Rect clip(const Sdl sdl, const SDL_Rect frame, const Point where, Point* const zbuff)
{
    SDL_Rect seen = frame;
    // Clips sprite from the left
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= sdl.xres)
            continue;
        if(where.x < zbuff[x].x)
            break;
    }
    // Clips sprite from the right
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= sdl.xres)
            continue;
        if(where.x < zbuff[x].x)
        {
            seen.w = seen.w + 1;
            break;
        }
    }
    return seen;
}

static void paste(const Sdl sdl, const Sprites sprites, Point* const zbuff, const Hero hero, const int ticks)
{
    // Go through all the sprites
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Move onto the next sprite if this sprite is behind the player
        if(sprite.where.x < 0)
            continue;
        // Calculate sprite size - the sprite must be an even integer else the sprite will jitter
        const int size = xbalance(xfocal(hero.fov) * sdl.yres / sprite.where.x);
        // Calculate sprite location on screen
        const int l = (sdl.xres - size) / 2;
        const int t = (sdl.yres - size) / 2;
        const int slider = xfocal(hero.fov) * (sdl.xres / 2) * xslp(sprite.where);
        const SDL_Rect target = { l + slider, t, size, size };
        // Move onto the next sprite if this sprite is off screen
        if(target.x + target.w < 0 || target.x >= sdl.xres)
            continue;
        // Get sprite surface and texture
        const int selected = sprite.ascii - ' ';
        SDL_Surface* const surface = sdl.surfaces.surface[selected];
        SDL_Texture* const texture = sdl.textures.texture[selected];
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        // Determine sprite animation based on ticks
        const SDL_Rect image = { w * (ticks % FRAMES), h * sprite.state, w, h };
        // Calculate how much of the sprite is seen
        volatile const SDL_Rect seen = clip(sdl, target, sprite.where, zbuff);
        // Move onto the next sprite if this totally behind a wall and cannot be seen
        if(seen.w <= 0)
            continue;
        // Apply lighting to the sprite
        const int modding = xilluminate(hero.torch, sprite.where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Apply transperancy to the sprite, if required
        if(sprite.transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Renders the sprite
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        // Removes transperancy from the sprite
        if(sprite.transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }
}

Sdl xsetup(const int xres, const int yres, const int fps)
{
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    Sdl sdl;
    xzero(sdl);
    sdl.window = SDL_CreateWindow("water", 0, 0, xres, yres, SDL_WINDOW_SHOWN);
    if(!sdl.window)
        xbomb("error: could not open window\n");
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    // Notice the flip between yres and xres
    sdl.texture = SDL_CreateTexture(sdl.renderer, format, SDL_TEXTUREACCESS_STREAMING, yres, xres);
    sdl.surfaces = xpull(format);
    sdl.textures = xcache(sdl.surfaces, sdl.renderer);
    sdl.xres = xres;
    sdl.yres = yres;
    sdl.fps = fps;
    return sdl;
}

void xrelease(const Sdl sdl)
{
    xclean(sdl.surfaces);
    xpurge(sdl.textures);
    SDL_DestroyTexture(sdl.texture);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
}

void xrender(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const int ticks)
{
    // Preallocate for render computations
    Point* const wheres = xtoss(Point, sdl.yres);
    Point* const zbuff = xtoss(Point, sdl.xres);
    int* const moddings = xtoss(int, sdl.yres);
    // For each column of the screen...
    const Display display = xlock(sdl);
    const Line camera = xrotate(hero.fov, hero.theta);
    for(int x = 0; x < sdl.xres; x++)
    {
        // Cast a ray...
        const Point column = xlerp(camera, x / (float) sdl.xres);
        const Ray ray = xcast(hero, map.walling, column, sdl.yres);
        // Render the scanline...
        const Scanline scanline = { sdl, display, x };
        xraster(scanline, ray, wheres, map, hero.torch, moddings);
        // And zbuffer the wall for the sprites
        zbuff[x] = ray.traceline.corrected;
    }
    xunlock(sdl);
    // The scene was rendered on its side for fast caching. Rotate the scene
    churn(sdl);
    // Orient sprite location and theta relative to player
    const Sprites relatives = xorient(sprites, hero);
    // Use the wall zbuffer to render the sprites
    paste(sdl, relatives, zbuff, hero, ticks);
    // Update the screen with the final rendered frame
    present(sdl);
    // Cleanup all local heap allocations
    xkill(relatives);
    free(wheres);
    free(zbuff);
    free(moddings);
}
