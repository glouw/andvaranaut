#include "Palette.h"

#include "util.h"

static const uint32_t colors[] = {
    0x00000000,
    0x00140C1C,
    0x00452434,
    0x0030346D,
    0x004D494D,
    0x00864D30,
    0x00346524,
    0x00D34549,
    0x00757161,
    0x00597DCF,
    0x00D37D2C,
    0x008696A2,
    0x006DAA2C,
    0x00D3AA9A,
    0x006DC3CB,
    0x00DBD75D,
    0x00DFEFD7,
    0x0000FFFF,
};

Palette p_make(void)
{
    static Palette zero;
    Palette pal = zero;
    pal.count = u_len(colors);
    pal.colors = u_toss(uint32_t, pal.count);

    for(int i = 0; i < pal.count; i++)
        pal.colors[i] = colors[i];

    pal.blk = pal.colors[ 0];
    pal.brn = pal.colors[ 5];
    pal.red = pal.colors[ 7];
    pal.yel = pal.colors[15];
    pal.wht = pal.colors[16];

    return pal;
}

void p_save(const Palette pal)
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

    for(int i = 0; i < pal.count; i++)
    {
        const uint32_t color = pal.colors[i];
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
