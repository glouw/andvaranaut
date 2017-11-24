#include "Scanline.h"

#include "util.h"

// Modulous modify a pixel.
static uint32_t mod(uint32_t pixel, const float m)
{
    return pixel;
    const int rm = (m / 0xFF) * ((pixel >> 0x10) & 0xFF);
    const int gm = (m / 0xFF) * ((pixel >> 0x08) & 0xFF);
    const int bm = (m / 0xFF) * ((pixel >> 0x00) & 0xFF);
    return rm << 0x10 | gm << 0x08 | bm << 0x00;
}

void xwraster(const Scanline sl, const Ray r, const Torch t)
{
    // Get the hit surface.
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[r.hit.surface];
    const int row = surface->h * r.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    // Calculate surface light modding.
    const int m = xilluminate(t, r.traceline.corrected.x);
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

static float fcast(const Traceline traceline, const float y, const float yres)
{
    return (traceline.fov.a.x / traceline.corrected.x) / (1.0 - y / (yres / 2.0));
}

void xfraster(const Scanline sl, const Ray r, const Torch t, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Calculate the floor casting offset.
        const float offset = 1.0 * fcast(r.traceline, x, sl.sdl.yres);
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

void xcraster(const Scanline sl, const Ray r, const Torch t, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const float offset = 1.0 * fcast(r.traceline, sl.sdl.yres - 1 - x, sl.sdl.yres);
        const Point where = xlerp(r.traceline.trace, offset);
        const int tile = xtile(where, map.ceiling);
        if(!tile) continue;
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        const int m = xilluminate(t, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xsraster(const Scanline sl, const Ray r, const Torch t, const Clouds clouds)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Calculate the floor casting offset.
        const float offset = 9.0 * fcast(r.traceline, sl.sdl.yres - 1 - x, sl.sdl.yres);
        const Point where = xlerp(r.traceline.trace, offset);
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[1];
        const int row = abs(surface->h * xdec(where.y + clouds.where.y));
        const int col = abs(surface->w * xdec(where.x + clouds.where.x));
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        const int m = xilluminate(t, xmag(xsub(where, r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
        // Shift the clouds
    }
}
