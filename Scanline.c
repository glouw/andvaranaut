#include "Scanline.h"

#include "util.h"

#include <assert.h>

// Modulous modify a pixel. Discards alpha.
static uint32_t mod(const uint32_t pixel, const int modding)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * modding) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * modding) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * modding) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

// Pixel getter.
static uint32_t pget(const SDL_Surface* const surface, const Point offset, const int clamp)
{
    const int row = clamp ? abs(surface->h * xdec(offset.y)) : surface->h * xdec(offset.y);
    const int col = clamp ? abs(surface->w * xdec(offset.x)) : surface->w * xdec(offset.x);
    // Row check.
    assert(row >= 0);
    assert(row < surface->h);
    // Col check.
    assert(col >= 0);
    assert(col < surface->w);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    return pixels[col + row * surface->w];
}

// Pixel putter.
static void pput(const Scanline sl, const int x, const int pixel)
{
    // Y check
    assert(sl.y >= 0);
    assert(sl.y < sl.sdl.xres);
    // X check
    assert(x >= 0);
    assert(x < sl.sdl.yres);
    sl.display.pixels[x + sl.y * sl.display.width] = pixel;
}

// Wall rasterer.
static void wraster(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        // Get pixel.
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.hit.offset };
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[r.hit.surface], offset, false);
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
        const Point offset = xlerp(r.traceline.trace, xfcast(r.proj, x));
        const int tile = xtile(offset, map.floring);
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[tile], offset, false);
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
        const Point offset = xlerp(r.traceline.trace, xccast(r.proj, x));
        const int tile = xtile(offset, map.ceiling);
        if(!tile) continue;
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[tile], offset, false);
        // Shade and transfer pixel.
        const float distance = xmag(xsub(offset, r.traceline.trace.a));
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

// Sky rasterer.
static void sraster(const Scanline sl, const Ray r, const Map map, const Clouds clouds, const int floor)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Get pixel.
        const Point offset = xlerp(r.traceline.trace, xccast(r.proj, x));
        const int tile = xtile(offset, map.ceiling);
        if(tile) continue;
        const Point skies = floor == 0 ?
            // See the blowing clouds.
            xadd(clouds.where, xlerp(r.traceline.trace, xccast(xrocket(r.proj), x))):
            // Do not see see the blowing clouds - added bonus of 'faux' level. Will always have ceiling tile.
            xlerp(r.traceline.trace, xccast(r.proj, x));
        const uint32_t pixel = pget(sl.sdl.surfaces.surface[floor == 0 ? '~' - ' ': '#' - ' '], skies, true);
        // Shade and transfer pixel.
        const float distance = xmag(xsub(offset, r.traceline.trace.a));
        pput(sl, x, mod(pixel, xilluminate(r.torch, distance)));
    }
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Clouds clouds, const Map map)
{
    // Upper walls.
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const behind = hit;
        const Hit* const before = hit->next;
        const Ray hind = xcalc(hero, *behind, true, sl.sdl.yres);
        if(link++ == 0)
            sraster(sl, hind, map, clouds, hero.floor);
        if(before)
        {
            const Ray fore = xcalc(hero, *before, true, sl.sdl.yres);
            const Ray flat = xoverlay(hind, fore);
            wraster(sl, flat);
        }
        else
            wraster(sl, hind);
    }
    // Lower walls.
    const Ray ray = xcalc(hero, hits.walling, false, sl.sdl.yres);
    wraster(sl, ray);
    fraster(sl, ray, map);
    craster(sl, ray, map);
    return ray.traceline.corrected;
}
