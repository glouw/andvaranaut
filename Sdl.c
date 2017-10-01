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
        const int size = balance(focal(hero.fov) * sdl.yres / sprite.where.x);
        // Calculate sprite location on screen
        const int l = (sdl.xres - size) / 2;
        const int t = (sdl.yres - size) / 2;
        const int slider = focal(hero.fov) * (sdl.xres / 2) * slp(sprite.where);
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
        // Note: gcc likes to mess with this SDL_Rect - ban optimizations
        const volatile SDL_Rect seen = clip(sdl, target, sprite.where, zbuff);
        // Move onto the next sprite if this totally behind a wall and cannot be seen
        if(seen.w <= 0)
            continue;
        // Apply lighting to the sprite
        const int modding = illuminate(hero.torch, sprite.where.x);
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

Sdl setup(const int xres, const int yres, const int fps)
{
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    Sdl sdl;
    zero(sdl);
    sdl.window = SDL_CreateWindow("water", 0, 0, xres, yres, SDL_WINDOW_SHOWN);
    if(!sdl.window)
        bomb("error: could not open window\n");
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    sdl.texture = SDL_CreateTexture(sdl.renderer, format, SDL_TEXTUREACCESS_STREAMING, yres, xres); // Notice the flip
    sdl.surfaces = pull(format);
    sdl.textures = cache(sdl.surfaces, sdl.renderer);
    sdl.xres = xres;
    sdl.yres = yres;
    sdl.fps = fps;
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
}

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const int ticks)
{
    // Orient sprite location and theta relative to player
    const Sprites relatives = orient(sprites, hero);
    // Preallocate for render computations
    Point* wheres = toss(Point, sdl.yres);
    int* moddings = toss(int, sdl.yres);
    Point* const zbuff = toss(Point, sdl.xres);
    // Lock the display for manual pixel painting
    const Display display = lock(sdl);
    // For each column of the screen...
    const Line camera = rotate(hero.fov, hero.theta);
    for(int x = 0; x < sdl.xres; x++)
    {
        // Cast a ray along the camera field of fov...
        const Point column = lerp(camera, x / (float) sdl.xres);
        const Ray ray = cast(hero, map.walling, column, sdl.yres);
        // Construct a virtual scanline...
        /* Note that the Scanline struct uses <y> as its struct member and not <x>.
         * Virtual scanlines pertain to renderers and are 90 degrees rotated from screen columns */
        const Scanline scanline = { sdl, display, x };
        // Render the wall...
        wrend(scanline, ray);
        // Render the floor...
        frend(scanline, ray, wheres, map.floring);
        // Render the ceilling...
        crend(scanline, ray, wheres, map.ceiling);
        // Apply lighting to the walls, floor, and ceilling
        light(scanline, ray, wheres, hero.torch, moddings);
        // The columns yield the corrected tracelines of the rays. Save these for
        // the sprite zbuffer to know when to cover a sprite with wall edges
        zbuff[x] = ray.traceline.corrected;
    }
    // Pixel painting is now done - unlock the display
    unlock(sdl);
    // The scene was rendered on its side for cache effeciency. Rotate the scene
    churn(sdl);
    // Use the wall zbuffer to render the sprites
    paste(sdl, relatives, zbuff, hero, ticks);
    // Update the screen with the final rendered frame
    present(sdl);
    // Cleanup all local heap allocations
    kill(relatives);
    free(wheres);
    free(moddings);
    free(zbuff);
}
