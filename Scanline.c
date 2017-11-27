#include "Scanline.h"

#include "util.h"

#include <assert.h>

// Modulous modify a pixel. Discards alpha.
static uint32_t mod(const uint32_t pixel, const int m)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * m) >> 0x08; // Shift by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * m) >> 0x08; // division by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * m) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

void xwraster(const Scanline sl, const Ray r)
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
        const float offset = (x - r.proj.bot) / r.proj.height;
        const int col = surface->w * offset;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, modding);
    }
}

void xfraster(const Scanline sl, const Ray r, const float yaw, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Calculate the floor casting offset.
        const float mid = yaw * sl.sdl.yres / 2.0;
        const float offset = xfcast(r.traceline, x, mid) / yaw;
        const Point where = xlerp(r.traceline.trace, offset);
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

void xcraster(const Scanline sl, const Ray r, const float yaw, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const float mid = yaw * sl.sdl.yres / 2.0;
        const float offset = xccast(r.traceline, x, mid) / yaw;
        const Point where = xlerp(r.traceline.trace, offset);
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

void xsraster(const Scanline sl, const Ray r)
{
    // Get the hit surface.
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        sl.display.pixels[x + sl.y * sl.display.width] = 0x0;
}
