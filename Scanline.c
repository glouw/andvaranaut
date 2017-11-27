#include "Scanline.h"

#include "util.h"

#include <assert.h>

// Modulous modify a pixel. Discards alpha.
static uint32_t mod(uint32_t pixel, const float m)
{
    return
    (int) (((pixel >> 0x10) /*  */) * m / 0xFF) << 0x10 |
    (int) (((pixel >> 0x08) & 0xFF) * m / 0xFF) << 0x08 |
    (int) (((pixel /*   */) & 0xFF) * m / 0xFF);
}

void xwraster(const Scanline sl, const Ray r, const Torch t)
{
    // Get the hit surface.
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[r.hit.surface];
    const int row = surface->h * r.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    // Calculate surface light modding.
    const float m = xilluminate(t, r.traceline.corrected.x);
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        // Calculate the floor casting offset.
        const float offset = (x - r.proj.bot) / r.proj.height;
        const int col = surface->w * offset;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xfraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Calculate the floor casting offset.
        const float offset = xfcast(r.traceline, x, r.proj.mid) / yaw;
        if(offset > 1.0) printf("flor: %f: %d %d %f\n", offset, sl.sdl.yres, sl.sdl.yres / 2, r.proj.mid);
        const Point where = xlerp(r.traceline.trace, offset);
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[xtile(where, map.floring)];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Calculate surface lighting midding. Moddings are saved for the ceiling rasterer.
        const float m = xilluminate(t, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xcraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const float offset = xccast(r.traceline, x, r.proj.mid) / yaw;
        const Point where = xlerp(r.traceline.trace, offset);
        const int tile = xtile(where, map.ceiling);
        if(offset > 1.0) printf("ceil: %f: %d %d %f\n", offset, sl.sdl.yres, sl.sdl.yres / 2, r.proj.mid);
        if(!tile) continue;
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        const float m = xilluminate(t, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xsraster(const Scanline sl, const Ray r, const Torch t, const float yaw, const Clouds clouds)
{
    // Get the hit surface.
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[1];
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const float offset = clouds.height * xccast(r.traceline, x, r.proj.mid) / yaw;
        const Point where = xlerp(r.traceline.trace, offset);
        const int row = abs(surface->h * xdec(where.y + clouds.where.y));
        const int col = abs(surface->w * xdec(where.x + clouds.where.x));
        const uint32_t pixel = pixels[col + row * surface->w];
        const float m = xilluminate(t, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        // SEGFAULTS are happening here?!
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xbraster(const Scanline sl)
{
    for(int x = sl.sdl.yres / 2; x < sl.sdl.yres; x++)
        sl.display.pixels[x + sl.y * sl.display.width] = 0x00;
}
