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
    sl.display.pixels[x + sl.y * sl.display.width] = pixel;
}

static void xfer(const Scanline sl, const int x, const Point offset, const int tile, const int distance)
{
    const uint32_t color = pget(sl.sdl.surfaces.surface[tile], offset);
    // Do not transfer if color matches color key.
    if(color == sl.sdl.key)
        return;
    pput(sl, x, shade(color, distance));
}

// Wall rasterer.
static void wraster(const Scanline sl, const Ray r)
{
    if(r.proj.clamped.bot < 0)
        xbomb("clamp bot error");
    if(r.proj.clamped.top >= sl.sdl.xres)
        xbomb("clamp top error");
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        const Point offset = { (x - r.proj.bot) / r.proj.size, r.offset };
        // Shade and transfer pixel.
        xfer(sl, x, offset, r.surface, xilluminate(r.torch, r.corrected.x));
    }
}

// Floor rasterer.
static void fraster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        const int tile = xtile(offset, map.floring);
        // Do not render where there will be a pit.
        if(!tile)
            continue;
        // Shade and transfer pixel.
        xfer(sl, x, offset, tile, xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Ceiling rasterer.
static void craster(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        const Point offset = xlerp(r.trace, xccast(r.proj, x));
        const int tile = xtile(offset, map.ceiling);
        // Do not render where there will a second ceiling.
        if(!tile)
            continue;
        // Shade and transfer pixel.
        xfer(sl, x, offset, tile, xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Second ceiling rasterer.
static void sraster(const Scanline sl, const Ray r, const Map map, const int floor, const Flow clouds)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        //// Zeroth floor renders sky.
        //if(floor == 0)
        //{
        //    // There are two sky layers: The forground layer, and the behind layer.
        //    // The foreground layer is closer, the behind layer is further.
        //    const Point hind = xlerp(r.trace, xccast(xstack(r.proj, clouds.height / 1.0f), x));
        //    const Point fore = xlerp(r.trace, xccast(xstack(r.proj, clouds.height / 2.0f), x));
        //    // Clouds are too smale. Scale multiply will enlargen clouds.
        //    const float scale = 6.0f;
        //    // The behind layer is slower. Division of flow position by some scalar is a good enough approximation.
        //    xfer(sl, x, xdiv(xabs(xsub(hind, xdiv(clouds.where, 3.0f))), scale), '&' - ' ', xilluminate(r.torch, xmag(xsub(hind, r.trace.a))));
        //    xfer(sl, x, xdiv(xabs(xsub(fore, xdiv(clouds.where, 1.0f))), scale), '*' - ' ', xilluminate(r.torch, xmag(xsub(fore, r.trace.a))));
        //}
        //else
        //{
            // Remaining floors render a second ceiling.
            const Point offset = xlerp(r.trace, xccast(r.proj, x));
            if(xtile(offset, map.ceiling))
                continue;
            xfer(sl, x,
                offset,
                // Second ceiling always uses the same texture.
                '#' - ' ',
                xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
        //}
    }
}

// Pit water rasterer.
static void praster(const Scanline sl, const Ray r, const Map map, const Flow current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const Point offset = xlerp(r.trace, xfcast(r.proj, x));
        // Do not render where there was a floor.
        if(xtile(offset, map.floring))
            continue;
        // Shade and transfer pixel.
        xfer(sl, x,
            xabs(xsub(offset, current.where)),
            // Pit water always uses the same texture.
            '%' - ' ',
            xilluminate(r.torch, xmag(xsub(offset, r.trace.a))));
    }
}

// Upper level rasterer (second ceiling and upper walls).
static void uraster(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow clouds)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        // TODO: Maybe randomize the height? Maybe random per level?
        const Ray ray = xcalc(hero, *which, 2.0f, 3.0f, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            sraster(sl, ray, map, hero.floor, clouds);
        wraster(sl, ray);
    }
}

// Lower level rasterer (pit and lower walls).
static void lraster(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow current)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;
        const Ray ray = xcalc(hero, *which, current.height, -1.0f, sl.sdl.yres, sl.sdl.xres);
        if(link++ == 0)
            praster(sl, ray, map, current);
        wraster(sl, ray);
    }
}

// Eye level rasterer. Returns a z-buffer element for the sprites.
static Point eraster(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = xcalc(hero, hits.walling, 0.0f, 1.0f, sl.sdl.yres, sl.sdl.xres);
    wraster(sl, ray);
    fraster(sl, ray, map);
    craster(sl, ray, map);
    return ray.corrected;
}

Point xraster(const Scanline sl, const Hits hits, const Hero hero, const Flow current, const Flow clouds, const Map map)
{
    // Highlighter.
    for(int x = 0; x < sl.sdl.yres; x++) pput(sl, x, 0xFF0000);
    uraster(sl, hits, hero, map, clouds);
    lraster(sl, hits, hero, map, current);
    return eraster(sl, hits, hero, map);
}
