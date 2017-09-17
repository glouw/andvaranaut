#include "Sdl.h"

#include "Frame.h"
#include "Scanline.h"
#include "util.h"

static void churn(const Sdl sdl)
{
    SDL_RenderCopyEx(sdl.renderer, sdl.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

static void present(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static SDL_Rect clip(const SDL_Rect frame, const Point where, const int res, Point* const zbuff)
{
    SDL_Rect seen = frame;
    // Clips sprite from the left
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= res)
            continue;
        if(where.x < zbuff[x].x)
            break;
    }
    // Clips sprite from the right
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= res)
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
        const int size = balance(focal(hero.fov) * sdl.res / sprite.where.x);
        // Calculate sprite location on screen
        const int corner = (sdl.res - size) / 2;
        const int slider = (sdl.res / 2) * ratio(hero.fov) * slp(sprite.where);
        const SDL_Rect target = { corner + slider, corner, size, size };
        // Move onto the next sprite if this sprite is off screen
        if(target.x + target.w < 0 || target.x >= sdl.res)
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
        const volatile SDL_Rect seen = clip(target, sprite.where, sdl.res, zbuff);
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

Sdl setup(const int res, const int fps)
{
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    Sdl sdl;
    zero(sdl);
    sdl.window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(!sdl.window)
        bomb("error: could not open window\n");
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
    sdl.texture = SDL_CreateTexture(sdl.renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    sdl.surfaces = pull(format);
    sdl.textures = cache(sdl.surfaces, sdl.renderer);
    sdl.res = res;
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
    // Arrange sprite location relative to player
    const Sprites relatives = arrange(sprites, hero);
    // Preallocate for render computations
    Point* wheres = toss(Point, sdl.res);
    int* moddings = toss(int, sdl.res);
    Point* const zbuff = toss(Point, sdl.res);
    // Lock the display for manual pixel painting
    const Display display = lock(sdl);
    // For each horizontal scanline of the screen...
    const Line camera = rotate(hero.fov, hero.theta);
    for(int y = 0; y < sdl.res; y++)
    {
        // Cast a ray...
        const Point column = lerp(camera, y / (float) sdl.res);
        const Ray ray = cast(hero, map.walling, column, sdl.res);
        const Scanline scanline = { sdl, display, y };
        // Render the wall...
        wrend(scanline, ray);
        // Render the floor...
        frend(scanline, ray, wheres, map);
        // Render the ceilling...
        crend(scanline, ray, wheres, map);
        // Apply lighting to the walls, floor, and ceilling...
        light(scanline, ray, wheres, hero.torch, moddings);
        // Save the rays from walls (corrected to the screen) for the sprite zbuffer
        zbuff[y] = ray.traceline.corrected;
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
