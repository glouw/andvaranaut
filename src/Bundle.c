#include "Bundle.h"

#include "Scanline.h"

// Bundle rasterer. Renders scanlines <a> to <b>. Thread safe.
int xbraster(void* const bundle)
{
    Bundle* const b = (Bundle*) bundle;
    for(int x = b->a; x < b->b; x++)
    {
        // For colums <a> to <b> of the display, all hits of the walling,
        // ceiling, and flooring are collected up until the first eye level wall.
        // The projections for these scanlines are calculated and rastered to the display.
        const Point column = xlerp(b->camera, x / (float) b->sdl.xres);
        const Hits hits = xmarch(b->hero.where, column, b->map);
        const Scanline scanline = { b->sdl, b->pixels, b->width, x };

        // The zbuffer for eye level walls is returned for the sprite rendering.
        b->zbuff[x] = xraster(scanline, hits, b->hero, b->current, b->clouds, b->map);
    }
    return 0;
}
