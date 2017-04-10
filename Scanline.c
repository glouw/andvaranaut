#include "Scanline.h"
#include "Line.h"
#include "Point.h"
#include "Util.h"

void wrend(const Scanline scanline, const Wall wall, const Hit hit)
{
    const SDL_Surface* const surface = scanline.gpu.surfaces.surface[hit.tile];
    const int row = (surface->h - 1) * hit.offset;
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    for(int x = wall.clamped.bot; x < wall.clamped.top; x++)
    {
        const int col = (surface->w - 1) * (x - wall.bot) / (wall.top - wall.bot);
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}

Point* frend(const Scanline scanline, const Wall wall, const Traceline traceline, char** const floring, float* party)
{
    Point* const wheres = (Point*) calloc(scanline.gpu.res, sizeof(*wheres));
    for(int x = 0; x < wall.clamped.bot; x++)
    {
        const Point where = wheres[scanline.gpu.res - 1 - x] = lerp(traceline.trace, party[x] / traceline.corrected.x);
        const SDL_Surface* const surface = scanline.gpu.surfaces.surface[tile(where, floring)];
        const int row = (surface->h - 1) * dec(where.y);
        const int col = (surface->w - 1) * dec(where.x);
        const uint32_t* const pixels = (uint32_t*) surface->pixels;
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
    return wheres;
}

void crend(const Scanline scanline, const Wall wall, const Point* const wheres, char** const ceiling)
{
    for(int x = wall.clamped.top; x < scanline.gpu.res; x++)
    {
        const Point where = wheres[x];
        if(tile(where, ceiling))
        {
            const SDL_Surface* const surface = scanline.gpu.surfaces.surface[tile(where, ceiling)];
            const int row = (surface->h - 1) * dec(where.y);
            const int col = (surface->w - 1) * dec(where.x);
            const uint32_t* const pixels = (uint32_t*) surface->pixels;
            scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
        }
    }
}

void brend(const Scanline scanline)
{
    for(int x = 0; x < scanline.gpu.res; x++)
        scanline.display.pixels[x + scanline.y * scanline.display.width] = 0x0;
}

void srend(const Scanline scanline, const float percent)
{
    const SDL_Surface* const surface = scanline.gpu.surfaces.surface['~' - ' '];
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const int mid = scanline.gpu.res / 2;
    const float ratio = (float) (surface->w - 1) / (float) mid;
    const int offset = surface->h * percent;
    const int corrected = ratio * scanline.y;
    const int row = (corrected + offset) % surface->h;
    for(int x = mid; x < scanline.gpu.res; x++)
    {
        const int col = ratio * (x - mid);
        scanline.display.pixels[x + scanline.y * scanline.display.width] = pixels[col + row * surface->w];
    }
}
