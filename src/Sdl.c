#include "Sdl.h"

#include "Bar.h"
#include "Frame.h"
#include "Scanline.h"
#include "Bundle.h"
#include "util.h"

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

void s_present(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static SDL_Rect left_clip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
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

static SDL_Rect rite_clip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
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

static SDL_Rect clip(SDL_Rect seen, const int xres, const Point where, Point* const zbuff)
{
    seen = left_clip(seen, xres, where, zbuff);
    return rite_clip(seen, xres, where, zbuff);
}

static void draw_box(const Sdl sdl, const int x, const int y, const int width, const uint32_t color, const int filled)
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

static void render_speech(Sprite* const sprite, const Sdl sdl, const Text text, const SDL_Rect target)
{
    if(sprite->state == SPEAKING)
    {
        //
        // TODO: Maybe tune the (y) offset per sprite?
        //

        const char* const sentence = sprite->speech.sentences[sprite->speech.index];
        const int x = target.x + target.w / 2;
        const int y = target.y + target.h / 3;
        t_write(text, sdl.renderer, x, y, MIDDLE, 0xFF, 0, sentence);
    }
}

static SDL_Rect calc_sprite_size(const Sdl sdl, Sprite* const sprite, const Hero hero)
{
    //
    // Projection.[ch] does the same thing, but this one accounts for sprite jitter.
    //

    const int size = sprite->size * hero.fov.a.x * 0.5f * sdl.xres / sprite->where.x;
    const int osize = u_odd(size) ? size + 1 : size;
    const int my = 0.5f * sdl.yres * (2.0f - hero.pitch);
    const int mx = 0.5f * sdl.xres;
    const int l = mx - osize * 0.5f;
    const int t = my - osize * (1.0f - hero.height / sprite->size);
    const int s = hero.fov.a.x * mx * p_slope(sprite->where);
    const SDL_Rect target = { l + s, t, osize, osize };
    return target;
}

static SDL_Rect calc_state_frame(SDL_Surface* const surface, const State state, const Frame frame)
{
    const int w = surface->w / FRAMES;
    const int h = surface->h / STATES;
    const SDL_Rect image = { w * frame, h * state, w, h };
    return image;
}

static void render_one_sprite(const Sdl sdl, const Text text, Sprite* const sprite, const Hero hero, SDL_Texture* const texture, const SDL_Rect image, const SDL_Rect target)
{
    //
    // Apply lighting to the sprite.
    //

    const int shade = t_illuminate(hero.torch.light, sprite->where.x);
    SDL_SetTextureColorMod(texture, shade, shade, shade);

    //
    // Apply transparency to the sprite, if required.
    //

    if(sprite->transparent)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

    //
    // Render the sprite.
    //

    SDL_RenderSetClipRect(sdl.renderer, &sprite->seen);
    SDL_RenderCopy(sdl.renderer, texture, &image, &target);
    SDL_RenderSetClipRect(sdl.renderer, NULL);

    //
    // Remove transparancy from the sprite.
    //

    if(sprite->transparent)
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

    //
    // Remove lighting from the sprite.
    //

    SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);

    //
    // If the sprite is within earshot of hero then render speech sentences.
    //

    render_speech(sprite, sdl, text, target);
}

static void render_all_sprites(const Sdl sdl, const Text text, const Sprites sprites, Point* const zbuff, const Hero hero, const Timer tm)
{
    s_pull(sprites, hero);

    s_turn(sprites, -hero.yaw);

    s_sort(sprites, s_furthest_sprite_first);

    for(int which = 0; which < sprites.count; which++)
    {
        Sprite* const sprite = &sprites.sprite[which];
        if(sprite->where.x > 0)
        {
            const SDL_Rect target = calc_sprite_size(sdl, sprite, hero);
            if(target.x + target.w >= 0 && target.x < sdl.xres)
            {
                const int selected = sprite->ascii - ' ';

                const Dye dye = sprite->dye;
                SDL_Surface* const surface = sdl.surfaces.surface[selected].dye[dye];
                SDL_Texture* const texture = sdl.textures.texture[selected].dye[dye];

                const State state = sprite->state;
                const Frame frame = t_lo(tm) ? FRAME_A : FRAME_B;
                const SDL_Rect image = calc_state_frame(surface, state, frame);
                sprite->seen = clip(target, sdl.xres, sprite->where, zbuff);
                if(sprite->seen.w > 0)
                    render_one_sprite(sdl, text, sprite, hero, texture, image, target);
            }
        }
    }
    s_sort(sprites, s_nearest_sprite_first);

    s_turn(sprites, hero.yaw);

    s_push(sprites, hero);
}

