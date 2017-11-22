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

// Wall rasterer for one scanline.
void xwraster(const Scanline sl, const Ray r, const Torch t)
{
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[r.hit.surface];
    const int row = surface->h * r.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const int m = xilluminate(t, r.traceline.corrected.x);
    for(int x = r.projection.clamped.bot; x < r.projection.clamped.top; x++)
    {
        const float height = r.projection.top - r.projection.bot;
        const float offset = (x - r.projection.bot) / height;
        const int col = surface->w * offset;
        const uint32_t pixel = pixels[col + row * surface->w];
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

// Floor rasterer for one scanline.
void xfraster(const Scanline sl, const Ray r, const Torch t, Point* wheres, char** floring, int* moddings)
{
    const float weight = r.traceline.fov.a.x / r.traceline.corrected.x;
    for(int x = 0; x < r.projection.clamped.bot; x++)
    {
        // Floor casting.
        const int xx = sl.sdl.yres - 1 - x;
        const float offset = weight / (1.0 - (float) x / r.projection.mid);
        const Point where = wheres[xx] = xlerp(r.traceline.trace, offset);
        const int tile = xtile(where, floring);
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const float distance = xmag(xsub(wheres[xx], r.traceline.trace.a));
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const int m = moddings[xx] = xilluminate(t, distance);
        const uint32_t pixel = pixels[col + row * surface->w];
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

// Ceiling rasterer for one scanline.
void xcraster(const Scanline sl, const Ray r, Point* wheres, char** ceiling, int* moddings)
{
    for(int x = r.projection.clamped.top; x < sl.sdl.yres; x++)
    {
        // Borrows calculations from the floor rasterer.
        const int tile = xtile(wheres[x], ceiling);
        if(!tile) continue;
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
        const int row = surface->h * xdec(wheres[x].y);
        const int col = surface->w * xdec(wheres[x].x);
        const int m = moddings[x];
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        sl.display.pixels[x + sl.y * sl.display.width] = mod(pixel, m);
    }
}

void xsraster(const Scanline sl)
{
    for(int x = sl.sdl.yres / 2; x < sl.sdl.yres; x++)
        sl.display.pixels[x + sl.y * sl.display.width] = 0x0;
}
