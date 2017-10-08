#include "Scanline.h"

#include "util.h"

// Wall renderer for one scanline
static void wrend(const Scanline sl, const Ray r)
{
    const SDL_Surface* const surface = sl.sdl.surfaces.surface[r.hit.surface];
    const int row = surface->h * r.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = r.projection.clamped.bot; x < r.projection.clamped.top; x++)
    {
        const float offset = (float) (x - r.projection.bot) / (float) (r.projection.height);
        const int col = surface->w * offset;
        sl.display.pixels[x + sl.y * sl.display.width] = pixels[col + row * surface->w];
    }
}

// Floor renderer for one scanline
static void frend(const Scanline sl, const Ray r, Point* wheres, char** floring)
{
    for(int x = 0; x < r.projection.clamped.bot; x++)
    {
        const Point where = wheres[sl.sdl.yres - 1 - x] =
            lerp(r.traceline.trace, fcast(r.traceline.fov, sl.sdl.yres, x, r.traceline.corrected.x));
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile(where, floring)];
        const int row = surface->h * xdec(where.y);
        const int col = surface->w * xdec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        sl.display.pixels[x + sl.y * sl.display.width] = pixels[col + row * surface->w];
    }
}

// Ceiling renderer for one scanline
static void crend(const Scanline sl, const Ray r, const Point* wheres, char** ceiling)
{
    for(int x = r.projection.clamped.top; x < sl.sdl.yres; x++)
    {
        const SDL_Surface* const surface = sl.sdl.surfaces.surface[tile(wheres[x], ceiling)];
        const int row = surface->h * xdec(wheres[x].y);
        const int col = surface->w * xdec(wheres[x].x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        sl.display.pixels[x + sl.y * sl.display.width] = pixels[col + row * surface->w];
    }
}

// Modify a pixel
static void mod(uint32_t* const pixel, const float m)
{
    const int rm = (m / 0xFF) * ((*pixel >> 0x10) & 0xFF);
    const int gm = (m / 0xFF) * ((*pixel >> 0x08) & 0xFF);
    const int bm = (m / 0xFF) * ((*pixel >> 0x00) & 0xFF);
    *pixel = rm << 0x10 | gm << 0x08 | bm << 0x00;
}

// Apply lighting for one scanline
static void light(const Scanline sl, const Ray r, Point* wheres, const Torch t, int* moddings)
{
    for(int x = 0; x < r.projection.clamped.bot; x++)
    {
        const int xx = sl.sdl.yres - 1 - x;
        // Flooring
        const float distance = mag(sub(wheres[xx], r.traceline.trace.a));
        moddings[x] = illuminate(t, distance);
        // Ceiling
        moddings[xx] = moddings[x];
    }
    // Walling
    for(int x = r.projection.clamped.bot; x < r.projection.clamped.top; x++)
    {
        const float distance = r.traceline.corrected.x;
        moddings[x] = illuminate(t, distance);
    }
    // Apply all calcs
    for(int x = 0; x < sl.sdl.yres; x++)
    {
        uint32_t* const pixel = &sl.display.pixels[x + sl.y * sl.display.width];
        mod(pixel, moddings[x]);
    }
}

// Render wall, floor, ceiling, and lighting, all for one scanline
void xrend(const Scanline sl, const Ray r, Point* wheres, const Map m, const Torch t, int* moddings)
{
    wrend(sl, r);
    frend(sl, r, wheres, m.floring);
    crend(sl, r, wheres, m.ceiling);
    light(sl, r, wheres, t, moddings);
}