Sdl s_setup(const Args args)
{
    static Sdl zero;
    Sdl sdl = zero;

    //
    // On cold boot, an everything init takes more than one second.
    // Sequential boots are near instant.
    //

    SDL_Init(SDL_INIT_EVERYTHING);

    sdl.window = SDL_CreateWindow(
        "Andvaranaut",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        args.xres,
        args.yres,
        SDL_WINDOW_SHOWN);

    if(sdl.window == NULL)
        u_bomb("error: could not open window\n");

    //
    // Renderer construcction also measured to take about one second.
    //

    sdl.renderer = SDL_CreateRenderer(
        sdl.window,
        -1,
        SDL_RENDERER_ACCELERATED |
        (args.vsync ? SDL_RENDERER_PRESENTVSYNC : 0x0)); 

    //
    // xres and yres swapped since software rendering is done flipped 90 degrees on its side.
    //

    sdl.canvas = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        args.yres, args.xres);

    sdl.xres = args.xres;
    sdl.yres = args.yres;
    sdl.fps = args.fps;
    sdl.threads = args.threads;

    //
    // Surface loading also measured to be about half a second.
    //

    sdl.palette = p_make();
    sdl.surfaces = s_load_surfaces(sdl.palette);
    sdl.textures = t_cache(sdl.surfaces, sdl.renderer);

    sdl.gui = '~' - ' ' + 26;

    //
    // Cold boot is about three seconds all together.
    //

    return sdl;
}

static Attack draw_gauge_melee(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    for(int i = 0; i < g.count; i++)
    {
        const float growth = i / (float) g.count;
        const int size = 16;
        const int width = growth * size;
        const int red = 0xFF;
        const int grn = 0xFF * growth;
        const int blu = 0xFF * g.alternate;
        const uint32_t color = (red << 0x10) | (grn << 0x08) | blu;
        const Point mid = {
            (width - sdl.xres) / 2.0f,
            (width - sdl.yres) / 2.0f,
        };
        const Point where = p_sub(p_mul(g.points[i], sens), mid);
        draw_box(sdl, where.x, where.y, width, color, true);
    }
    const int tail = 10;
    if(g.count < tail)
    {
        static Attack zero;
        return zero;
    }
    static Point zero;
    const Attack melee = { it, g_velocity(g), g_sum(g, g.count), MELEE, 0, zero };
    return melee;
}

static Attack draw_gauge_range(const Sdl sdl, const Gauge g, const Item it, const float sens)
{
    if(g.count > 0)
    {
        const int state = it.amplitude / 2.0f;
        const int width = it.amplitude * u_sinc(g.count, it.period) + state;

        const int x = g.points[g.count - 1].x * sens - (width - sdl.xres) / 2;
        const int y = g.points[g.count - 1].y * sens - (width - sdl.yres) / 2;
        draw_box(sdl, x, y, width, sdl.palette.wht, false);

        //
        // Reticule includes divide by zero check.
        //

        const Point reticule = {
            (float) (x + rand() % (width < 1 ? 1 : width)),
            (float) (y + rand() % (width < 1 ? 1 : width)),
        };

        //
        // Range attacks will just have south facing hurt animation drawn.
        //

        const Point dir = { 0.0f, -1.0f };
        static Point zero;
        const Attack range = { it, dir, zero, RANGE, 0, reticule };

        return range;
    }
    else
    {
        static Attack zero;
        return zero;
    }
}

//
// TODO: This function ins too long. Needs to be cleaned up.
//

