#include "Sdl.h"

#include "Bar.h"
#include "Frame.h"
#include "Scanline.h"
#include "Bundle.h"
#include "util.h"

Sdl xzsdl(void)
{
    static Sdl sdl;
    return sdl;
}

static void churn(const Sdl sdl)
{
    const SDL_Rect dst = {
        (sdl.xres - sdl.yres) / 2,
        (sdl.yres - sdl.xres) / 2,
        sdl.yres,
        sdl.xres,
    };
    SDL_RenderCopyEx(sdl.renderer, sdl.canvas, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

void xpresent(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

// Left clip a sprite based on zbuffer.
static SDL_Rect lclip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
{
    for(; seen.w > 0; seen.w--, seen.x++)
    {
        const int x = seen.x;
        if(x < 0 || x >= xres)
            continue;
        if(where.x < zbuff[x].x)
            break;
    }
    return seen;
}

// Right clip a sprite based on zbuffer.
static SDL_Rect rclip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
{
    for(; seen.w > 0; seen.w--)
    {
        const int x = seen.x + seen.w;
        if(x < 0 || x >= xres)
            continue;
        if(where.x < zbuff[x].x)
        {
            seen.w = seen.w + 1;
            break;
        }
    }
    return seen;
}

// Clips a sprite left and right based on wall zbuffer.
static SDL_Rect clip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
{
    seen = lclip(seen, xres, where, zbuff);
    return rclip(seen, xres, where, zbuff);
}

static void dbox(const Sdl sdl, const int x, const int y, const int width, const uint32_t color, const int filled)
{
    const int a = (color >> 0x18) & 0xFF;
    const int r = (color >> 0x10) & 0xFF;
    const int g = (color >> 0x08) & 0xFF;
    const int b = (color >> 0x00) & 0xFF;
    const SDL_Rect square = { x, y, width, width };
    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
    filled ?
        SDL_RenderFillRect(sdl.renderer, &square):
        SDL_RenderDrawRect(sdl.renderer, &square);
}

// Renders sprite speech text box.
static void rspeech(Sprite* const sprite, const Sdl sdl, const Text text, const SDL_Rect target, const Timer tm)
{
    const int ticks = tm.ticks - sprite->speech.ticks;
    const int index = (ticks / 8) % sprite->speech.count;
    const char* const sentence = sprite->speech.sentences[index];
    xfputsq(text.fill, text.line, sentence, 0xFF, sdl.renderer, target);
}

// Calculates sprite size releative to player.
static SDL_Rect rtarget(const Sdl sdl, Sprite* const sprite, const Hero hero)
{
    // Projection.[ch] does the same thing, but this one accounts for sprite jitter.
    const int size = sprite->size * hero.fov.a.x * 0.5f * sdl.xres / sprite->where.x;
    const int osize = xodd(size) ? size + 1 : size;
    const int my = 0.5f * sdl.yres * (2.0f - hero.yaw);
    const int mx = 0.5f * sdl.xres;
    const int l = mx - osize * 0.5f;
    const int t = my - osize * (1.0f - hero.height / sprite->size);
    const int s = hero.fov.a.x * mx * xslp(sprite->where);
    const SDL_Rect target = { l + s, t, osize, osize };
    return target;
}

// Calculates which sprite frame and state to use based on sprite state and timer clock.
static SDL_Rect rimage(SDL_Surface* const surface, const State state, const Timer tm)
{
    const int w = surface->w / FRAMES;
    const int h = surface->h / STATES;
    const SDL_Rect image = { w * xtmhi(tm), h * state, w, h };
    return image;
}

// Renders one sprite to screen.
static void rsprite(const Sdl sdl, const Text text, Sprite* const sprite, const Hero hero, SDL_Texture* const texture, const SDL_Rect image, const SDL_Rect target, const Timer tm)
{
    // Apply lighting to the sprite.
    const int modding = xilluminate(hero.torch, sprite->where.x);
    SDL_SetTextureColorMod(texture, modding, modding, modding);

    // Apply transparency to the sprite, if required.
    if(sprite->transparent)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

    // Render the sprite.
    SDL_RenderSetClipRect(sdl.renderer, &sprite->seen);
    SDL_RenderCopy(sdl.renderer, texture, &image, &target);
    SDL_RenderSetClipRect(sdl.renderer, NULL);

    // Remove transperancy from the sprite.
    if(sprite->transparent)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Remove lighting from the sprite.
    SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);

    // If the sprite is within earshot of hero then render speech sentences.
    if(!xisdead(sprite->state) && !xismute(sprite))
        xeql(xadd(hero.where, sprite->where), hero.where, hero.aura) ? rspeech(sprite, sdl, text, target, tm) : xstick(sprite, tm);
}

// Prepares one sprite render to screen.
static void rsetup(const Sdl sdl, const Text text, Sprite* const sprite, Point* zbuff, const Hero hero, const Timer tm)
{
    if(sprite->where.x > 0)
    {
        const SDL_Rect target = rtarget(sdl, sprite, hero);
        if(target.x + target.w >= 0 && target.x < sdl.xres)
        {
            const int selected = sprite->ascii - ' ';
            SDL_Surface* const surface = sdl.surfaces.surface[selected];
            SDL_Texture* const texture = sdl.textures.texture[selected];
            const SDL_Rect image = rimage(surface, sprite->state, tm);
            sprite->seen = clip(target, sdl.xres, sprite->where, zbuff);
            if(sprite->seen.w > 0)
                rsprite(sdl, text, sprite, hero, texture, image, target, tm);
        }
    }
}

// Renders all sprites to screen.
static void rsprites(const Sdl sdl, const Text text, const Sprites sprites, Point* const zbuff, const Hero hero, const Timer tm)
{
    for(int which = 0; which < sprites.count; which++)
        rsetup(sdl, text, &sprites.sprite[which], zbuff, hero, tm);
}

Sdl xsetup(const Args args)
{
    SDL_Init(SDL_INIT_VIDEO);
    Sdl sdl = xzsdl();

    sdl.window = SDL_CreateWindow(
        "Andvaranaut",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        args.xres,
        args.yres,
        SDL_WINDOW_SHOWN);

    if(sdl.window == NULL)
        xbomb("error: could not open window\n");

    sdl.renderer = SDL_CreateRenderer(
        sdl.window,
        -1,
        SDL_RENDERER_ACCELERATED |
        (args.vsync ? SDL_RENDERER_PRESENTVSYNC : 0x0));

    sdl.canvas = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        // XRES and YRES swapped since rendering is done 90 degrees on side.
        args.yres, args.xres);

    sdl.xres = args.xres;
    sdl.yres = args.yres;
    sdl.fps = args.fps;
    sdl.threads = args.threads;
    sdl.surfaces = xpull();
    sdl.textures = xcache(sdl.surfaces, sdl.renderer);
    sdl.gui = '~' - ' ' + 25;
    sdl.wht = 0xFFDFEFD7;
    sdl.blk = 0xFF000000;
    sdl.red = 0xFFD34549;
    sdl.yel = 0xFFDBD75D;

    return sdl;
}

void xrender(const Sdl sdl, const Text text, const Hero hero, const Sprites sprites, const Map map, const Flow current, const Flow clouds, const Timer tm)
{
    Point* const zbuff = xtoss(Point, sdl.xres);
    const Line camera = xrotate(hero.fov, hero.theta);
    const Vram vram = xvlock(sdl.canvas);

    // Threaded software rendering.
    // Each thread handles vertical columns <a> to <b> of the screen.
    Bundle* const b = xtoss(Bundle, sdl.threads);
    for(int i = 0; i < sdl.threads; i++)
    {
        b[i].a = (i + 0) * sdl.xres / sdl.threads;
        b[i].b = (i + 1) * sdl.xres / sdl.threads;
        b[i].zbuff = zbuff;
        b[i].camera = camera;
        b[i].vram = vram;
        b[i].sdl = sdl;
        b[i].hero = hero;
        b[i].current = current;
        b[i].clouds = clouds;
        b[i].map = map;
    };
    SDL_Thread** const threads = xtoss(SDL_Thread*, sdl.threads);
    for(int i = 0; i < sdl.threads; i++)
        threads[i] = SDL_CreateThread(xbraster, "n/a", &b[i]);
    for(int i = 0; i < sdl.threads; i++)
    {
        int status; // Ignored.
        SDL_WaitThread(threads[i], &status);
    }
    xvunlock(sdl.canvas);

    // Render was done sideways for cache efficiency. Rotate upwards.
    churn(sdl);

    // Orientate sprites to player's gaze and render to screen. Place back to global coords afterwards.
    xorient(sprites, hero);
    rsprites(sdl, text, sprites, zbuff, hero, tm);
    xplback(sprites, hero);

    free(zbuff);
    free(b);
    free(threads);
}

// Draws melee gauge and calculates attack.
static Attack dgmelee(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    for(int i = 0; i < g.count; i++)
    {
        const float growth = i / (float) g.count;
        const int size = 16;
        const int width = growth * size;
        const int green = 0xFF * growth;
        const uint32_t color = (0xFF << 0x10) | (green << 0x08);
        const Point mid = {
            (width - sdl.xres) / 2.0f,
            (width - sdl.yres) / 2.0f,
        };
        const Point where = xsub(xmul(g.points[i], sens), mid);
        dbox(sdl, where.x, where.y, width, color, true);
    }

    const int tail = 10;
    if(g.count < tail)
        return xzattack();

    const float mag = xgmag(g, it.damage);

    const int last = g.count - 1;
    const int first = g.count - tail;
    const Point dir = xunt(xsub(g.points[last], g.points[first]));
    const Attack melee = { mag, dir, it.hurts, MELEE, 0, xzpoint() };
    return melee;
}

// Draws ranged gauge.
static Attack dgrange(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    if(g.count > 0)
    {
        const int state = it.amplitude / 2.0f;
        const int width = it.amplitude * xsinc(g.count, it.period) + state;

        const int x = g.points[g.count - 1].x * sens - (width - sdl.xres) / 2;
        const int y = g.points[g.count - 1].y * sens - (width - sdl.yres) / 2;
        dbox(sdl, x, y, width, sdl.wht, false);

        // TODO: Fix this so that range attack is based on sinc steady state.
        const float mag = 100.0f;

        const Point reticule = {
            (float) (x + rand() % (width < 1 ? 1 : width)), // Divide by zero check.
            (float) (y + rand() % (width < 1 ? 1 : width)),
        };

        // Range attacks will just have south facing hurt animation drawn.
        const Point dir = { 0.0f, -1.0f };
        const Attack range = { mag, dir, it.hurts, RANGE, 0, reticule };
        return range;
    }
    else return xzattack();
}

// Draws magic gauge.
static Attack dgmagic(const Sdl sdl, const Gauge g, const Item it, const float sens, const Inventory inv, const Scroll sc)
{
    xsclear(sc);
    if(g.count > 0)
    {
        const int size = (sc.width - 1) / 2;
        const int grid = sdl.yres / (sc.width / 0.8f);

        // Middle of the screen.
        const Point middle = { (float) sdl.xres / 2, (float) sdl.yres / 2 };

        // Half a grid for centering grid squares.
        const Point shift = { (float) grid / 2, (float) grid / 2 };

        // Animate attack (inside square for mouse cursor).
        for(int i = 0; i < g.count; i++)
        {
            const Point point = xadd(xmul(g.points[i], sens), shift);
            const Point corner = xsnap(point, grid);
            const Point center = xsub(xadd(corner, middle), shift);

            dbox(sdl, center.x, center.y, grid, sdl.wht, true);

            // Populate Scroll int array that was cleared earlier.
            const int x = size + corner.x / grid;
            const int y = size + corner.y / grid;

            // Clamp if over boundry.
            const int xc = x < 0 ? 0 : x >= sc.width ? sc.width - 1 : x;
            const int yc = y < 0 ? 0 : y >= sc.width ? sc.width - 1 : y;
            sc.casting[xc + yc * size] = 1;
        }

        // Animate attack (grid squares).
        for(int x = -size; x <= size; x++)
        for(int y = -size; y <= size; y++)
        {
            const Point which = {
                (float) x,
                (float) y,
            };
            const Point corner = xmul(which, grid);
            const Point center = xsub(xadd(corner, middle), shift);
            dbox(sdl, center.x, center.y, grid, sdl.red, false);
        }

        // Draw the cursor.
        const Point point = xadd(xmul(g.points[g.count - 1], sens), shift);
        const Point center = xsub(xadd(point, middle), shift);
        dbox(sdl, center.x, center.y, 6, sdl.red, true);
    }

    // Calculate attack.
    const int scindex = xsindex(sc);

    // TODO: Maybe accuracy to scroll gives better attack + item base attack?
    (void) it;
    const float mag = 0.0f;

    // If the scroll index is not found in the inventory the attack will go from MAGIC to NOATTACK.
    (void) inv;

    // Direction won't be needed for magic attacks as magic will spawn new sprites, be it food sprites,
    // attack sprites (fire / ice), etc.
    const Point dir = { 0.0f, 0.0f };
    const Attack magic = { mag, dir, 0, MAGIC, scindex, xzpoint() };
    return magic;
}

// Draws all power gauge squares.
Attack xdgauge(const Sdl sdl, const Gauge g, const Inventory inv, const Scroll sc)
{
    const Item it = inv.items.item[inv.selected];
    const float sens = 2.0f;
    return
        xismelee(it.c) ? dgmelee(sdl, g, it, sens) :
        xisrange(it.c) ? dgrange(sdl, g, it, sens) :
        xismagic(it.c) ? dgmagic(sdl, g, it, sens, inv, sc) :
        xzattack();
}

static int clipping(const Sdl sdl, const Overview ov, const SDL_Rect to)
{
    return (to.x > sdl.xres || to.x < -ov.w)
        && (to.y > sdl.yres || to.y < -ov.h);
}

// Draws tiles for the grid layout.
static void dgridl(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const Timer tm)
{
    SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sdl.renderer);

    // Put down tiles. Snaps to grid.
    for(int j = 0; j < map.rows; j++)
    for(int i = 0; i < map.cols; i++)
    {
        const int ascii =
            ov.party == FLORING ? map.floring[j][i] :
            ov.party == CEILING ? map.ceiling[j][i] : map.walling[j][i];

        const int ch = ascii - ' ';
        if(ch == 0)
            continue;

        const SDL_Rect to = { ov.w * i + ov.px, ov.h * j + ov.py, ov.w, ov.h };

        if(clipping(sdl, ov, to))
            continue;

        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[ch], NULL, &to);
    }

    // Put down sprites.
    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int index = sprite->ascii - ' ';
        const int w = sdl.surfaces.surface[index]->w / FRAMES;
        const int h = sdl.surfaces.surface[index]->h / STATES;

        const SDL_Rect from = { w * xtmhi(tm), h * sprite->state, w, h };

        const SDL_Rect to = {
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
static void dpanel(const Sdl sdl, const Overview ov, const Timer tm)
{
    for(int i = ov.wheel; i <= '~' - ' '; i++)
    {
        const SDL_Rect to = { ov.w * (i - ov.wheel), 0, ov.w, ov.h };

        // Draw sprite on panel.
        if(xsissprite(i + ' '))
        {
            const int w = sdl.surfaces.surface[i]->w / FRAMES;
            const int h = sdl.surfaces.surface[i]->h / STATES;
            const SDL_Rect from = { w * xtmhi(tm), h * IDLE, w, h };
            if(clipping(sdl, ov, to))
                continue;
            SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], &from, &to);
        }
        // Draw tile block on panel.
        else SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i], NULL, &to);
    }
}

