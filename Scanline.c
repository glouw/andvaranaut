#include "Scanline.h"

#include "util.h"

#include <assert.h>

// Modulous modify a pixel. Discards alpha.
static uint32_t mod(const uint32_t pixel, const int m)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * m) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * m) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * m) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

static void wraster(const Scanline sl, const Ray r)
{
    // Get the hit surface.
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[r.hit.surface];
    const int row = surface->h * r.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    // Calculate surface light modding.
    const int modding = xilluminate(r.torch, r.traceline.corrected.x);
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        // Calculate the floor casting offset.
        const float offset = (x - r.proj.bot) / r.proj.size;
        const int col = surface->w * offset;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, modding);
    }
}

float ccast(const float height, const float size, const int x, const float yaw, const int yres)
{
    return height * size / ((x + 1) - yaw * (yres / 2));
}

float fcast(const float height, const float size, const int x, const float yaw, const int yres)
{
    return height * size / (yaw * (yres / 2) - (x - 1));
}

static void fraster(const Scanline sl, const Ray r, const float yaw, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Calculate the floor casting offset.
        const Point where = xlerp(r.traceline.trace, fcast(0.5, r.proj.size, x, yaw, sl.sdl.yres));
        const int tile = xtile(where, map.floring);
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Calculate surface lighting midding. Moddings are saved for the ceiling rasterer.
        const int modding = xilluminate(r.torch, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, modding);
    }
}

static void craster(const Scanline sl, const Ray r, const float yaw, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const Point where = xlerp(r.traceline.trace, ccast(0.5, r.proj.size, x, yaw, sl.sdl.yres));
        const int tile = xtile(where, map.ceiling);
        if(!tile) continue;
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        const int modding = xilluminate(r.torch, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, modding);
    }
}

static void sraster(const Scanline sl, const Ray r, const float yaw, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const Point where = xlerp(r.traceline.trace, ccast(0.5, r.proj.size, x, yaw, sl.sdl.yres));
        const int tile = xtile(where, map.ceiling);
        if(tile) continue;
        // Get the hit surface.
        const Point skies = xlerp(r.traceline.trace, ccast(1.5, r.proj.size, x, yaw, sl.sdl.yres));
        const SDL_Surface* const surface = sl.sdl.surfaces.surface['#' - ' '];
        const int row = abs(surface->h * xdec(skies.y));
        const int col = abs(surface->w * xdec(skies.x));
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        const int modding = xilluminate(r.torch, xmag(xsub(skies, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, modding);
    }
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    // Upper walls.
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const behind = hit;
        const Hit* const before = hit->next;
        const Ray hind = xcalc(hero, *behind, true, sl.sdl.yres);
        if(link++ == 0)
            sraster(sl, hind, hero.yaw, map);
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
    fraster(sl, ray, hero.yaw, map);
    craster(sl, ray, hero.yaw, map);
    return ray.traceline.corrected;
}