static Attack draw_gauge_magic(const Sdl sdl, const Gauge g, const Item it, const float sens, const Inventory inv, const Scroll sc)
{
    s_clear(sc);
    if(g.count > 0)
    {
        const int size = (sc.width - 1) / 2;
        const int grid = sdl.yres / (sc.width / 0.8f);

        //
        // Middle of the screen.
        //

        const Point middle = { (float) sdl.xres / 2, (float) sdl.yres / 2 };

        //
        // Half a grid for centering grid squares.
        //

        const Point shift = { (float) grid / 2, (float) grid / 2 };

        //
        // Animate attack (inside square for mouse cursor).
        //

        for(int i = 0; i < g.count; i++)
        {
            const Point point = p_add(p_mul(g.points[i], sens), shift);
            const Point corner = p_snap(point, grid);
            const Point center = p_sub(p_add(corner, middle), shift);

            draw_box(sdl, center.x, center.y, grid, sdl.palette.wht, true);

            //
            // Populate Scroll int array that was cleared earlier.
            //

            const int x = size + corner.x / grid;
            const int y = size + corner.y / grid;

            //
            // Clamp if over boundry.
            //

            const int xc = x < 0 ? 0 : x >= sc.width ? sc.width - 1 : x;
            const int yc = y < 0 ? 0 : y >= sc.width ? sc.width - 1 : y;
            sc.casting[xc + yc * size] = 1;
        }

        //
        // Animate attack (grid squares).
        //

        for(int x = -size; x <= size; x++)
        for(int y = -size; y <= size; y++)
        {
            const Point which = {
                (float) x,
                (float) y,
            };
            const Point corner = p_mul(which, grid);
            const Point center = p_sub(p_add(corner, middle), shift);
            draw_box(sdl, center.x, center.y, grid, sdl.palette.red, false);
        }

        //
        // Draw the cursor.
        //

        const Point point = p_add(p_mul(g.points[g.count - 1], sens), shift);
        const Point center = p_sub(p_add(point, middle), shift);
        draw_box(sdl, center.x, center.y, 6, sdl.palette.red, true);
    }

    const int scroll_index = s_index(sc);

    //
    // If the scroll index is not found in the inventory the attack will go from MAGIC to NOATTACK.
    //

    (void) inv;

    //
    // Direction won't be needed for magic attacks as magic will spawn new sprites, be it food sprites,
    // attack sprites (fire / ice), etc.
    //

    const Point dir = { 0.0f, 0.0f };
    static Point zero;
    const Attack magic = { it, dir, zero, MAGIC, scroll_index, zero };
    return magic;
}

Hero s_draw_gauge(const Sdl sdl, Hero hero, const Scroll scroll)
{
    const float sens = 2.0f;
    const Item item = hero.inventory.items.item[hero.inventory.selected];
    static Attack zero;
    hero.attack =
        c_is_melee(item.id.clas) ? draw_gauge_melee(sdl, hero.gauge, item, sens) :
        c_is_range(item.id.clas) ? draw_gauge_range(sdl, hero.gauge, item, sens) :
        c_is_magic(item.id.clas) ? draw_gauge_magic(sdl, hero.gauge, item, sens, hero.inventory, scroll) : zero;
    return hero;
}

static int clipping(const Sdl sdl, const Overview ov, const SDL_Rect to)
{
    return (to.x > sdl.xres || to.x < -ov.w)
        && (to.y > sdl.yres || to.y < -ov.h);
}

static void draw_grid_layout(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const Timer tm)
{
    SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(sdl.renderer);

    //
    // Put down tiles. Snaps to grid.
    //

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
        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[ch].dye[DYE_WHT], NULL, &to);
    }

    //
    // Put down sprites.
    //

    for(int s = 0; s < sprites.count; s++)
    {
        Sprite* const sprite = &sprites.sprite[s];
        const int index = sprite->ascii - ' ';

        const Dye dye = sprite->dye;
        const int w = sdl.surfaces.surface[index].dye[dye]->w / FRAMES;
        const int h = sdl.surfaces.surface[index].dye[dye]->h / STATES;

        const SDL_Rect from = { w * t_lo(tm), h * sprite->state, w, h };
        const SDL_Rect to = {
            (int) ((ov.w * sprite->where.x - ov.w / 2) + ov.px),
            (int) ((ov.h * sprite->where.y - ov.h / 1) + ov.py),
            ov.w, ov.h,
        };

        if(clipping(sdl, ov, to))
            continue;

        SDL_RenderCopy(sdl.renderer, sdl.textures.texture[index].dye[dye], &from, &to);
    }
}

