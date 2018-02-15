#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path)
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
    SDL_SetColorKey(converted, SDL_TRUE, SDL_MapRGB(converted->format, 0x00, 0xFF, 0xFF));
    return converted;
}

Surfaces xpull()
{
    static const char* const names[] = {
        /*   */ "art/stat/delete.bmp",
        /* ! */ "art/stat/error.bmp",
        /* " */ "art/stat/floor.bmp",
        /* # */ "art/stat/wall.bmp",
        /* $ */ "art/stat/ceiling.bmp",
        /* % */ "art/stat/water.bmp",
        /* & */ "art/stat/cloud.bmp",
        /* ' */ "art/stat/board.bmp",
        /* ( */ "art/stat/grass.bmp",
        /* ) */ "art/stat/dirt.bmp",
        /* * */ "art/stat/error.bmp",
        /* + */ "art/stat/error.bmp",
        /* , */ "art/stat/error.bmp",
        /* - */ "art/stat/error.bmp",
        /* . */ "art/stat/error.bmp",
        /* / */ "art/stat/error.bmp",
        /* 0 */ "art/stat/error.bmp",
        /* 1 */ "art/stat/error.bmp",
        /* 2 */ "art/stat/error.bmp",
        /* 3 */ "art/stat/error.bmp",
        /* 4 */ "art/stat/error.bmp",
        /* 5 */ "art/stat/error.bmp",
        /* 6 */ "art/stat/error.bmp",
        /* 7 */ "art/stat/error.bmp",
        /* 8 */ "art/stat/error.bmp",
        /* 9 */ "art/stat/error.bmp",
        /* : */ "art/stat/error.bmp",
        /* ; */ "art/stat/error.bmp",
        /* < */ "art/stat/error.bmp",
        /* = */ "art/stat/error.bmp",
        /* > */ "art/stat/error.bmp",
        /* ? */ "art/stat/error.bmp",
        /* @ */ "art/stat/error.bmp",
        /* A */ "art/dyna/missing.bmp",
        /* B */ "art/dyna/missing.bmp",
        /* C */ "art/dyna/missing.bmp",
        /* D */ "art/dyna/missing.bmp",
        /* E */ "art/dyna/missing.bmp",
        /* F */ "art/dyna/missing.bmp",
        /* G */ "art/dyna/missing.bmp",
        /* H */ "art/dyna/missing.bmp",
        /* I */ "art/dyna/missing.bmp",
        /* J */ "art/dyna/missing.bmp",
        /* K */ "art/dyna/missing.bmp",
        /* L */ "art/dyna/missing.bmp",
        /* M */ "art/dyna/missing.bmp",
        /* N */ "art/dyna/missing.bmp",
        /* O */ "art/dyna/missing.bmp",
        /* P */ "art/dyna/missing.bmp",
        /* Q */ "art/dyna/missing.bmp",
        /* R */ "art/dyna/missing.bmp",
        /* S */ "art/dyna/missing.bmp",
        /* T */ "art/dyna/missing.bmp",
        /* U */ "art/dyna/missing.bmp",
        /* V */ "art/dyna/missing.bmp",
        /* W */ "art/dyna/missing.bmp",
        /* X */ "art/dyna/missing.bmp",
        /* Y */ "art/dyna/missing.bmp",
        /* Z */ "art/dyna/missing.bmp",
        /* [ */ "art/invr/Gui0.bmp",
        /* \ */ "art/stat/error.bmp",
        /* ] */ "art/invr/Gui1.bmp",
        /* ^ */ "art/stat/error.bmp",
        /* _ */ "art/stat/error.bmp",
        /* ` */ "art/stat/error.bmp",
        /* a */ "art/dyna/flower.bmp",
        /* b */ "art/dyna/outlaw.bmp",
        /* c */ "art/dyna/missing.bmp",
        /* d */ "art/dyna/lootbag.bmp",
        /* e */ "art/dyna/tree.bmp",
        /* f */ "art/dyna/treeleaves.bmp",
        /* g */ "art/dyna/missing.bmp",
        /* h */ "art/dyna/missing.bmp",
        /* i */ "art/dyna/missing.bmp",
        /* j */ "art/dyna/missing.bmp",
        /* k */ "art/dyna/missing.bmp",
        /* l */ "art/dyna/missing.bmp",
        /* m */ "art/dyna/missing.bmp",
        /* n */ "art/dyna/missing.bmp",
        /* o */ "art/dyna/missing.bmp",
        /* p */ "art/dyna/missing.bmp",
        /* q */ "art/dyna/missing.bmp",
        /* r */ "art/dyna/missing.bmp",
        /* s */ "art/dyna/missing.bmp",
        /* t */ "art/dyna/missing.bmp",
        /* u */ "art/dyna/missing.bmp",
        /* v */ "art/dyna/missing.bmp",
        /* w */ "art/dyna/missing.bmp",
        /* x */ "art/dyna/missing.bmp",
        /* y */ "art/dyna/missing.bmp",
        /* z */ "art/dyna/missing.bmp",
        /* { */ "art/stat/error.bmp",
        /* | */ "art/stat/error.bmp",
        /* } */ "art/stat/error.bmp",
        /* ~ */ "art/stat/trap_door.bmp",
    };
    const int count = xlen(names);
    SDL_Surface** const surface = xtoss(SDL_Surface*, count);
    for(int i = 0; i < count; i++)
        surface[i] = load(names[i]);
    const Surfaces surfaces = { surface, count };
    return surfaces;
}

void xclean(const Surfaces surfaces)
{
    for(int i = 0; i < surfaces.count; i++)
        SDL_FreeSurface(surfaces.surface[i]);
    free(surfaces.surface);
}
