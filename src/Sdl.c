#include "Sdl.h"

#include "Bar.h"
#include "Frame.h"
#include "Scanline.h"
#include "Bundle.h"
#include "util.h"

Sdl xzsdl()
{
    static Sdl sdl;
    return sdl;
}

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
        const int my = sdl.yres / 2 * (sprite->state == LIFTED ? 1.0f : (2.0f - hero.yaw));
        const int mx = sdl.xres / 2;
        const int l = mx - osize / 2;
        const int t = my - osize * (sprite->state == LIFTED ? 0.5f : (1.0f - hero.height));
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
        // NEEDS to be volatile else it exits without segfault on my Thinkpad t43 with
        // either x or y 255 overflow. Assuming integrated graphics driver bug.
        volatile const SDL_Rect seen = clip(sdl, target, sprite->where, zbuff);
        // Move onto the next sprite if this totally behind a wall and cannot be seen.
        if(seen.w <= 0)
            continue;
        // Apply lighting to the sprite.
        const int modding = xilluminate(hero.torch, sprite->where.x);
        SDL_SetTextureColorMod(texture, modding, modding, modding);
        // Apply transperancy to the sprite, if required.
        if(sprite->transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        // Render the sprite.
        SDL_RenderSetClipRect(sdl.renderer, (SDL_Rect*) &seen);
        SDL_RenderCopy(sdl.renderer, texture, &image, &target);
        SDL_RenderSetClipRect(sdl.renderer, NULL);
        // Remove transperancy from the sprite.
        if(sprite->transparent)
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // Revert lighting to the sprite.
        SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
    }
}