static void draw_sprite_panel(const Sdl sdl, const Overview ov, const Timer tm)
{
    for(int i = ov.wheel; i <= '~' - ' '; i++)
    {
        const SDL_Rect to = { ov.w * (i - ov.wheel), 0, ov.w, ov.h };

        //
        // Draw sprite on panel, otherwise draw tile block on panel.
        //

        const int ascii = i + ' ';
        if(s_sprite(ascii))
        {
            const Dye dye = s_lookup_dye(ascii);
            const int w = sdl.surfaces.surface[i].dye[dye]->w / FRAMES;
            const int h = sdl.surfaces.surface[i].dye[dye]->h / STATES;
            const SDL_Rect from = { w * t_lo(tm), h * IDLE, w, h };
            if(clipping(sdl, ov, to))
                continue;
            SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i].dye[dye], &from, &to);
        }
        else
            SDL_RenderCopy(sdl.renderer, sdl.textures.texture[i].dye[DYE_WHT], NULL, &to);
    }
}

void s_render_overlay(const Sdl sdl, const Overview ov, const Sprites sprites, const Map map, const Timer tm)
{
    draw_grid_layout(sdl, ov, sprites, map, tm);
    draw_sprite_panel(sdl, ov, tm);
}

static void draw_one_bar(const Sdl sdl, const Hero hero, const int position, const Timer tm, const int size, const Bar bar)
{
    const int max = bar == HEALTH ? hero.health_max : bar == FATIGUE ? hero.fatigue_max : hero.mana_max;

    const float level = bar == HEALTH ? hero.health : bar == FATIGUE ? hero.fatigue : hero.mana;

    const float threshold = hero.warning * max;

    SDL_Texture* const texture = sdl.textures.texture[sdl.gui].dye[DYE_WHT];
    SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui].dye[DYE_WHT];

    const int w = surface->w;

    const SDL_Rect gleft = { 0,  0, w, w };
    const SDL_Rect glass = { 0, 32, w, w };
    const SDL_Rect grite = { 0, 64, w, w };

    //
    // Will animate bar to flicker if below threshold.
    //

    const int frame = t_lo(tm);
    const SDL_Rect fluid = { 0, (int) bar + (level < threshold ? w * frame : 0), w, w };
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

        //
        // Full fluid level.
        //

        if(u_fl(level) > i)
            SDL_RenderCopy(sdl.renderer, texture, &fluid, &to);

        //
        // Partial fluid level.
        //

        if(u_fl(level) == i)
            u_dec(level) > 0.75f ? SDL_RenderCopy(sdl.renderer, texture, &empty[0], &to) :
            u_dec(level) > 0.50f ? SDL_RenderCopy(sdl.renderer, texture, &empty[1], &to) :
            u_dec(level) > 0.25f ? SDL_RenderCopy(sdl.renderer, texture, &empty[2], &to) : 0;

        //
        // Glass around fluid.
        //

        SDL_RenderCopy(sdl.renderer, texture, i == 0 ? &gleft : i == max - 1 ? &grite : &glass, &to);
    }
}

static void draw_all_bars(const Sdl sdl, const Hero hero, const Timer tm, const int size)
{
    draw_one_bar(sdl, hero, 2, tm, size, HEALTH);
    draw_one_bar(sdl, hero, 1, tm, size, MANA);
    draw_one_bar(sdl, hero, 0, tm, size, FATIGUE);
}

static void draw_inventory_panel(const Sdl sdl, const Inventory inv)
{
    const Point wht = { 0.0, 512.0 };
    const Point red = { 0.0, 528.0 };
    const Point grn = { 0.0, 544.0 };

    for(int i = 0; i < inv.items.max; i++)
    {
        SDL_Texture* const texture = sdl.textures.texture[sdl.gui].dye[DYE_WHT];
        SDL_Surface* const surface = sdl.surfaces.surface[sdl.gui].dye[DYE_WHT];
        const int w = surface->w;
        const int xx = sdl.xres - inv.width;

        //
        // Selected inventory item is highlighted.
        //

        const SDL_Rect from = {
            (int) (i == inv.hilited ? grn.x : i == inv.selected ? red.x : wht.x),
            (int) (i == inv.hilited ? grn.y : i == inv.selected ? red.y : wht.y),
            w, w
        };

        const SDL_Rect to = { xx, inv.width * i, inv.width, inv.width };
        SDL_RenderCopy(sdl.renderer, texture, &from, &to);
    }
}

