#include "Sdl.h"

#include "Frame.h"
#include "Scanline.h"
#include "Bundle.h"
#include "util.h"

// Assumes renderer is rotated 90 degrees.
static void churn(const Sdl sdl)
{
    const SDL_Rect dst = {
        (sdl.xres - sdl.yres) / 2,
        (sdl.yres - sdl.xres) / 2,
        sdl.yres,
        sdl.xres
    };
    SDL_RenderCopyEx(sdl.renderer, sdl.canvas, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

// Presents the entire renderer to the screen.
void xpresent(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

// Clips a sprite, left and right, based on zbuffer.
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
        Sprite* const sprite = &sprites.sprite[which];
        // Move onto the next sprite if this sprite is behind the player.
        if(sprite->where.x < 0)
            continue;
        // Calculate sprite size - the sprite must be an even integer else the sprite will jitter.
        const float focal = hero.fov.a.x;
        const int size = (focal * sdl.xres / 2.0f) / sprite->where.x;
        const int osize = xodd(size) ? size + 1 : size;
        // Calculate sprite location on screen. Account for hero yaw and height.
        const int my = sdl.yres / 2 * (sprite->state == GRABBED ? 1.0f : (2.0f - hero.yaw));
        const int mx = sdl.xres / 2;
        const int l = mx - osize / 2;
        const int t = my - osize * (sprite->state == GRABBED ? 0.5f : (1.0f - hero.height));
        const int s = hero.fov.a.x * (sdl.xres / 2) * xslp(sprite->where);
        const SDL_Rect target = { l + s, t, osize, osize };
        // Move onto the next sprite if this sprite is off screen.
        if(target.x + target.w < 0 || target.x >= sdl.xres)
            continue;
        // Get sprite surface and texture.
        const int selected = sprite->ascii - ' ';
        SDL_Surface* const surface = sdl.surfaces.surface[selected];
        SDL_Texture* const texture = sdl.textures.texture[selected];
        const int w = surface->w / FRAMES;
        const int h = surface->h / STATES;
        // Determine sprite animation based on ticks.
        const SDL_Rect image = { w * (ticks % FRAMES), h * sprite->state, w, h };
        // Calculate how much of the sprite is seen.
        /* NEEDS to be volatile else it exits without segfault on my Thinkpad t43 with
         * either x or y 255 overflow. Assuming integrated graphics driver bug. */
        volatile const SDL_Rect seen = clip(sdl, target, sprite->where, zbuff);
        // Move onto the next sprite if this totally behind a wall and cannot be seen.
        if(seen.w <= 0)
            continue;
        // Apply lighting to the sprite.
        const int modding = xilluminate(hero.torch, sprite->where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Apply transperancy to the sprite, if required.
        if(sprite->transparent) SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Render the sprite.
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        /* Cleanup */
        // Remove transperancy from the sprite.
        if(sprite->transparent) SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // Revert lighting to the sprite.
        SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
    }
}

Sdl xsetup(const Args args)
{
    SDL_Init(SDL_INIT_VIDEO);
    Sdl sdl;
    xzero(sdl);
    sdl.window = SDL_CreateWindow("Andvaranaut",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, args.xres, args.yres, SDL_WINDOW_SHOWN);
    if(sdl.window == NULL)
        xbomb("error: could not open window\n");
    sdl.renderer = SDL_CreateRenderer(
        sdl.window,
        -1,
        // Hardware acceleration.
        SDL_RENDERER_ACCELERATED |
        // Screen Vertical Sync on / off.
        (args.vsync ? SDL_RENDERER_PRESENTVSYNC : 0x0));
    // The canvas texture will be used for per pixel drawings. This will be used to the walls, floors, and ceiling.
    // Notice the flip between yres and xres in the following call for the sdl canvas texture.
    // This was done for fast caching. Upon presenting the canvas will be rotated upwards by 90 degrees.
    // Notice how ARGB8888 is used for the hardware. This is the fastest option for the GPU.
    sdl.canvas = SDL_CreateTexture(sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, args.yres, args.xres);
    sdl.surfaces = xpull();
    sdl.textures = xcache(sdl.surfaces, sdl.renderer);
    sdl.xres = args.xres;
    sdl.yres = args.yres;
    sdl.fps = args.fps;
    sdl.threads = args.threads;
    return sdl;
}

// Cleanup at program exit.
void xrelease(const Sdl sdl)
{
    xclean(sdl.surfaces);
    xpurge(sdl.textures);
    SDL_DestroyTexture(sdl.canvas);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
}

void xrender(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const Current current, const int ticks)
{
    // Z-buffer will be populated once the map renderering is finished.
    Point* const zbuff = xtoss(Point, sdl.xres);
    // The display must be locked for per-pixel writes.
    const Display display = xlock(sdl);
    const Line camera = xrotate(hero.fov, hero.theta);
    // Rendering bundles are used for rendering a
    // portion of the map (ceiling, walls, and flooring) to the display.
    // One thread per CPU is allocated.
    Bundle* const b = xtoss(Bundle, sdl.threads);
    for(int i = 0; i < sdl.threads; i++)
    {
        b[i].a = (i + 0) * sdl.xres / sdl.threads;
        b[i].b = (i + 1) * sdl.xres / sdl.threads;
        b[i].zbuff = zbuff;
        b[i].camera = camera;
        b[i].display = display;
        b[i].sdl = sdl;
        b[i].hero = hero;
        b[i].current = current;
        b[i].map = map;
    };
    // Launch all threads and wait for their completion.
    SDL_Thread** const threads = xtoss(SDL_Thread*, sdl.threads);
    for(int i = 0; i < sdl.threads; i++)
        threads[i] = SDL_CreateThread(xbraster, "n/a", &b[i]);
    for(int i = 0; i < sdl.threads; i++)
    {
        int status; /* Ignored */
        SDL_WaitThread(threads[i], &status);
    }
    // Per-pixel writes are done. Unlock the display.
    xunlock(sdl);
    // The map was rendered on its side. Rotate the map upwards.
    churn(sdl);
    // Sort the sprites furthest to nearest.
    const Sprites relatives = xorient(sprites, hero);
    // Use the zbuffer to render the sprites.
    // The furthest sprites are pasted first such that the closer sprites overlap.
    paste(sdl, relatives, zbuff, hero, ticks);
    // Tidy up the heap.
    xkill(relatives);
    free(zbuff);
    free(b);
    free(threads);
}

// Returns true if a tile is clipped off the screen.
static int clipping(const Sdl sdl, const Overview ov, const SDL_Rect to)
{
    return (to.x > sdl.xres || to.x < -ov.w)
        && (to.y > sdl.yres || to.y < -ov.h);
}

// Draws all power gauge squares.
// As a trained swordsman your sword will always return to the center of the screen.
void xdgauge(const Sdl sdl, const Gauge g)
{
    for(int i = 0; i < g.count; i++)
    {
        const float growth = i / (float) g.count;
        const float scale = 28.0; // Arbitrary.
        const int width = growth * xmin(sdl.xres, sdl.yres) / scale;
        const int color = growth * 0xFF;
        const float sens = 2.33;
        const int x = g.points[i].x * sens - (width - sdl.xres) / 2;
        const int y = g.points[i].y * sens - (width - sdl.yres) / 2;
        const SDL_Rect square = { x, y, width, width };
        SDL_SetRenderDrawColor(sdl.renderer, 0xFF, color, 0x00, 0xFF);
        SDL_RenderFillRect(sdl.renderer, &square);
    }
}

// Animates first the liquid then glass for a bar.
static void dmeter(const Sdl sdl, const int ticks, const Meter m)
{
    // Bars start at this column on GUI.bmp.
    const int col = 6;
    // Tile <from> size is 16x16.
    const int fs = 16;
    // Will flicker if less than 25% for a stat.
    const int frame = m.stat < m.max / 4.0f ?  (ticks % FRAMES ? ']' : '[' ) - ' ' : '[' - ' ';
    // Tile <to> size is resolution dependent.
    const int ts = sdl.xres / 30;
    /* Draw liquid in glass. */
    for(int i = 0; i < xcl(m.stat); i++)
    {
        const SDL_Rect fm = {
            // Column.
            i == xfl(m.stat) && (xdec(m.stat) < 0.25f) ? (col + 3) * fs :
            i == xfl(m.stat) && (xdec(m.stat) < 0.50f) ? (col + 2) * fs :
            i == xfl(m.stat) && (xdec(m.stat) < 0.75f) ? (col + 1) * fs :
            (col + 0) * fs,
            // Row.
            m.bar * fs,
            // Dimension
            fs, fs
        };
        const SDL_Rect to = { i * ts, sdl.yres - m.bar * ts, ts, ts };
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[frame], &fm, &to);
    }
    /* Draw glass. */
    for(int i = 0; i < m.max; i++)
    {
        const int final = m.max - 1;
        const int first = 0;
        const SDL_Rect fm = {
            // Column.
            i == first ? (col + 0) * fs : // Left end of glass bar.
            i == final ? (col + 2) * fs : // Right end of glass bar.
            (col + 1) * fs, // Anywhere in the middle of the glass bar.
            // Row.
            0,
            // Dimension.
            fs, fs
        };
        const SDL_Rect to = { i * ts, sdl.yres - m.bar * ts, ts, ts };
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[frame], &fm, &to);
    }
}