Sdl xsetup(const Args args)
{
    SDL_Init(SDL_INIT_VIDEO);
    Sdl sdl = xzsdl();
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
    sdl.key = 0x00FFFF;
    sdl.xres = args.xres;
    sdl.yres = args.yres;
    sdl.fps = args.fps;
    sdl.threads = args.threads;
    sdl.surfaces = xpull(sdl.key);
    sdl.textures = xcache(sdl.surfaces, sdl.renderer);
    // GUI surfaces start at this index of the surfaces.
    sdl.gui = '~' - ' ' + 25;
    sdl.wht = 0xFFDFEFD7;
    sdl.blk = 0xFF000000;
    sdl.red = 0xFFD34549;
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

void xrender(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map, const Flow current, const Flow clouds, const int ticks)
{
    // Z-buffer will be populated once the map renderering is finished.
    Point* const zbuff = xtoss(Point, sdl.xres);
    // The display must be locked for per-pixel writes.
    void* screen;
    int pitch;
    SDL_LockTexture(sdl.canvas, NULL, &screen, &pitch);
    const int width = pitch / sizeof(uint32_t);
    uint32_t* pixels = (uint32_t*) screen;
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
        b[i].pixels = pixels;
        b[i].width = width;
        b[i].sdl = sdl;
        b[i].hero = hero;
        b[i].current = current;
        b[i].clouds = clouds;
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
    SDL_UnlockTexture(sdl.canvas);
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

static void drect(const Sdl sdl, const int x, const int y, const int width, const int color, const int filled)
{
    const int a = (color >> 24) & 0xFF;
    const int r = (color >> 16) & 0xFF;
    const int g = (color >>  8) & 0xFF;
    const int b = (color >>  0) & 0xFF;
    const SDL_Rect square = { x, y, width, width };
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
    filled ?
        SDL_RenderFillRect(sdl.renderer, &square):
        SDL_RenderDrawRect(sdl.renderer, &square);
}

// Draws melee gauge.
static Attack dgmelee(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    // Animate attack.
    for(int i = 0; i < g.count; i++)
    {
        const float growth = i / (float) g.count;
        const int width = growth * 12; // Hard coded size.
        const int x = g.points[i].x * sens - (width - sdl.xres) / 2;
        const int y = g.points[i].y * sens - (width - sdl.yres) / 2;
        drect(sdl, x, y, width, sdl.wht, true);
    }
    // Calculate attack.
    const int last = g.count - 1;
    const int tail = 6;
    if(g.count < tail)
        return xzattack();
    float mag = 0.0f;
    for(int i = 0; i < g.count - 1; i++)
        mag += xmag(xsub(g.points[i + 1], g.points[i + 0]));
    mag += it.damage;
    // Hurts is a melee property. For instance, more than one
    // enemy can be hurt when a warhammer is used.
    const Point dir = xsub(g.points[last], g.points[last - tail]);
    const Attack melee = { mag, xunt(dir), it.hurts, MELEE, 0 };
    return xmag(dir) > 0.0f ? melee : xzattack();
}

// Draws ranged gauge.
static Attack dgrange(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    if(g.count > 0)
    {
        // Animate attack. Both amplitude and attack are bow properties.
        // A weaker bow may have longer period and amplitude, for instance.
        // A longbow may have long period and amplitude but huge attack.
        // A shortbow may have short period and amplitude and medium attack.
        const float steady = it.amplitude / 2.0f;
        const float width = it.amplitude * xsinc(g.count, it.period) + steady;
        // Hurts is also a bow property. Longbows, for instance, can hurt more than one sprite.
        const int x = g.points[g.count - 1].x * sens - (width - sdl.xres) / 2;
        const int y = g.points[g.count - 1].y * sens - (width - sdl.yres) / 2;
        drect(sdl, x, y, width, sdl.wht, false);
        // Calculate attack.
        const float mag = 1.0f / (steady - width);
        // Animation direction is constant.
        const Point dir = { 0.0f, -1.0f };
        const Attack range = { mag, dir, it.hurts, RANGE, 0 };
        return range;
    }
    return xzattack();
}

// Draws magic gauge.
static Attack dgmagic(const Sdl sdl, const Gauge g, const Item it, const float sens, const Inventory inv, const Scroll sc)
{
    if(g.count > 0)
    {
        const int size = 4; // Must come from Scroll.[ch].
        const int grid = 64;
        const Point middle = { sdl.xres / 2, sdl.yres / 2, };
        const Point shift = { grid / 2, grid / 2, };
        // Animate attack (inside square for mouse cursor).
        for(int i = 0; i < g.count; i++)
        {
            // Must populate Scroll int array.
            const Point point = xadd(xmul(g.points[i], sens), shift);
            const Point corner = xsnap(point, grid);
            const Point center = xsub(xadd(corner, middle), shift);
            drect(sdl, center.x, center.y, grid, sdl.wht, true);
        }
        // Animate attack (grid squares).
        for(int x = -size; x <= size; x++)
        for(int y = -size; y <= size; y++)
        {
            const Point which = { x, y };
            const Point corner = xmul(which, grid);
            const Point center = xsub(xadd(corner, middle), shift);
            drect(sdl, center.x, center.y, grid, sdl.red, false);
        }
        // Draw the cursor.
        const Point point = xadd(xmul(g.points[g.count - 1], sens), shift);
        const Point center = xsub(xadd(point, middle), shift);
        drect(sdl, center.x, center.y, 6, sdl.red, true);
    }
    // Calculate attack.
    // Runs through scroll int array and checks for error with all scroll int array shapes.
    const float mag = 0.0f; // Error rate.
    const Point dir = { 0.0f, 0.0f }; // Maybe randomize.
    // The magic scroll closest to the drawn gauge shape is calculated in the attack shape.
    const int scindex = 0;
    const Attack magic = { mag, dir, 0, MAGIC, scindex };
    return magic;
}

// Draws all power gauge squares.
Attack xdgauge(const Sdl sdl, const Gauge g, const Item it, const Inventory inv, const Scroll sc)
{
    const float sens = 2.33;
    return
        xismelee(it.c) ? dgmelee(sdl, g, it, sens) :
        xisrange(it.c) ? dgrange(sdl, g, it, sens) :
        xismagic(it.c) ? dgmagic(sdl, g, it, sens, inv, sc) :
        xzattack();
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
    for(int i = ov.wheel; i <= '~' - ' '; i++)
    {
        const SDL_Rect to = { ov.w * (i - ov.wheel), 0, ov.w, ov.h };
        // Sprites.
        if(xsissprite(i + ' '))
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

// Draws one status bar.
void xdbar(const Sdl sdl, const Hero hero, const int position, const int ticks, const int size, const Bar bar)
{
    const int max =
        bar == HPS ? hero.hpsmax :
        bar == FTG ? hero.ftgmax : hero.mnamax;
    const float lvl =
        bar == HPS ? hero.hps :
        bar == FTG ? hero.ftg : hero.mna;
    const float threshold = 0.25f * max;
    SDL_Texture* const texture = sdl.textures.texture[sdl.gui];
    SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui];
    const int frame = ticks % 2 == 0;
    const int w = surface->w;
    const SDL_Rect gleft = { 0,  0, w, w };
    const SDL_Rect glass = { 0, 32, w, w };
    const SDL_Rect grite = { 0, 64, w, w };
    // Will animate bar if below threshold.
    const SDL_Rect fluid = { 0, (int) bar + (lvl < threshold ? w * frame : 0), w, w };
    const SDL_Rect empty[] = {
        { 0, fluid.y + 2 * w, w, w }, // 75%.
        { 0, fluid.y + 4 * w, w, w }, // 50%.
        { 0, fluid.y + 6 * w, w, w }, // 25%.
    };
    // Draw.
    for(int i = 0; i < max; i++)
    {
        const int ww = size * w;
        const int yy = sdl.yres - ww * (1 + position);
        const SDL_Rect to = { ww * i, yy, ww, ww };
        // Full fluid level.
        if(xfl(lvl) > i)
            SDL_RenderCopy(sdl.renderer, texture, &fluid, &to);
        // Partial fluid level.
        if(xfl(lvl) == i)
            xdec(lvl) > 0.75f ? SDL_RenderCopy(sdl.renderer, texture, &empty[0], &to) :
            xdec(lvl) > 0.50f ? SDL_RenderCopy(sdl.renderer, texture, &empty[1], &to) :
            xdec(lvl) > 0.25f ? SDL_RenderCopy(sdl.renderer, texture, &empty[2], &to) : 0;
        // Glass.
        SDL_RenderCopy(sdl.renderer, texture, i == 0 ? &gleft : i == max - 1 ? &grite : &glass, &to);
    }
}

// Draws hero status bars.
void xdbars(const Sdl sdl, const Hero hero, const int ticks)
{
    const int size = 1;
    xdbar(sdl, hero, 2, ticks, size, HPS);
    xdbar(sdl, hero, 1, ticks, size, MNA);
    xdbar(sdl, hero, 0, ticks, size, FTG);
}

// Draws the inventory backpanel. Selected inventory item is highlighted.
static void dinvbp(const Sdl sdl, const Inventory inv)
{
    const Point whtbp = { 0.0, 512.0 };
    const Point redbp = { 0.0, 528.0 };
    for(int i = 0; i < inv.items.max; i++)
    {
        SDL_Texture* const texture = sdl.textures.texture[sdl.gui];
        SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui];
        const int w = surface->w;
        const int xx = sdl.xres - inv.w;
        const SDL_Rect from = {
            (int) (i == inv.selected ? redbp.x : whtbp.x),
            (int) (i == inv.selected ? redbp.y : whtbp.y),
            w, w
        };
        const SDL_Rect to = { xx, inv.w * i, inv.w, inv.w };
        SDL_RenderCopy(sdl.renderer, texture, &from, &to);
    }
}

// Draws the inventory items.
static void dinvits(const Sdl sdl, const Inventory inv)
{
    for(int i = 0; i < inv.items.max; i++)
    {
        const Item item = inv.items.item[i];
        if(item.c == NONE)
            continue;
        const int index = xcindex(item.c);
        SDL_Texture* const texture = sdl.textures.texture[index];
        SDL_Surface* const surface = sdl.surfaces.surface[index];
        const int w = surface->w;
        const int xx = sdl.xres - inv.w;
        const SDL_Rect from = { 0, w * item.index, w, w }, to = { xx, inv.w * i, inv.w, inv.w };
        SDL_RenderCopy(sdl.renderer, texture, &from, &to);
    }
}

void xdinv(const Sdl sdl, const Inventory inv)
{
    dinvbp(sdl, inv);
    dinvits(sdl, inv);
}

static void drooms(uint32_t* pixels, const int width, const Map map, const uint32_t in, const uint32_t out)
{
    for(int y = 1; y < map.rows - 1; y++)
    for(int x = 1; x < map.cols - 1; x++)
    {
        if(map.walling[y][x] != ' ' && map.walling[y][x + 1] == ' ') pixels[x + y * width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y][x - 1] == ' ') pixels[x + y * width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y + 1][x] == ' ') pixels[x + y * width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y - 1][x] == ' ') pixels[x + y * width] = out;
        if(map.walling[y][x] == ' ')
            pixels[x + y * width] = in;
    }
}

