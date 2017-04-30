#include "Boundary.h"

#include "Line.h"
#include "Point.h"
#include "Util.h"
#include "Light.h"

// Software implementation of the SDL2 Texture Color Mod function - Discards alpha
static uint32_t mod(const uint32_t pixel, const int r, const int g, const int b)
{
    const int rm = r / (float) 0xFF * (pixel >> 0x10 & 0xFF);
    const int gm = g / (float) 0xFF * (pixel >> 0x08 & 0xFF);
    const int bm = b / (float) 0xFF * (pixel >> 0x00 & 0xFF);
    return rm << 0x10 | gm << 0x08 | bm << 0x00;
}

// Wall renderer
void wrend(const Boundary boundary, const Hit hit, const int modding)
{
    if(hit.neighbor) return;
    const SDL_Surface* const surface = boundary.scanline.sdl.surfaces.surface[hit.tile];
    const int row = (surface->h - 1) * hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = boundary.wall.clamped.bot; x < boundary.wall.clamped.top; x++)
    {
        const int col = (surface->w - 1) * (x - boundary.wall.bot) / (boundary.wall.top - boundary.wall.bot);
        const int y = boundary.scanline.y; // Alias (to save horizontal screen space)
        const int width = boundary.scanline.display.width; // Alias
        const uint32_t pixel = pixels[col + row * surface->w];
        boundary.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
    }
}

// Floor renderer
void frend(const Boundary boundary, Point* const wheres, char** const floring, int* const moddings, const Tracery tracery)
{
    for(int x = 0; x < boundary.wall.clamped.bot; x++)
    {
        const int log = boundary.scanline.sdl.res - 1 - x;
        const Point where = wheres[log] = lerp(tracery.traceline.trace, tracery.party[x] / tracery.traceline.corrected.x);
        const SDL_Surface* const surface = boundary.scanline.sdl.surfaces.surface[tile(where, floring)];
        const int row = (surface->h - 1) * dec(where.y);
        const int col = (surface->w - 1) * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const int y = boundary.scanline.y; // Alias
        const int width = boundary.scanline.display.width; // Alias
        const uint32_t pixel = pixels[col + row * surface->w];
        const int modding = moddings[log] = illuminate(tracery.light, mag(sub(where, tracery.traceline.trace.a)));
        boundary.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
    }
}

// Ceiling renderer - Saves time by using some of frend()'s calculations
void crend(const Boundary boundary, Point* const wheres, char** const ceiling, int* const moddings)
{
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.sdl.res; x++)
    {
        // Only bother rendering what can be seen
        const Point where = wheres[x];
        if(tile(where, ceiling))
        {
            const SDL_Surface* const surface = boundary.scanline.sdl.surfaces.surface[tile(where, ceiling)];
            const int row = (surface->h - 1) * dec(where.y);
            const int col = (surface->w - 1) * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            const int y = boundary.scanline.y; // Alias
            const int width = boundary.scanline.display.width; // Alias
            const uint32_t pixel = pixels[col + row * surface->w];
            const int modding = moddings[x];
            boundary.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
        }
    }
}

// Sky renderer
void srend(const Boundary boundary, const Day day)
{
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.sdl.res; x++)
    {
        const float percentage = 1.0 - x / (float) boundary.scanline.sdl.res;
        const int shade = percentage * day.light;
        const int y = boundary.scanline.y; // Alias
        const int width = boundary.scanline.display.width; // Alias
        boundary.scanline.display.pixels[x + y * width] = shade << 0x10 | shade << 0x08 | shade;
    }
}
