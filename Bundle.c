#include "Bundle.h"

#include "Scanline.h"

int xbraster(void* const bundle)
{
    Bundle* const b = (Bundle*) bundle;
    for(int x = b->a; x < b->b; x++)
    {
        const Point column = xlerp(b->g.camera, x / (float) b->g.sdl.xres);
        const Hits hits = xmarch(b->g.hero.where, column, b->g.map);
        const Scanline scanline = { b->g.sdl, b->g.display, x };
        b->g.zbuff[x] = xraster(scanline, hits, b->g.hero, b->g.current, b->g.map);
    }
    return 0;
}
