#include "Scanline.h"

#include "util.h"

void wrend(const Scanline scanline, const Ray ray, const Torch torch, int* const moddings)
{
    const SDL_Surface* const surface = scanline.sdl.surfaces.surface[ray.hit.surface];
    const int row = surface->h * ray.hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const float height = ray.projection.top - ray.projection.bot;
    for(int x = ray.projection.clamped.bot; x < ray.projection.clamped.top; x++)
    {
        const float offset = (x - ray.projection.bot) / height;
        const int col = surface->w * offset;
        scanline.display.pixels[x + scanline.y * scanline.display.width] =
            pixels[col + row * surface->w];
    }
    // Save projection light modding
    const int modding = illuminate(torch, ray.traceline.corrected.x);
    for(int x = ray.projection.clamped.bot; x < ray.projection.clamped.top; x++)
        moddings[x] = modding;
}

void frend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres,
    const Hero hero, int* const moddings)
{
    for(int x = 0; x < ray.projection.clamped.bot; x++)
    {
        const int log = scanline.sdl.res - 1 - x;
        const float party = fcast(hero.fov, scanline.sdl.res, x);
        const Point where = wheres[log] =
            lerp(ray.traceline.trace, party / ray.traceline.corrected.x);
        const int seen = tile(where, map.floring);
        const SDL_Surface* const surface = scanline.sdl.surfaces.surface[seen];
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const int row = surface->h * dec(where.y);
        const int col = surface->w * dec(where.x);
        scanline.display.pixels[x + scanline.y * scanline.display.width] =
            pixels[col + row * surface->w];
        // Save floor and ceiling light modding
        const int modding = illuminate(hero.torch,
                mag(sub(where, ray.traceline.trace.a)));
        moddings[x] = moddings[log] = modding;
    }
}

void crend(const Scanline scanline, const Ray ray, const Map map, Point* const wheres)
{
    for(int x = ray.projection.clamped.top; x < scanline.sdl.res; x++)
    {
        const Point where = wheres[x];
        const SDL_Surface* const surface = scanline.sdl.surfaces.surface[tile(where, map.ceiling)];
        const int row = surface->h * dec(where.y);
        const int col = surface->w * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] =
            pixels[col + row * surface->w];
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

void light(const Scanline scanline, int* const moddings)
{
    for(int x = 0; x < scanline.sdl.res; x++)
        mod(&scanline.display.pixels[x + scanline.y * scanline.display.width], moddings[x]);
}