// View the map editor.
void xview(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const Timer tm)
{
    dgridl(sdl, ov, sprites, map, tm);
    dpanel(sdl, ov, tm);
}

// Draws status bar (one of health, magic, or fatigue).
void xdbar(const Sdl sdl, const Hero hero, const int position, const Timer tm, const int size, const Bar bar)
{
    const int max =
        bar == HPS ? hero.hpsmax :
        bar == FTG ? hero.ftgmax : hero.mnamax;

    const float lvl =
        bar == HPS ? hero.hps :
        bar == FTG ? hero.ftg : hero.mna;

    const float threshold = hero.warning * max;

    SDL_Texture* const texture = sdl.textures.texture[sdl.gui];
    SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui];

    const int w = surface->w;

    const SDL_Rect gleft = { 0,  0, w, w };
    const SDL_Rect glass = { 0, 32, w, w };
    const SDL_Rect grite = { 0, 64, w, w };

    // Will animate bar to flicker if below threshold.
    const int frame = xtmhi(tm);
    const SDL_Rect fluid = { 0, (int) bar + (lvl < threshold ? w * frame : 0), w, w };
    const SDL_Rect empty[] = {
        { 0, fluid.y + 2 * w, w, w }, // 75%.
        { 0, fluid.y + 4 * w, w, w }, // 50%.
        { 0, fluid.y + 6 * w, w, w }, // 25%.
    };

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

        // Glass around fluid.
        SDL_RenderCopy(sdl.renderer, texture, i == 0 ? &gleft : i == max - 1 ? &grite : &glass, &to);
    }
}

