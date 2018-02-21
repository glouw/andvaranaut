#include "Scanline.h"

#include "util.h"

// Modulous modify a pixel. Discards alpha. Great for pixel shading
static uint32_t shade(const uint32_t pixel, const int shading)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * shading) >> 0x08; // Shift right by 0x08 is same as
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * shading) >> 0x08; // dividing by 256. Somehow
    const uint32_t b = (((pixel /*****/) & 0xFF) * shading) >> 0x08; // ofast was not catching this.
    return r << 0x10 | g << 0x08 | b;
}

// Pixel getter.
static uint32_t pget(const SDL_Surface* const surface, const Point offset)
{
    const int row = surface->h * xdec(offset.y);
    const int col = surface->w * xdec(offset.x);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    return pixels[col + row * surface->w];
}

// Pixel putter.
static void pput(const Scanline sl, const int x, const uint32_t pixel)
{
    sl.pixels[x + sl.y * sl.width] = pixel;
}

static void xfer(const Scanline sl, const int x, const Point offset, const int tile, const int distance)
{
    const uint32_t color = pget(sl.sdl.surfaces.surface[tile], offset);
    if(color == sl.sdl.key)
        return;
    pput(sl, x, shade(color, distance));
}

// Wall rasterer.
static void rwall(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.offset };
        xfer(sl, x,
            offset,
            r.surface,
            xilluminate(r.torch, r.corrected.x));
    }
}

// Floor rasterer.
static void rflor(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        const int tile = xtile(offset, map.floring);
        if(!tile)
            continue;
        xfer(sl, x,
            offset,
            tile,
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Ceiling rasterer.
static void rceil(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        const Point offset = xlerp(r.trace, xccast(r.proj, x));
        const int tile = xtile(offset, map.ceiling);
        if(!tile)
            continue;
        xfer(sl, x,
            offset,
            tile,
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Sky rasterer.
static void rsky(const Scanline sl, const Ray r, const Map map, const int floor, const Flow clouds)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        // Zeroth floor renders sky.
        // There are two sky layers: The forground layer, and the behind layer.
        // The foreground layer is closer, the behind layer is further.
        if(floor == 0)
        {
            const Point a = xlerp(r.trace, xccast(xsheer(r.proj, 0.0f, clouds.height / 1.0f), x));
            const Point b = xlerp(r.trace, xccast(xsheer(r.proj, 0.0f, clouds.height / 1.5f), x));
            // Scale multiply enlargen clouds.
            const float scale = 6.0f;
            xfer(sl, x, xdiv(xabs(xsub(a, clouds.where)), scale), '&' - ' ', xilluminate(r.torch, xmag(xsub(a, r.trace.a))));
            xfer(sl, x, xdiv(xabs(xsub(b, clouds.where)), scale), '*' - ' ', xilluminate(r.torch, xmag(xsub(b, r.trace.a))));
        }
        // Remaining floors render a second ceiling instead.
        else
        {
            const Point offset = xlerp(r.trace, xccast(r.proj, x));
            if(xtile(offset, map.ceiling))
                continue;
            xfer(sl, x,
                offset,
                '#' - ' ',
                xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
        }
    }
}

// Pit water rasterer.
static void rpit(const Scanline sl, const Ray r, const Map map, const Flow current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        if(xtile(offset, map.floring))
            continue;
        xfer(sl, x,
            xabs(xsub(offset, current.where)),
            '%' - ' ',
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Upper level rasterer (second ceiling and upper walls).
static void rupper(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow clouds)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        // TODO: Maybe randomize the height? Maybe random per level?
        const Ray ray = xcalc(hero, *which, 2.0f, 3.0f, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            rsky(sl, ray, map, hero.floor, clouds);
        rwall(sl, ray);
    }
}

// Lower level rasterer (pit and lower walls).
static void rlower(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow current)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Ray ray = xcalc(hero, *which, current.height, -1.0f, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            rpit(sl, ray, map, current);
        rwall(sl, ray);
    }
}

// Middle level rasterer. Returns a z-buffer element for the sprites.
static Point rmiddle(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = xcalc(hero, hits.walling, 0.0f, 1.0f, sl.sdl.yres, sl.sdl.xres);
    rwall(sl, ray);
    rflor(sl, ray, map);
    rceil(sl, ray, map);
    return ray.corrected;
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Flow current, const Flow clouds, const Map map)
{
    // Highlighter for finding missing pixels.
    if(true)
        for(int x = 0; x < sl.sdl.yres; x++) pput(sl, x, 0xFF0000);
    rupper(sl, hits, hero, map, clouds);
    rlower(sl, hits, hero, map, current);
    return rmiddle(sl, hits, hero, map);
}
