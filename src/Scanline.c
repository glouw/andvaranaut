#include "Scanline.h"

#include "util.h"

//
// Color Lookup Table.
//

static uint32_t** clut;

static const uint32_t palette[] = {
    //
    // These two x marks below represent which bits to use for the hash
    // function as they are unique.
    //
    //---xx-
    0x000000,
    0x140C1C,
    0x30346D,
    0x346524,
    0x452434,
    0x4D494D,
    0x597DCF,
    0x6DAA2C,
    0x6DC3CB,
    0x757161,
    0x8696A2,
    0x864D30,
    0xD34549,
    0xD37D2C,
    0xD3AA9A,
    0xDBD75D,
    0xDFEFD7,
};

static uint8_t hash(const uint32_t color)
{
    return color >> 4;
}

static const int shades = 256;

static uint32_t shade_pixel(const uint32_t pixel, const int shading)
{
    const uint32_t r = (((pixel >> 0x10) /****/) * shading) / shades;
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * shading) / shades;
    const uint32_t b = (((pixel /*****/) & 0xFF) * shading) / shades;

    return r << 0x10 | g << 0x08 | b;
}

void s_init(void)
{
    //
    // 8-bit color lookup table hashing reveals 256 bins.
    //

    clut = u_toss(uint32_t*, 256);

    //
    // Each bin holds a number of precomputed pixel shades.
    //

    const int colors = u_len(palette);

    for(int i = 0; i < colors; i++)
    {
        const uint32_t color = palette[i];

        const uint8_t index = hash(color);
        clut[index] = u_toss(uint32_t, shades);

        for(int j = 0; j < shades; j++)
            clut[index][j] = shade_pixel(color, j);
    }
}

static uint32_t get_pixel(const SDL_Surface* const surface, const Point offset)
{
    const int row = surface->h * u_dec(offset.y);
    const int col = surface->w * u_dec(offset.x);

    const uint32_t* const pixels = (uint32_t*) surface->pixels;

    return pixels[col + row * surface->w];
}

static void set_pixel(const Scanline sl, const int x, const uint32_t pixel)
{
    sl.pixels[x + sl.y * sl.width] = pixel;
}

static void pixel_xfer(const Scanline sl, const int x, const Point offset, const int tile, const int shade)
{
    const uint32_t color = get_pixel(sl.sdl.surfaces.surface[tile], offset);

    const uint8_t index = hash(color);

    const uint32_t pixel = clut[index][shade];

    set_pixel(sl, x, pixel);
}

static void raster_wall(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        const float xx = (x - r.proj.bot) / r.proj.size;

        const float yy = r.offset;

        const Point offset = { xx, yy };

        const int shade = t_illuminate(r.torch, r.corrected.x);

        pixel_xfer(sl, x, offset, r.surface, shade);
    }
}

static void raster_flor(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const float percentage = p_flor_cast(r.proj, x);

        const Point offset = l_lerp(r.trace, percentage);

        const int tile = p_tile(offset, map.floring);

        if(!tile)
            continue;

        const float distance = p_mag(p_sub(offset, r.trace.a));

        const int shade = t_illuminate(r.torch, distance);

        pixel_xfer(sl, x, offset, tile, shade);
    }
}

static void raster_ceil(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        const float percentage = p_ceil_cast(r.proj, x);

        const Point offset = l_lerp(r.trace, percentage);

        const int tile = p_tile(offset, map.ceiling);

        if(!tile)
            continue;

        const float distance = p_mag(p_sub(offset, r.trace.a));

        const int shade = t_illuminate(r.torch, distance);

        pixel_xfer(sl, x, offset, tile, shade);
    }
}

static void raster_sky(const Scanline sl, const Ray r, const Map map, const int floor, const Flow clouds)
{
    if(floor == 0)
    {
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const Sheer sa = { 0.0f, clouds.height };

            const float percentage = p_ceil_cast(p_sheer(r.proj, sa), x);

            const Point a = l_lerp(r.trace, percentage);

            const float distance = p_mag(p_sub(a, r.trace.a));

            const int shade = t_illuminate(r.torch, distance);

            pixel_xfer(sl, x, p_div(p_abs(p_sub(a, clouds.where)), 8.0f), '&' - ' ', shade);
        }
    }
    else
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const float percentage = p_ceil_cast(r.proj, x);

            const Point offset = l_lerp(r.trace, percentage);

            if(p_tile(offset, map.ceiling))
                continue;

            const float distance = p_mag(p_sub(offset, r.trace.a));

            const int shade = t_illuminate(r.torch, distance);

            pixel_xfer(sl, x, offset, '#' - ' ', shade);
        }
}

static void raster_pit(const Scanline sl, const Ray r, const Map map, const Flow current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const float percentage = p_flor_cast(r.proj, x);

        const Point offset = l_lerp(r.trace, percentage);

        if(p_tile(offset, map.floring))
            continue;

        const float distance = p_mag(p_sub(offset, r.trace.a));

        const int shade = t_illuminate(r.torch, distance);

        pixel_xfer(sl, x, p_abs(p_sub(offset, current.where)), '%' - ' ', shade);
    }
}

static void raster_upper_section(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow clouds)
{
    int link = 0;
    for(Hit* hit = hits.ceiling, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;

        const Ray ray = h_cast(hero, *which, map.top, sl.sdl.yres, sl.sdl.xres);

        if(link++ == 0)
            raster_sky(sl, ray, map, hero.floor, clouds);

        raster_wall(sl, ray);
    }
}

static void raster_lower_section(const Scanline sl, const Hits hits, const Hero hero, const Map map, const Flow current)
{
    int link = 0;
    for(Hit* hit = hits.floring, *next; hit; next = hit->next, free(hit), hit = next)
    {
        const Hit* const which = hit;

        const Sheer sheer = { current.height, -1.0f };

        const Ray ray = h_cast(hero, *which, sheer, sl.sdl.yres, sl.sdl.xres);

        if(link++ == 0)
            raster_pit(sl, ray, map, current);

        raster_wall(sl, ray);
    }
}

static Point raster_middle_section(const Scanline sl, const Hits hits, const Hero hero, const Map map)
{
    const Ray ray = h_cast(hero, hits.walling, map.mid, sl.sdl.yres, sl.sdl.xres);

    raster_wall(sl, ray);
    raster_flor(sl, ray, map);
    raster_ceil(sl, ray, map);

    return ray.corrected;
}

Point s_raster(const Scanline sl, const Hits hits, const Hero hero, const Flow current, const Flow clouds, const Map map)
{
    raster_upper_section(sl, hits, hero, map, clouds);

    raster_lower_section(sl, hits, hero, map, current);

    return raster_middle_section(sl, hits, hero, map);
}
