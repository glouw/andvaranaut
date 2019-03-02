#include "Scanline.h"

#include "util.h"

//
// A precomputed color lookup table will hold 256 shades for each color in the color palette.
//

static const int shades = 256;

static uint32_t** color_table = NULL;

static const uint32_t color_palette[] = {
    0x00000000, 0x00140C1C, 0x00452434, 0x0030346D, 0x004D494D, 0x00864D30,
    0x00346524, 0x00D34549, 0x00757161, 0x00597DCF, 0x00D37D2C, 0x008696A2,
    0x006DAA2C, 0x00D3AA9A, 0x006DC3CB, 0x00DBD75D, 0x00DFEFD7, 0x0000FFFF,
};

//
// The shading function is a mockup of SDL_SetTextureColorMod.
//

static uint32_t shade_pixel(const uint32_t pixel, const int shading)
{
    const uint32_t r = (((pixel >> 0x10) /*--*/) * shading) / shades;
    const uint32_t g = (((pixel >> 0x08) & 0xFF) * shading) / shades;
    const uint32_t b = (((pixel /*---*/) & 0xFF) * shading) / shades;
    return r << 0x10 | g << 0x08 | b;
}

//
// Indexing a color table shade is done with an 8 bit hash of the color itself.
//

static int hash(const uint32_t color)
{
    return (color >> 4) & 0xFF;
}

void s_init_color_table(void)
{
    //
    // 8-bit color lookup table hashing reveals 2 to the power of 8 bins.
    //

    const int bins = 256;
    color_table = u_toss(uint32_t*, bins);

    for(int i = 0; i < u_len(color_palette); i++)
    {
        const uint32_t color = color_palette[i];
        const int index = hash(color);
        color_table[index] = u_toss(uint32_t, shades);
        for(int j = 0; j < shades; j++)
            color_table[index][j] = shade_pixel(color, j);
    }
}

void s_save_color_pallete_as_pal(void)
{
    FILE* const file = fopen("art/dawn.pal", "w");

    //
    // Save the JASC header.
    //

    const char* header[] = { "JASC-PAL", "0100", "256" };
    for(int i = 0; i < u_len(header); i++)
        fprintf(file, "%s\n", header[i]);

    //
    // Dump the color table.
    //

    for(int i = 0; i < u_len(color_palette); i++)
    {
        const uint32_t color = color_palette[i];
        fprintf(file, "%d %d %d\n",
            (color >> 0x10) & 0xFF,
            (color >> 0x08) & 0xFF,
            (color >> 0x00) & 0xFF);
    }

    //
    // GRAFX2 expects a little padding (black will do) for its color palette.
    //

    const int padding = 136;
    for(int i = 0; i < padding; i++)
        fprintf(file, "0 0 0\n");

    fclose(file);
}

static void put_pixel(const Scanline sl, const int x, const Point offset, const int tile, const int shade)
{
    SDL_Surface* const surface = sl.sdl.surfaces.surface[tile];
    const int row = surface->h * u_dec(offset.y);
    const int col = surface->w * u_dec(offset.x);
    const uint32_t* const pixels = (uint32_t*) surface->pixels;
    const uint32_t color = pixels[col + row * surface->w];
    const int index = hash(color);
    const uint32_t pixel = color_table[index][shade];
    sl.pixels[x + sl.y * sl.width] = pixel;
}

static void put_shaded_pixel(const Scanline sl, const int x, const Point offset, const int tile, const int light, const float distance)
{
    const int shade = t_illuminate(light, distance);
    put_pixel(sl, x, offset, tile, shade);
}

static void raster_wall(const Scanline sl, const Ray r)
{
    for(int x = r.proj.clamped.bot; x < r.proj.clamped.top; x++)
    {
        const float xx = (x - r.proj.bot) / r.proj.size;
        const float yy = r.offset;
        const Point offset = { xx, yy };
        put_shaded_pixel(sl, x, offset, r.surface, r.torch.light, r.corrected.x);
    }
}

static void raster_flor(const Scanline sl, const Ray r, const Map map)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const float percentage = p_flor_cast(r.proj, x);
        const Point offset = l_lerp(r.trace, percentage);
        const int tile = p_tile(offset, map.floring);

        if(tile)
        {
            const float distance = p_mag(p_sub(offset, r.trace.a));
            put_shaded_pixel(sl, x, offset, tile, r.torch.light, distance);
        }
    }
}

static void raster_ceil(const Scanline sl, const Ray r, const Map map)
{
    for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
    {
        const float percentage = p_ceil_cast(r.proj, x);
        const Point offset = l_lerp(r.trace, percentage);
        const int tile = p_tile(offset, map.ceiling);

        if(tile)
        {
            const float distance = p_mag(p_sub(offset, r.trace.a));
            put_shaded_pixel(sl, x, offset, tile, r.torch.light, distance);
        }
    }
}

static void raster_sky(const Scanline sl, const Ray r, const Map map, const int floor, const Flow clouds)
{
    if(floor == 0)
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const Sheer sa = { 0.0f, clouds.height };
            const float percentage = p_ceil_cast(p_sheer(r.proj, sa), x);
            const Point a = l_lerp(r.trace, percentage);
            const float distance = p_mag(p_sub(a, r.trace.a));
            const Point new_offset = p_div(p_abs(p_sub(a, clouds.where)), 8.0f);

            //
            // The zeroth floor will always use a sky texture.
            //

            const int tile = '&' - ' ';

            put_shaded_pixel(sl, x, new_offset, tile, r.torch.light, distance);
        }
    else
        for(int x = r.proj.clamped.top; x < sl.sdl.yres; x++)
        {
            const float percentage = p_ceil_cast(r.proj, x);
            const Point offset = l_lerp(r.trace, percentage);

            if(!p_tile(offset, map.ceiling))
            {
                const float distance = p_mag(p_sub(offset, r.trace.a));

                //
                // The remaining floors will always use a generic tile (like stone).
                //

                const int tile = '#' - ' ';

                put_shaded_pixel(sl, x, offset, tile, r.torch.light, distance);
            }
        }
}

static void raster_pit(const Scanline sl, const Ray r, const Map map, const Flow current)
{
    for(int x = 0; x < r.proj.clamped.bot; x++)
    {
        const float percentage = p_flor_cast(r.proj, x);
        const Point offset = l_lerp(r.trace, percentage);

        if(!p_tile(offset, map.floring))
        {
            const float distance = p_mag(p_sub(offset, r.trace.a));
            const Point new_offset = p_abs(p_sub(offset, current.where));

            //
            // A pit will always use a genreic tile like water (or maybe even lava).
            //

            const int tile = '%' - ' ';

            put_shaded_pixel(sl, x, new_offset, tile, r.torch.light, distance);
        }
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
