#include "Boundary.h"
#include "Line.h"
#include "Point.h"
#include "Util.h"

void wrend(const Boundary boundary, const Hit hit)
{
    // The ceiling wall renderer will cover up any rendered upper walls so there is no need
    // to render the upper wall if a neighboring ceiling block is present.
    if(hit.neighbor) return;
    // Note that lower walls will never have a neighboring block, making this wall renderer
    // useful for both upper and lower walls.
    const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[hit.tile];
    const int row = (surface->h - 1) * hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = boundary.wall.clamped.bot; x < boundary.wall.clamped.top; x++)
    {
        const int col = (surface->w - 1) * (x - boundary.wall.bot) / (boundary.wall.top - boundary.wall.bot);
        const int y = boundary.scanline.y;
        const int width = boundary.scanline.display.width;
        boundary.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
    }
}

void frend(const Boundary boundary, Point* const wheres, char** const floring, const Tracery tracery)
{
    for(int x = 0; x < boundary.wall.clamped.bot; x++)
    {
        const Point where = wheres[boundary.scanline.gpu.res - 1 - x] =
            lerp(tracery.traceline.trace, tracery.party[x] / tracery.traceline.corrected.x);
        const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[tile(where, floring)];
        const int row = (surface->h - 1) * dec(where.y);
        const int col = (surface->w - 1) * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        const int y = boundary.scanline.y;
        const int width = boundary.scanline.display.width;
        boundary.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
    }
}

void crend(const Boundary boundary, Point* const wheres, char** const ceiling)
{
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.gpu.res; x++)
    {
        // The ceiling must only be drawn if present.
        const Point where = wheres[x];
        if(tile(where, ceiling))
        {
            const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface[tile(where, ceiling)];
            const int row = (surface->h - 1) * dec(where.y);
            const int col = (surface->w - 1) * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            const int y = boundary.scanline.y;
            const int width = boundary.scanline.display.width;
            boundary.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
        }
    }
}

void srend(const Boundary boundary, const float percent)
{
    const SDL_Surface* const surface = boundary.scanline.gpu.surfaces.surface['~' - ' '];
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const int mid = boundary.scanline.gpu.res / 2;
    const float ratio = (surface->w - 1) / (float) mid;
    const int offset = surface->h * percent;
    const int corrected = ratio * boundary.scanline.y;
    const int row = (corrected + offset) % surface->h;
    for(int x = boundary.wall.clamped.top; x < boundary.scanline.gpu.res; x++)
    {
        const int col = ratio * (x - mid);
        const int y = boundary.scanline.y;
        const int width = boundary.scanline.display.width;
        boundary.scanline.display.pixels[x + y * width] = pixels[col + row * surface->w];
    }
}