// Draw health, mana, and fatigue meters.
void xdmeters(const Sdl sdl, const Hero hero, const int ticks)
{
    const Meter meters[] = {
        { hero.hlth, hero.maxhlth, HEALTH  },
        { hero.mana, hero.maxmana, MANA    },
        { hero.fatg, hero.maxfatg, FATIGUE },
    };
    for(int i = 0; i < xlen(meters); i++) dmeter(sdl, ticks, meters[i]);
}

// Draw tiles for the grid layout.
static void dgridl(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const int ticks)
{
    // Clear renderer and draw overview tiles.
    SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sdl.renderer);
    for(int j = 0; j < map.rows; j++)
    for(int i = 0; i < map.cols; i++)
    {
        // Walling will default if anything other 1, 2, or 3 is selected.
        const int ascii =
            ov.party == FLORING ? map.floring[j][i] :
            ov.party == CEILING ? map.ceiling[j][i] : map.walling[j][i];
        // If empty space then skip the tile.
        const int ch = ascii - ' ';
        if(ch == 0)
            continue;
        // Otherwise render the tile.
        const SDL_Rect to = { ov.w * i + ov.px, ov.h * j + ov.py, ov.w, ov.h };
        if(clipping(sdl, ov, to))
            continue;
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
        if(clipping(sdl, ov, to))
            continue;
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[index], &from, &to);
    }
}

// Draw the selection panel.
static void dpanel(const Sdl sdl, const Overview ov, const int ticks)
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
            if(clipping(sdl, ov, to))
                continue;
            SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], &from, &to);
        }
        // Blocks.
        else SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], NULL, &to);
    }
}

void xview(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const int ticks)
{
    dgridl(sdl, ov, sprites, map, ticks);
    dpanel(sdl, ov, ticks);
}
