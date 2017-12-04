#include "Scanline.h"

#include "util.h"

// Modulous modify a pixel. Discards alpha. Great for lighting.
static uint32_t mod(const uint32_t pixel, const int modding)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * modding) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * modding) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * modding) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

// Pixel getter.
static uint32_t pget(const SDL_Surface* const surface, const Point offset)
{
    const int row = surface->h * xdec(offset.y);
    const int col = surface->w * xdec(offset.x);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    return pixels[col + row * surface->w];
}

// Pixel putter.
static void pput(const Scanline sl, const int x, const int pixel)
{
    sl.display.pixels[x + sl.y * sl.display.width] = pixel;
}

// Wall rasterer.
static void wraster(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        // Get pixel.
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.hit.offset };
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[r.hit.surface], offset);
        // Shade and transfer pixel.
        const float distance = r.traceline.corrected.x;
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

// Floor rasterer.
static void fraster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.traceline.trace, xfcast(r.proj, x - 1));
        const int tile = xtile(offset, map.floring);
        if(!tile) continue;
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[tile], offset);
        // Shade and transfer pixel.
        const float distance = xmag(xsub(offset, r.traceline.trace.a));
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

// Ceiling rasterer.
static void craster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.traceline.trace, xccast(r.proj, x + 1));
        const int tile = xtile(offset, map.ceiling);
        if(!tile) continue;
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[tile], offset);
        // Shade and transfer pixel.
        const float distance = xmag(xsub(offset, r.traceline.trace.a));
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

// Sky rasterer.
static void sraster(const Scanline sl, const Ray r, const Map map, const int floor)
{
    // Simple black sky if on zeroth floor.
    if(floor == 0)
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++) pput(sl, x, 0x000000);
    // Otherwise faux ceiling.
    else
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            // Get pixel.
            const Point offset = xlerp(r.traceline.trace, xccast(r.proj, x + 2));
            const int tile = xtile(offset, map.ceiling);
            if(tile) continue;
            const uint32_t pixel = pget(sl.sdl.surfaces.surface['#' - ' '], offset);
            // Shade and transfer pixel.
            const float distance = xmag(xsub(offset, r.traceline.trace.a));
            pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
        }
}

// Pit rasterer.
static void praster(const Scanline sl, const Ray r, const Map map, const Current current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.traceline.trace, xfcast(r.proj, x - 2));
        const int tile = xtile(offset, map.floring);
        if(tile) continue;
        const Point water = xadd(current.where, offset);
        const uint32_t pixel = pget(sl.sdl.surfaces.surface['%' - ' '], water);
        // Shade and transfer pixel.
        const float distance = xmag(xsub(offset, r.traceline.trace.a));
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

// Upper level rasterer.
void uraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const behind = hit;
        const Hit* const before = hit->next;
        const Ray hind = xcalc(hero, *behind, 1, sl.sdl.yres);
        if(link++ == 0)
            sraster(sl, hind, map, hero.floor);
        if(before)
        {
            const Ray fore = xcalc(hero, *before, 1, sl.sdl.yres);
            const Ray flat = xoverlay(hind, fore);
            wraster(sl, flat);
        }
        else
            wraster(sl, hind);
    }
}

// Low level rasterer.
static void lraster(const Scanline sl, const Hits hits, const Hero hero, const Current current, const Map map)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const behind = hit;
        const Hit* const before = hit->next;
        const Ray hind = xcalc(hero, *behind, -1, sl.sdl.yres);
        if(link++ == 0)
            praster(sl, hind, map, current);
        if(before)
        {
            const Ray fore = xcalc(hero, *before, -1, sl.sdl.yres);
            const Ray flat = xoverlay(hind, fore);
            wraster(sl, flat);
        }
        else
            wraster(sl, hind);
    }
}

// Eye level rasterer.
static Point eraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = xcalc(hero, hits.walling, 0, sl.sdl.yres);
    wraster(sl, ray);
    fraster(sl, ray, map);
    craster(sl, ray, map);
    return ray.traceline.corrected;
}


Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Current current, const Map map)
{
    // Debugging highlighter for finding uncolored pixels.
    #if 1
    for(int x = 0; x < sl.sdl.yres; x++) pput(sl, x, 0xFF0000);
    #endif
    uraster(sl, hits, hero, map);
    lraster(sl, hits, hero, current, map);
    return eraster(sl, hits, hero, map);
}
