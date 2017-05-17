#include "Sliver.h"

#include "Line.h"
#include "Point.h"
#include "Util.h"
#include "Torch.h"

// Software implementation of the SDL2 Texture Color Mod function - Discards alpha
static uint32_t mod(const uint32_t pixel, const int r, const int g, const int b)
{
    const int rm = r / (float) 0xFF * (uint8_t) (pixel >> 0x10);
    const int gm = g / (float) 0xFF * (uint8_t) (pixel >> 0x08);
    const int bm = b / (float) 0xFF * (uint8_t) (pixel >> 0x00);
    return rm << 0x10 | gm << 0x08 | bm << 0x00;
}

// Wall renderer
void wrend(const Sliver sliver, const Hit hit, const int modding)
{
    // Aliases
    const int y = sliver.scanline.y;
    const int width = sliver.scanline.display.width;
    // Paint
    const SDL_Surface* const surface = sliver.scanline.sdl.surfaces.surface[hit.surface];
    const int row = surface->h * hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = sliver.wall.clamped.bot; x < sliver.wall.clamped.top; x++)
    {
        const float offset = (x - sliver.wall.bot) / (float) (sliver.wall.top - sliver.wall.bot);
        const int col = surface->w * offset;
        const uint32_t pixel = pixels[col + row * surface->w];
        sliver.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
    }
}

// Floor renderer
void frend(const Sliver sliver, char** const floring, Point* wheres, int* moddings, const Tracery tracery)
{
    // Aliases
    const int width = sliver.scanline.display.width;
    const int y = sliver.scanline.y;
    // Paint
    for(int x = 0; x < sliver.wall.clamped.bot; x++)
    {
        const int log = sliver.scanline.sdl.res - 1 - x;
        const Point where = lerp(tracery.traceline.trace, tracery.party[x] / tracery.traceline.corrected.x);
        // Save
        wheres[log] = where;
        const int seen = tile(where, floring);
        const SDL_Surface* const surface = sliver.scanline.sdl.surfaces.surface[seen];
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const int row = surface->h * dec(where.y);
        const int col = surface->w * dec(where.x);
        const uint32_t pixel = pixels[col + row * surface->w];
        const int modding = illuminate(tracery.torch, mag(sub(where, tracery.traceline.trace.a)));
        // Save
        moddings[log] = modding;
        sliver.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
    }
}

// Ceiling renderer - Saves time by using some of frend()'s calculations
void crend(const Sliver sliver, char** const ceiling, Point* wheres, int* moddings)
{
    // Aliases
    const int y = sliver.scanline.y;
    const int width = sliver.scanline.display.width;
    for(int x = sliver.wall.clamped.top; x < sliver.scanline.sdl.res; x++)
    {
        const Point where = wheres[x];
        // Offsets
        const SDL_Surface* const surface = sliver.scanline.sdl.surfaces.surface[tile(where, ceiling)];
        const int row = surface->h * dec(where.y);
        const int col = surface->w * dec(where.x);
        const int modding = moddings[x];
        // Paint
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const uint32_t pixel = pixels[col + row * surface->w];
        sliver.scanline.display.pixels[x + y * width] = mod(pixel, modding, modding, modding);
    }
}
