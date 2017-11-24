#include "Scanline.h"

#include "util.h"

// Modulous modify a pixel.
static uint32_t mod(uint32_t pixel, const float m)
{
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

void xfraster(const Scanline sl, const Ray r, Point* wheres, const Map map, int* moddings, const Torch t)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        // Calculate the floor casting offset.
        const int xx = sl.sdl.yres - 1 - x;
        const float offset = (r.traceline.fov.a.x / r.traceline.corrected.x) / (1.0 - (float) x / (sl.sdl.yres / 2));
        const Point where = wheres[xx] = xlerp(r.traceline.trace, offset);
        // Get the hit surface.
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[xtile(where, map.floring)];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Calculate surface lighting midding. Moddings are saved for the ceiling rasterer.
        moddings[xx] = xilluminate(t, xmag(xsub(wheres[xx], r.traceline.trace.a)));
        // Transfer surface to display.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, moddings[xx]);
    }
}

void xcraster(const Scanline sl, const Ray r, Point* wheres, const Map map, int* moddings)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Get the hit surface. Do not render if no ceiling.
        const int tile = xtile(wheres[x], map.ceiling);
        if(!tile) continue;
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(wheres[x].y);
        const int col = surface->w * xdec(wheres[x].x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        // Transfer surface to display. Light modding borrowed from ceiling renderer.
        // Since light moddings are array accessed, the yres of the display must be even.
        // This can be cooerced when command line res arguments are parsed.
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, moddings[x]);
    }
}

void xsraster(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        sl.display.pixels[x + sl.y * sl.display.width] = 0x0F0F0F;
}
