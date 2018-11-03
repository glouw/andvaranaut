#include "Bundle.h"

#include "Scanline.h"

int b_raster(void* const bundle)
{
    Bundle* const b = (Bundle*) bundle;
    for(int x = b->a; x < b->b; x++)
    {
        const Point column = l_lerp(b->camera, x / (float) b->sdl.xres);
        const Hits hits = h_march(b->hero.where, column, b->map);
        const Scanline scanline = { b->sdl, b->vram.pixels, b->vram.width, x };
        b->zbuff[x] = s_raster(scanline, hits, b->hero, b->current, b->clouds, b->map);
    }
    return 0;
}
