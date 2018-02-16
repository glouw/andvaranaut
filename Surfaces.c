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
        /*   */ "art/static/delete.bmp",
        /* ! */ "art/static/error.bmp",
        /* " */ "art/static/floor.bmp",
        /* # */ "art/static/wall.bmp",
        /* $ */ "art/static/ceiling.bmp",
        /* % */ "art/static/water.bmp",
        /* & */ "art/static/cloud.bmp",
        /* ' */ "art/static/board.bmp",
        /* ( */ "art/static/grass.bmp",
        /* ) */ "art/static/dirt.bmp",
        /* * */ "art/static/cloud2.bmp",
        /* + */ "art/static/error.bmp",
        /* , */ "art/static/error.bmp",
        /* - */ "art/static/error.bmp",
        /* . */ "art/static/error.bmp",
        /* / */ "art/static/error.bmp",
        /* 0 */ "art/static/error.bmp",
        /* 1 */ "art/static/error.bmp",
        /* 2 */ "art/static/error.bmp",
        /* 3 */ "art/static/error.bmp",
        /* 4 */ "art/static/error.bmp",
        /* 5 */ "art/static/error.bmp",
        /* 6 */ "art/static/error.bmp",
        /* 7 */ "art/static/error.bmp",
        /* 8 */ "art/static/error.bmp",
        /* 9 */ "art/static/error.bmp",
        /* : */ "art/static/error.bmp",
        /* ; */ "art/static/error.bmp",
        /* < */ "art/static/error.bmp",
        /* = */ "art/static/error.bmp",
        /* > */ "art/static/error.bmp",
        /* ? */ "art/static/error.bmp",
        /* @ */ "art/static/error.bmp",
        /* A */ "art/dynamic/missing.bmp",
        /* B */ "art/dynamic/missing.bmp",
        /* C */ "art/dynamic/missing.bmp",
        /* D */ "art/dynamic/missing.bmp",
        /* E */ "art/dynamic/missing.bmp",
        /* F */ "art/dynamic/missing.bmp",
        /* G */ "art/dynamic/missing.bmp",
        /* H */ "art/dynamic/missing.bmp",
        /* I */ "art/dynamic/missing.bmp",
        /* J */ "art/dynamic/missing.bmp",
        /* K */ "art/dynamic/missing.bmp",
        /* L */ "art/dynamic/missing.bmp",
        /* M */ "art/dynamic/missing.bmp",
        /* N */ "art/dynamic/missing.bmp",
        /* O */ "art/dynamic/missing.bmp",
        /* P */ "art/dynamic/missing.bmp",
        /* Q */ "art/dynamic/missing.bmp",
        /* R */ "art/dynamic/missing.bmp",
        /* S */ "art/dynamic/missing.bmp",
        /* T */ "art/dynamic/missing.bmp",
        /* U */ "art/dynamic/missing.bmp",
        /* V */ "art/dynamic/missing.bmp",
        /* W */ "art/dynamic/missing.bmp",
        /* X */ "art/dynamic/missing.bmp",
        /* Y */ "art/dynamic/missing.bmp",
        /* Z */ "art/dynamic/missing.bmp",
        /* [ */ "art/inventory/Gui0.bmp",
        /* \ */ "art/static/error.bmp",
        /* ] */ "art/inventory/Gui1.bmp",
        /* ^ */ "art/static/error.bmp",
        /* _ */ "art/static/error.bmp",
        /* ` */ "art/static/error.bmp",
        /* a */ "art/dynamic/flower.bmp",
        /* b */ "art/dynamic/outlaw.bmp",
        /* c */ "art/dynamic/missing.bmp",
        /* d */ "art/dynamic/lootbag.bmp",
        /* e */ "art/dynamic/tree.bmp",
        /* f */ "art/dynamic/treeleaves.bmp",
        /* g */ "art/dynamic/missing.bmp",
        /* h */ "art/dynamic/missing.bmp",
        /* i */ "art/dynamic/missing.bmp",
        /* j */ "art/dynamic/missing.bmp",
        /* k */ "art/dynamic/missing.bmp",
        /* l */ "art/dynamic/missing.bmp",
        /* m */ "art/dynamic/missing.bmp",
        /* n */ "art/dynamic/missing.bmp",
        /* o */ "art/dynamic/missing.bmp",
        /* p */ "art/dynamic/missing.bmp",
        /* q */ "art/dynamic/missing.bmp",
        /* r */ "art/dynamic/missing.bmp",
        /* s */ "art/dynamic/missing.bmp",
        /* t */ "art/dynamic/missing.bmp",
        /* u */ "art/dynamic/missing.bmp",
        /* v */ "art/dynamic/missing.bmp",
        /* w */ "art/dynamic/missing.bmp",
        /* x */ "art/dynamic/missing.bmp",
        /* y */ "art/dynamic/missing.bmp",
        /* z */ "art/dynamic/missing.bmp",
        /* { */ "art/static/error.bmp",
        /* | */ "art/static/error.bmp",
        /* } */ "art/static/error.bmp",
        /* ~ */ "art/static/trap_door.bmp",
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
