#include "Scanline.h"

#include "util.h"

void wrend(const Scanline scanline, const Ray ray)
{
    const SDL_Surface* const surface = scanline.sdl.surfaces.surface[ray.hit.surface];
    const int row = surface->h * ray.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = ray.projection.clamped.bot; x < ray.projection.clamped.top; x++)
    {
        const float offset = (x - ray.projection.bot) / (float) (ray.projection.top - ray.projection.bot);
        const int col = surface->w * offset;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}

void frend(const Scanline scanline, const Ray ray, Point* const wheres, const Map map)
{
    for(int x = 0; x < ray.projection.clamped.bot; x++)
    {
        const Point where = wheres[scanline.sdl.res - 1 - x] =
            lerp(ray.traceline.trace, fcast(ray.traceline.fov, scanline.sdl.res, x, ray.traceline.corrected.x));
        const SDL_Surface* const surface = scanline.sdl.surfaces.surface[tile(where, map.floring)];
        const int row = surface->h * dec(where.y);
        const int col = surface->w * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}

void crend(const Scanline scanline, const Ray ray, Point* const wheres, const Map map)
{
    for(int x = ray.projection.clamped.top; x < scanline.sdl.res; x++)
    {
        const SDL_Surface* const surface = scanline.sdl.surfaces.surface[tile(wheres[x], map.ceiling)];
        const int row = surface->h * dec(wheres[x].y);
        const int col = surface->w * dec(wheres[x].x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}

// Software version of SDL_SetTextureColorMod
static void mod(uint32_t* const pixel, const int m)
{
    const int rm = m / (float) 0xFF * (*pixel >> 0x10 & 0xFF);
    const int gm = m / (float) 0xFF * (*pixel >> 0x08 & 0xFF);
    const int bm = m / (float) 0xFF * (*pixel >> 0x00 & 0xFF);
    *pixel = rm << 0x10 | gm << 0x08 | bm << 0x00;
}

void light(const Scanline scanline, const Ray ray, Point* const wheres, const Torch torch, int* const moddings)
{
    for(int x = 0; x < ray.projection.clamped.bot; x++)
    {
        const int xx = scanline.sdl.res - 1 - x;
        // Flooring
        const float distance = mag(sub(wheres[xx], ray.traceline.trace.a));
        moddings[x] = illuminate(torch, distance);
        // Ceiling
        moddings[xx] = moddings[x];
    }
    // Walling
    for(int x = ray.projection.clamped.bot; x < ray.projection.clamped.top; x++)
    {
        const float distance = ray.traceline.corrected.x;
        moddings[x] = illuminate(torch, distance);
    }
    // Apply
    for(int x = 0; x < scanline.sdl.res; x++)
    {
        uint32_t* const pixel = &scanline.display.pixels[x + scanline.y * scanline.display.width];
        mod(pixel, moddings[x]);
    }
}
