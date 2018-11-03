#include "Bundle.h"

#include "Scanline.h"

int b_raster(void* const bundle)
{
    Bundle* const b = (Bundle*) bundle;
    for(int x = b->a; x < b->b; x++)
    {
        const Point column = xlerp(b->camera, x / (float) b->sdl.xres);
        const Hits hits = xmarch(b->hero.where, column, b->map);
        const Scanline scanline = { b->sdl, b->vram.pixels, b->vram.width, x };
        b->zbuff[x] = xraster(scanline, hits, b->hero, b->current, b->clouds, b->map);
    }
    return 0;
}
