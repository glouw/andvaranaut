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
    // Does a 90 degree flip upon copy.
    SDL_RenderCopyEx(sdl.renderer, sdl.canvas, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

void xpresent(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static SDL_Rect clip(const Sdl sdl, const SDL_Rect frame, const Point where, Point* const zbuff)
{
    SDL_Rect seen = frame;
    // Clips sprite from the left.
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= sdl.xres)
            continue;
        if(where.x < zbuff[x].x)
            break;
    }
    // Clips sprite from the right.
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

// Pastes all visible sprites on screen. The wall z-buffer will determine when to partially or fully hide a sprite.
static void paste(const Sdl sdl, const Sprites sprites, Point* const zbuff, const Hero hero, const int ticks)
{
    // Go through all the sprites.
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Move onto the next sprite if this sprite is behind the player.
        if(sprite.where.x < 0) continue;
        // Calculate sprite size - the sprite must be an even integer else the sprite will jitter.
        const int size = xbalance(hero.fov.a.x * sdl.yres / sprite.where.x), half = size / 2;
        // Only shift sprites for the yaw that are grabbed.
        const float shift = sprite.state == GRABBED ? 1.0 : (2.0 - hero.yaw);
        // Calculate sprite location on screen.
        const int mx = sdl.xres / 2;
        const int my = sdl.yres / 2;
        const int l = -half + mx;
        const int t = -half + my * shift;
        const int slider = hero.fov.a.x * (sdl.xres / 2) * xslp(sprite.where);
        const SDL_Rect target = { l + slider, t, size, size };
        // Move onto the next sprite if this sprite is off screen.
        if(target.x + target.w < 0 || target.x >= sdl.xres) continue;
        // Get sprite surface and texture.
        const int selected = sprite.ascii - ' ';
        SDL_Surface* const surface = sdl.surfaces.surface[selected];
        SDL_Texture* const texture = sdl.textures.texture[selected];
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        // Determine sprite animation based on ticks.
        const SDL_Rect image = { w * (ticks % FRAMES), h * sprite.state, w, h };
        // Calculate how much of the sprite is seen.
        volatile const SDL_Rect seen = clip(sdl, target, sprite.where, zbuff);
        // Move onto the next sprite if this totally behind a wall and cannot be seen.
        if(seen.w <= 0) continue;
        // Apply lighting to the sprite.
        const int modding = xilluminate(hero.torch, sprite.where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Apply transperancy to the sprite, if required.
        if(sprite.transparent) SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Renders the sprite.
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        /* Cleanup */
        // Removes transperancy from the sprite.
        if(sprite.transparent) SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // Revert lighting to the sprite.
        SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
    }
}

Sdl xsetup(const Args args)
{
    SDL_Init(SDL_INIT_VIDEO);
    Sdl sdl;
    xzero(sdl);
    sdl.window = SDL_CreateWindow("water", 0, 0, args.xres, args.yres, SDL_WINDOW_SHOWN);
    if(!sdl.window) xbomb("error: could not open window\n");
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1,
        // Hardware acceleration
        SDL_RENDERER_ACCELERATED |
        // Screen Vertical Sync
        (args.vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
    // The canvas texture will be used for per pixel drawings. This will be used to the walls, floors, and ceiling.
    // Notice the flip between yres and xres in the following call for the sdl canvas texture.
    // Notice how ARGB8888 is used for the hardware. This is the fastest option for the GPU.
    sdl.canvas = SDL_CreateTexture(sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, args.yres, args.xres);
    sdl.surfaces = xpull();
    sdl.textures = xcache(sdl.surfaces, sdl.renderer);
    sdl.xres = args.xres;
    sdl.yres = args.yres;
    sdl.fps = args.fps;
    return sdl;
}

void xrelease(const Sdl sdl)
{
    xclean(sdl.surfaces);
    xpurge(sdl.textures);
    SDL_DestroyTexture(sdl.canvas);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
}

void xrender(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const Clouds clouds, const int ticks)
{
    Point* const zbuff = xtoss(Point, sdl.xres);
    // The map, that is, the walls, ceiling, and floor, are drawn by software;
    // for each column of the screen, a ray is cast, and a line projected
    // to the screen is drawn. The projected line is raised for the upper ceiling walls.
    const Display display = xlock(sdl);
    const Line camera = xrotate(hero.fov, hero.theta);
    for(int x = 0; x < sdl.xres; x++)
    {
        const Scanline scanline = { sdl, display, x };
        const Point column = xlerp(camera, x / (float) sdl.xres);
        const Hits hits = xmarch(hero.where, column, map);
        // The ceiing walls are rendered first.
        // Due to the nature of ceiling walls, the sky will show as gaps are left in the ceiling.
        // A sky renderer is required above the furthest ceiling wall; this is simply just the
        // zeroth link the ceiling hits link list.
        // The link list is cleaned up as it is traversed.
        int link = 0;
        for(Hit* hit = hits.ceiling, *next; hit != NULL; next = hit->next, free(hit), hit = next)
        {
            const Hit* const behind = hit;
            const Hit* const before = hit->next;
            Ray a = xcalc(hero, *behind, sdl.yres);
            // Sky renderer.
            if(link++ == 0) xsraster(scanline, a, hero.torch, hero.yaw, clouds);
            // If a ceiling wall is before another ceiling wall,
            // overlay the two to prevent drawing unecessary parts of the behind wall.
            a.proj = xstack(a.proj, sdl.yres);
            if(before)
            {
                Ray b = xcalc(hero, *before, sdl.yres);
                b.proj = xstack(b.proj, sdl.yres);
                a.proj = xoverlay(a.proj, b.proj);
            }
            xwraster(scanline, a, hero.torch);
        }
        // Eye level walls. No linked list is needed as leading
        // eye level wall will always overlap behind eye level walls.
        // Note that this style of rendering will not permit ceiling level walls
        // to be behind eye level walls. The two must either be flush, or the ceiling
        // level walls in front of the eye level walls. Ceiling level walls
        // will artifact if they are behind eye levels walls.
        const Ray ray = xcalc(hero, hits.walling, sdl.yres);
        xwraster(scanline, ray, hero.torch);
        //// The floor and ceiling are renderered here.
        xfraster(scanline, ray, hero.torch, hero.yaw, map);
        xcraster(scanline, ray, hero.torch, hero.yaw, map);
        // A z-buffer is populated on the eye level walls and stored later for the sprite renderer.
        zbuff[x] = ray.traceline.corrected;
    }
    xunlock(sdl);
    // The scene was rendered on its side for fast caching. Rotate the scene.
    churn(sdl);
    // Orient sprite location and theta relative to player.
    const Sprites relatives = xorient(sprites, hero);
    // Use the wall zbuffer to render the sprites.
    paste(sdl, relatives, zbuff, hero, ticks);
    // Cleanup all local heap allocations.
    xkill(relatives);
    free(zbuff);
}

static bool clipping(const Sdl sdl, const Overview ov, const SDL_Rect to)
{
    return (to.x > sdl.xres || to.x < -ov.w)
        && (to.y > sdl.yres || to.y < -ov.h);
}

// Copy over all tiles for the grid layout.
static void gridl(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const int ticks)
{
    // Clear renderer and copy over block overview tiles.
    // The block overview tile will snap to the grid.
    SDL_RenderClear(sdl.renderer);
    for(int j = 0; j < map.rows; j++)
    for(int i = 0; i < map.cols; i++)
    {
        // Walling will default if anything other 1, 2, or 3 is selected.
        const int ascii =
            ov.party == FLORING ? map.floring[j][i] :
            ov.party == CEILING ? map.ceiling[j][i] : map.walling[j][i];
        // If ch is zero, then it is empty space. Skip the render.
        const int ch = ascii - ' ';
        if(ch == 0) continue;
        // Otherwise, render the tile.
        const SDL_Rect to = { ov.w * i + ov.px, ov.h * j + ov.py, ov.w, ov.h };
        if(clipping(sdl, ov, to)) continue;
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[ch], NULL, &to);
    }
    // Put down sprites. Sprites will not snap to the grid.
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int index = sprite->ascii - ' ';
        const int w = sdl.surfaces.surface[index]->w / FRAMES;
        const int h = sdl.surfaces.surface[index]->h / STATES;
        const SDL_Rect from = { w * (ticks % FRAMES), h * sprite->state, w, h };
        const SDL_Rect to = {
            // Right above cursor.
            (int) ((ov.w * sprite->where.x - ov.w / 2) + ov.px),
            (int) ((ov.h * sprite->where.y - ov.h / 1) + ov.py),
            ov.w, ov.h,
        };
        if(clipping(sdl, ov, to)) continue;
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[index], &from, &to);
    }
}

// Copy over the selection tiles.
// This will be done on the top row of the screen.
static void panel(const Sdl sdl, const Overview ov, const int ticks)
{
    for(int i = ov.wheel; i < sdl.textures.count; i++)
    {
        const SDL_Rect to = { ov.w * (i - ov.wheel), 0, ov.w, ov.h };
        // Sprites.
        if(xissprite(i + ' '))
        {
            const int w = sdl.surfaces.surface[i]->w / FRAMES;
            const int h = sdl.surfaces.surface[i]->h / STATES;
            // Copy over the tile. Make animation idle.
            const SDL_Rect from = { w * (ticks % FRAMES), h * IDLE, w, h };
            if(clipping(sdl, ov, to)) continue;
            SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], &from, &to);
        }
        // Blocks.
        else SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], NULL, &to);
    }
}

void xview(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const int ticks)
{
    // All blocks and sprites.
    gridl(sdl, ov, sprites, map, ticks);
    // Block and sprite selection panel.
    panel(sdl, ov, ticks);
}
