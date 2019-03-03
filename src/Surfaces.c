#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);

    if(bmp == NULL)
        u_bomb("%s\n", SDL_GetError());

    //
    // Color keys can not be set in SDL when an alpha channel is present.
    //

    SDL_PixelFormat* const allocation = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);

    const uint32_t key = 0x00FFFF;
    const uint32_t r = 0xFF & (key >> 0x10);
    const uint32_t g = 0xFF & (key >> 0x08);
    const uint32_t b = 0xFF & (key >> 0x00);
    SDL_SetColorKey(converted, SDL_TRUE, SDL_MapRGB(converted->format, r, g, b));

    return converted;
}

static SDL_Surface* dye(SDL_Surface* const surface, const uint32_t from, const uint32_t to)
{
    uint32_t* const pixels = (uint32_t*) surface->pixels;
    const int size = surface->w * surface->h;

    for(int i = 0; i < size; i++)
        if(pixels[i] == from)
            pixels[i] = to;
    //
    // Convenience return.
    //

    return surface;
}

Surfaces s_load_surfaces(const Palette palette)
{
#pragma message "Maintainer: Apply new sprite pixel art in Surfaces.c::s_load_surfaces"

    //
    // Sprites: Alpha ASCII
    // Tiles: Non-Alpha ASCII
    // Items and GUI: '~' and up.
    //

    static const char* names[] = {
        /*        */ "art/tiles/delete.bmp",
        /* !      */ "art/tiles/error.bmp",
        /* "      */ "art/tiles/floor.bmp",
        /* #      */ "art/tiles/wall.bmp",
        /* $      */ "art/tiles/ceiling.bmp",
        /* %      */ "art/tiles/water.bmp",
        /* &      */ "art/tiles/error.bmp",
        /* '      */ "art/tiles/board.bmp",
        /* (      */ "art/tiles/grass.bmp",
        /* )      */ "art/tiles/dirt.bmp",
        /* *      */ "art/tiles/error.bmp",
        /* +      */ "art/tiles/error.bmp",
        /* ,      */ "art/tiles/error.bmp",
        /* -      */ "art/tiles/error.bmp",
        /* .      */ "art/tiles/error.bmp",
        /* /      */ "art/tiles/error.bmp",
        /* 0      */ "art/tiles/error.bmp",
        /* 1      */ "art/tiles/error.bmp",
        /* 2      */ "art/tiles/error.bmp",
        /* 3      */ "art/tiles/error.bmp",
        /* 4      */ "art/tiles/error.bmp",
        /* 5      */ "art/tiles/error.bmp",
        /* 6      */ "art/tiles/error.bmp",
        /* 7      */ "art/tiles/error.bmp",
        /* 8      */ "art/tiles/error.bmp",
        /* 9      */ "art/tiles/error.bmp",
        /* :      */ "art/tiles/error.bmp",
        /* ;      */ "art/tiles/error.bmp",
        /* <      */ "art/tiles/error.bmp",
        /* =      */ "art/tiles/error.bmp",
        /* >      */ "art/tiles/error.bmp",
        /* ?      */ "art/tiles/error.bmp",
        /* @      */ "art/tiles/error.bmp",
        /* A      */ "art/sprites/missing.bmp",
        /* B      */ "art/sprites/dwarf.bmp", // MAKE ME MY OWN TUTOR SPRITE
        /* C      */ "art/sprites/missing.bmp",
        /* D      */ "art/sprites/missing.bmp",
        /* E      */ "art/sprites/missing.bmp",
        /* F      */ "art/sprites/missing.bmp",
        /* G      */ "art/sprites/missing.bmp",
        /* H      */ "art/sprites/missing.bmp",
        /* I      */ "art/sprites/missing.bmp",
        /* J      */ "art/sprites/missing.bmp",
        /* K      */ "art/sprites/missing.bmp",
        /* L      */ "art/sprites/missing.bmp",
        /* M      */ "art/sprites/missing.bmp",
        /* N      */ "art/sprites/missing.bmp",
        /* O      */ "art/sprites/missing.bmp",
        /* P      */ "art/sprites/missing.bmp",
        /* Q      */ "art/sprites/missing.bmp",
        /* R      */ "art/sprites/missing.bmp",
        /* S      */ "art/sprites/missing.bmp",
        /* T      */ "art/sprites/missing.bmp",
        /* U      */ "art/sprites/missing.bmp",
        /* V      */ "art/sprites/missing.bmp",
        /* W      */ "art/sprites/missing.bmp",
        /* X      */ "art/sprites/missing.bmp",
        /* Y      */ "art/sprites/missing.bmp",
        /* Z      */ "art/sprites/missing.bmp",
        /* [      */ "art/tiles/error.bmp",
        /* \      */ "art/tiles/error.bmp",
        /* ]      */ "art/tiles/error.bmp",
        /* ^      */ "art/tiles/error.bmp",
        /* _      */ "art/tiles/error.bmp",
        /* `      */ "art/tiles/error.bmp",
        /* a      */ "art/sprites/flower.bmp",
        /* b      */ "art/sprites/dwarf.bmp",
        /* c      */ "art/sprites/ember.bmp",
        /* d      */ "art/sprites/lootbag.bmp",
        /* e      */ "art/sprites/missing.bmp",
        /* f      */ "art/sprites/missing.bmp",
        /* g      */ "art/sprites/missing.bmp",
        /* h      */ "art/sprites/missing.bmp",
        /* i      */ "art/sprites/missing.bmp",
        /* j      */ "art/sprites/missing.bmp",
        /* k      */ "art/sprites/missing.bmp",
        /* l      */ "art/sprites/missing.bmp",
        /* m      */ "art/sprites/missing.bmp",
        /* n      */ "art/sprites/missing.bmp",
        /* o      */ "art/sprites/missing.bmp",
        /* p      */ "art/sprites/missing.bmp",
        /* q      */ "art/sprites/missing.bmp",
        /* r      */ "art/sprites/missing.bmp",
        /* s      */ "art/sprites/missing.bmp",
        /* t      */ "art/sprites/missing.bmp",
        /* u      */ "art/sprites/missing.bmp",
        /* v      */ "art/sprites/missing.bmp",
        /* w      */ "art/sprites/missing.bmp",
        /* x      */ "art/sprites/missing.bmp",
        /* y      */ "art/sprites/missing.bmp",
        /* z      */ "art/sprites/missing.bmp",
        /* {      */ "art/tiles/error.bmp",
        /* |      */ "art/tiles/error.bmp",
        /* }      */ "art/tiles/error.bmp",
        /* ~      */ "art/tiles/trap_door.bmp",
        /* ~ +  1 */ "art/items/ammo.bmp",
        /* ~ +  2 */ "art/items/amulet.bmp",
        /* ~ +  3 */ "art/items/armor.bmp",
        /* ~ +  4 */ "art/items/book.bmp",
        /* ~ +  5 */ "art/items/boot.bmp",
        /* ~ +  6 */ "art/items/chest.bmp",
        /* ~ +  7 */ "art/items/flesh.bmp",
        /* ~ +  8 */ "art/items/food.bmp",
        /* ~ +  9 */ "art/items/glove.bmp",
        /* ~ + 10 */ "art/items/hat.bmp",
        /* ~ + 11 */ "art/items/key.bmp",
        /* ~ + 12 */ "art/items/letter.bmp",
        /* ~ + 13 */ "art/items/light.bmp",
        /* ~ + 14 */ "art/items/longwep.bmp",
        /* ~ + 15 */ "art/items/medwep.bmp",
        /* ~ + 16 */ "art/items/money.bmp",
        /* ~ + 17 */ "art/items/music.bmp",
        /* ~ + 18 */ "art/items/potion.bmp",
        /* ~ + 19 */ "art/items/ring.bmp",
        /* ~ + 20 */ "art/items/rock.bmp",
        /* ~ + 21 */ "art/items/scroll.bmp",
        /* ~ + 22 */ "art/items/shield.bmp",
        /* ~ + 23 */ "art/items/shortwep.bmp",
        /* ~ + 24 */ "art/items/tool.bmp",
        /* ~ + 25 */ "art/items/wand.bmp",
        /* ~ + 26 */ "art/gui/gui.bmp",
    };

    const int count = u_len(names);
    Surface* surface = u_toss(Surface, count);

    for(int i = 0; i < count; i++)
    {
        const char* const name = names[i];
        surface[i].dye[DYE_WHT] = load(name);
        surface[i].dye[DYE_RED] = dye(load(name), palette.wht, palette.red);
        surface[i].dye[DYE_BRN] = dye(load(name), palette.wht, palette.brn);
    }

    const Surfaces surfaces = { surface, count };
    return surfaces;
}
