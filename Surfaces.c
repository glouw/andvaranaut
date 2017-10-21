#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    if(!bmp)
        xbomb("Could not open %s\n", path);
    // Color keys can not be set in SDL when an alpha channel is present.
    // The image must first be converted to RGB888 as the source may not be a true color BMP.
    // Notice how RGB888 is used for the conversion contrary to the ARGB8888 of the rendering canvas.
    SDL_PixelFormat* const allocation = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    SDL_SetColorKey(converted, SDL_TRUE, SDL_MapRGB(converted->format, 0x00, 0xFF, 0xFF));
    return converted;
}

Surfaces xpull()
{
    FILE* const file = fopen("surfaces/surfaces.cfg", "r");
    const int lines = xlns(file);
    SDL_Surface** const surface = xtoss(SDL_Surface*, lines);
    for(int i = 0; i < lines; i++)
    {
        char* const line = xreadln(file);
        // Comments allowed
        char* const trim = strtok(line, "# \n");
        surface[i] = load(trim);
        free(line);
    }
    fclose(file);
    const Surfaces surfaces = { surface, lines };
    return surfaces;
}

void xclean(const Surfaces surfaces)
{
    for(int i = 0; i < surfaces.count; i++)
        SDL_FreeSurface(surfaces.surface[i]);
    free(surfaces.surface);
}