// Like drect, but draws a dot with per pixel access. Does not use SDL_Rect.
static void ddot(uint32_t* pixels, const int width, const Point where, const int size, const uint32_t in, const uint32_t out)
{
    for(int y = -size; y <= size; y++)
    for(int x = -size; x <= size; x++)
    {
        const int xx = x + where.x;
        const int yy = y + where.y;
        pixels[xx + yy * width] = in;
        if(x == -size) pixels[xx + yy * width] = out;
        if(x == +size) pixels[xx + yy * width] = out;
        if(y == -size) pixels[xx + yy * width] = out;
        if(y == +size) pixels[xx + yy * width] = out;
    }
}

void xdmap(const Sdl sdl, const Map map, const Point where)
{
    // This map texture is not apart of the Sdl struct since it must be refreshed
    // each frame via creation and destruction.
    SDL_Texture* const texture = SDL_CreateTexture(
        sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, map.cols, map.rows);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // Lock.
    void* screen;
    int pitch;
    SDL_LockTexture(texture, NULL, &screen, &pitch);
    const int width = pitch / sizeof(uint32_t);
    uint32_t* pixels = (uint32_t*) screen;
    // Draw rooms and hero dot.
    drooms(pixels, width, map, sdl.wht, sdl.blk);
    ddot(pixels, width, where, 3, sdl.red, sdl.blk);
    // Unlock and send.
    SDL_UnlockTexture(texture);
    const SDL_Rect dst = { 0, 0, map.cols, map.rows };
    SDL_RenderCopy(sdl.renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}