// Draws all hero status bars (health, mana, and fatigue).
void xdbars(const Sdl sdl, const Hero hero, const Timer tm)
{
    const int size = 1;
    xdbar(sdl, hero, 2, tm, size, HPS);
    xdbar(sdl, hero, 1, tm, size, MNA);
    xdbar(sdl, hero, 0, tm, size, FTG);
}

// Draws the inventory backpanel. Selected inventory item is highlighted.
static void dinvbp(const Sdl sdl, const Inventory inv)
{
    const Point wht = { 0.0, 512.0 };
    const Point red = { 0.0, 528.0 };
    const Point grn = { 0.0, 544.0 };
    for(int i = 0; i < inv.items.max; i++)
    {
        SDL_Texture* const texture = sdl.textures.texture[sdl.gui];
        SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui];

        const int w = surface->w;
        const int xx = sdl.xres - inv.width;

        const SDL_Rect from = {
            (int) (i == inv.hilited ? grn.x : i == inv.selected ? red.x : wht.x),
            (int) (i == inv.hilited ? grn.y : i == inv.selected ? red.y : wht.y),
            w, w
        };

        const SDL_Rect to = { xx, inv.width * i, inv.width, inv.width };
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
        const int xx = sdl.xres - inv.width;
        const SDL_Rect from = { 0, w * item.index, w, w }, to = { xx, inv.width * i, inv.width, inv.width };

        SDL_RenderCopy(sdl.renderer, texture, &from, &to);
    }
}

// Draws the inventory.
void xdinv(const Sdl sdl, const Inventory inv)
{
    dinvbp(sdl, inv);
    dinvits(sdl, inv);
}

// Draws the map.
void xdmap(const Sdl sdl, const Map map, const Point where)
{
    SDL_Texture* const texture = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        map.cols,
        map.rows);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    const Vram vram = xvlock(texture);
    xvdrooms(vram, map, sdl.wht, sdl.blk);
    xvddot(vram, where, 3, sdl.red, sdl.blk);
    xvunlock(texture);

    const SDL_Rect dst = { 0, 0, map.cols, map.rows };
    SDL_RenderCopy(sdl.renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}


void xdfps(const Sdl sdl, const Text text, const int fps)
{
    char* const str = fmts("%d", fps);
    xfputbr(text.fill, text.line, str, 0xFF, sdl.renderer, sdl.xres, sdl.yres);
    free(str);
}