static void draw_inventory_items(const Sdl sdl, const Inventory inv, const Input in)
{
    for(int i = 0; i < inv.items.max; i++)
    {
        const Item item = inv.items.item[i];

        if(item.id.clas == NONE)
            continue;

        const int index = c_get_surface_index(item.id.clas);
        SDL_Texture* const texture = sdl.textures.texture[index].dye[DYE_WHT];
        SDL_Surface* const surface = sdl.surfaces.surface[index].dye[DYE_WHT];
        const int w = surface->w;
        const int xx = sdl.xres - inv.width;
        const SDL_Rect from = { 0, w * item.id.index, w, w };
        const int dragging = i == inv.drag;
        const SDL_Rect to = {
            dragging ? in.x : xx,
            dragging ? in.y : inv.width * i,
            inv.width,
            inv.width,
        };
        SDL_RenderCopy(sdl.renderer, texture, &from, &to);
    }
}

static void draw_inventory(const Sdl sdl, const Inventory inv, const Input in)
{
    draw_inventory_panel(sdl, inv);
    draw_inventory_items(sdl, inv, in);
}

static void draw_map(const Sdl sdl, const Map map, const Point where)
{
    //
    // Background is black with nex texture.
    //

    SDL_Texture* const texture = SDL_CreateTexture(
        sdl.renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        map.cols,
        map.rows);

    //
    // Black background will dissapear with a blending blend mode.
    //

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    const Vram vram = v_lock(texture);
    v_draw_rooms(vram, map, sdl.palette.wht, sdl.palette.blk);
    v_draw_dot(vram, where, 3, sdl.palette.red, sdl.palette.blk);
    v_unlock(texture);

    const SDL_Rect dst = { 0, 0, map.cols, map.rows };
    SDL_RenderCopy(sdl.renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

void s_draw_room_lookup(const Sdl sdl, const Text yel, const Text red, const Rooms rooms, const int floor, const int room)
{
    t_printf(yel, sdl.renderer, sdl.xres, 0, TOP_RITE, 0xFF, 0, "FLOOR: %2d", floor);
    for(int i = 0; i < rooms.count; i++)
    {
        const char* name = t_get_name(rooms.room[i].theme);
        t_printf(room == i ? red : yel, sdl.renderer, 0, 0, TOP_LEFT, 0xFF, i, "[%c] %s", i + 'A', name);
    }
}

void s_render_playing(const Sdl sdl, const Text text, const Hero hero, const Sprites sprites, const Map map, const Flow current, const Flow clouds, const Timer tm, const Input in)
{
    Point* const zbuff = u_toss(Point, sdl.xres);
    const Line camera = l_rotate(hero.fov, hero.yaw);
    const Vram vram = v_lock(sdl.canvas);

    //
    // Threaded software rendering - each thread handles vertical columns <a> to <b> of the screen.
    //

    Bundle* const b = u_toss(Bundle, sdl.threads);
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
    SDL_Thread** const threads = u_toss(SDL_Thread*, sdl.threads);
    for(int i = 0; i < sdl.threads; i++)
        threads[i] = SDL_CreateThread(b_raster, "n/a", &b[i]);
    for(int i = 0; i < sdl.threads; i++)
    {
        int status;
        SDL_WaitThread(threads[i], &status);
    }
    v_unlock(sdl.canvas);

    //
    // Render was done sideways for cache efficiency. Rotate upwards.
    //

    churn(sdl);

    //
    // Sprites are rendered upwards.
    //

    render_all_sprites(sdl, text, sprites, zbuff, hero, tm);

    //
    // Draw the user interface.
    //

    draw_inventory(sdl, hero.inventory, in);
    draw_all_bars(sdl, hero, tm, 1);
    draw_map(sdl, map, hero.where);

    //
    // Cleanup.
    //

    free(zbuff);
    free(b);
    free(threads);
}
