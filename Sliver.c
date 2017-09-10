#include "Sliver.h"

#include "util.h"

void wrend(
    const Sliver sliver,
    const Torch torch,
    int* const moddings)
    {
        // Aliases
        const int y = sliver.scanline.y;
        const int width = sliver.scanline.display.width;
        const Surfaces surfaces = sliver.scanline.sdl.surfaces;
        const Wall wall = sliver.impact.wall;
        // Paint
        const SDL_Surface* const surface = surfaces.surface[sliver.impact.hit.surface];
        const int row = surface->h * sliver.impact.hit.offset;
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const float height = wall.top - wall.bot;
        for(int x = wall.clamped.bot; x < wall.clamped.top; x++)
        {
            const float offset = (x - wall.bot) / height;
            const int col = surface->w * offset;
            sliver.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
        }
        // Save wall light modding
        const int modding = illuminate(torch, sliver.impact.traceline.corrected.x);
        for(int x = wall.clamped.bot; x < wall.clamped.top; x++)
            moddings[x] = modding;
    }

void frend(
    const Sliver sliver,
    char** const floring,
    const Torch torch,
    Point* const wheres,
    int* const moddings,
    const float* party)
    {
        // Aliases
        const int width = sliver.scanline.display.width;
        const int y = sliver.scanline.y;
        const Traceline traceline = sliver.impact.traceline;
        // Paint
        for(int x = 0; x < sliver.impact.wall.clamped.bot; x++)
        {
            const int log = sliver.scanline.sdl.res - 1 - x;
            const Point where = wheres[log] = lerp(traceline.trace, party[x] / traceline.corrected.x);
            const int seen = tile(where, floring);
            const SDL_Surface* const surface = sliver.scanline.sdl.surfaces.surface[seen];
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            const int row = surface->h * dec(where.y);
            const int col = surface->w * dec(where.x);
            sliver.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
            // Save floor and ceiling light modding
            const int modding = illuminate(torch, mag(sub(where, traceline.trace.a)));
            moddings[x] = moddings[log] = modding;
        }
    }

void crend(
    const Sliver sliver,
    char** const ceiling,
    Point* wheres)
    {
        // Aliases
        const int y = sliver.scanline.y;
        const int width = sliver.scanline.display.width;
        const Surfaces surfaces = sliver.scanline.sdl.surfaces;
        // Paint
        for(int x = sliver.impact.wall.clamped.top; x < sliver.scanline.sdl.res; x++)
        {
            const Point where = wheres[x];
            const SDL_Surface* const surface = surfaces.surface[tile(where, ceiling)];
            const int row = surface->h * dec(where.y);
            const int col = surface->w * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            sliver.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
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

void light(const Sliver sliver, int* const moddings)
{
    // Aliases
    const int y = sliver.scanline.y;
    const int width = sliver.scanline.display.width;
    // Apply all lighting mods
    for(int x = 0; x < sliver.scanline.sdl.res; x++)
        mod(&sliver.scanline.display.pixels[x + y * width], moddings[x]);
}
