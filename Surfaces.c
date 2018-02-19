#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path, const uint32_t key)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    if(bmp == NULL)
        xbomb("%s\n", SDL_GetError());
    // Notice how RGB888 is used here contrary to the ARGB8888 of the rendering canvas.
    // Color keys can not be set in SDL when an alpha channel is present.
    SDL_PixelFormat* const allocation = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    const uint32_t r = 0xFF & (key >> 0x10);
    const uint32_t g = 0xFF & (key >> 0x08);
    const uint32_t b = 0xFF & (key >> 0x00);
    SDL_SetColorKey(converted, SDL_TRUE, SDL_MapRGB(converted->format, r, g, b));
    return converted;
}

Surfaces xpull(const uint32_t key)
{
    static const char* const names[] = {
        /*   */ "art/delete.bmp",
        /* ! */ "art/error.bmp",
        /* " */ "art/floor.bmp",
        /* # */ "art/wall.bmp",
        /* $ */ "art/ceiling.bmp",
        /* % */ "art/water.bmp",
        /* & */ "art/cloud.bmp",
        /* ' */ "art/board.bmp",
        /* ( */ "art/grass.bmp",
        /* ) */ "art/dirt.bmp",
        /* * */ "art/cloud2.bmp",
        /* + */ "art/error.bmp",
        /* , */ "art/error.bmp",
        /* - */ "art/error.bmp",
        /* . */ "art/error.bmp",
        /* / */ "art/error.bmp",
        /* 0 */ "art/error.bmp",
        /* 1 */ "art/error.bmp",
        /* 2 */ "art/error.bmp",
        /* 3 */ "art/error.bmp",
        /* 4 */ "art/error.bmp",
        /* 5 */ "art/error.bmp",
        /* 6 */ "art/error.bmp",
        /* 7 */ "art/error.bmp",
        /* 8 */ "art/error.bmp",
        /* 9 */ "art/error.bmp",
        /* : */ "art/error.bmp",
        /* ; */ "art/error.bmp",
        /* < */ "art/error.bmp",
        /* = */ "art/error.bmp",
        /* > */ "art/error.bmp",
        /* ? */ "art/error.bmp",
        /* @ */ "art/error.bmp",
        /* A */ "art/missing.bmp",
        /* B */ "art/missing.bmp",
        /* C */ "art/missing.bmp",
        /* D */ "art/missing.bmp",
        /* E */ "art/missing.bmp",
        /* F */ "art/missing.bmp",
        /* G */ "art/missing.bmp",
        /* H */ "art/missing.bmp",
        /* I */ "art/missing.bmp",
        /* J */ "art/missing.bmp",
        /* K */ "art/missing.bmp",
        /* L */ "art/missing.bmp",
        /* M */ "art/missing.bmp",
        /* N */ "art/missing.bmp",
        /* O */ "art/missing.bmp",
        /* P */ "art/missing.bmp",
        /* Q */ "art/missing.bmp",
        /* R */ "art/missing.bmp",
        /* S */ "art/missing.bmp",
        /* T */ "art/missing.bmp",
        /* U */ "art/missing.bmp",
        /* V */ "art/missing.bmp",
        /* W */ "art/missing.bmp",
        /* X */ "art/missing.bmp",
        /* Y */ "art/missing.bmp",
        /* Z */ "art/missing.bmp",
        /* [ */ "art/missing.bmp",
        /* \ */ "art/error.bmp",
        /* ] */ "art/missing.bmp",
        /* ^ */ "art/error.bmp",
        /* _ */ "art/error.bmp",
        /* ` */ "art/error.bmp",
        /* a */ "art/flower.bmp",
        /* b */ "art/outlaw.bmp",
        /* c */ "art/missing.bmp",
        /* d */ "art/lootbag.bmp",
        /* e */ "art/tree.bmp",
        /* f */ "art/treeleaves.bmp",
        /* g */ "art/missing.bmp",
        /* h */ "art/missing.bmp",
        /* i */ "art/missing.bmp",
        /* j */ "art/missing.bmp",
        /* k */ "art/missing.bmp",
        /* l */ "art/missing.bmp",
        /* m */ "art/missing.bmp",
        /* n */ "art/missing.bmp",
        /* o */ "art/missing.bmp",
        /* p */ "art/missing.bmp",
        /* q */ "art/missing.bmp",
        /* r */ "art/missing.bmp",
        /* s */ "art/missing.bmp",
        /* t */ "art/missing.bmp",
        /* u */ "art/missing.bmp",
        /* v */ "art/missing.bmp",
        /* w */ "art/missing.bmp",
        /* x */ "art/missing.bmp",
        /* y */ "art/missing.bmp",
        /* z */ "art/missing.bmp",
        /* { */ "art/error.bmp",
        /* | */ "art/error.bmp",
        /* } */ "art/error.bmp",
        /* ~ */ "art/trap_door.bmp",
    };
    const int count = xlen(names);
    SDL_Surface** const surface = xtoss(SDL_Surface*, count);
    for(int i = 0; i < count; i++)
        surface[i] = load(names[i], key);
    const Surfaces surfaces = { surface, count };
    return surfaces;
}

void xclean(const Surfaces surfaces)
{
    for(int i = 0; i < surfaces.count; i++)
        SDL_FreeSurface(surfaces.surface[i]);
    free(surfaces.surface);
}
