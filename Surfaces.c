#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    if(!bmp)
        xbomb("Could not open %s\n", path);
    SDL_SetColorKey(bmp, SDL_TRUE, SDL_MapRGB(bmp->format, 0x00, 0xFF, 0xFF));
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    return converted;
}

Surfaces xpull(const uint32_t format)
{
    FILE* const file = fopen("surfaces/surfaces.cfg", "r");
    const int lines = xlns(file);
    SDL_Surface** const surface = xtoss(SDL_Surface*, lines);
    for(int i = 0; i < lines; i++)
    {
        char* const line = xreadln(file);
        // Comments allowed
        char* const trim = strtok(line, "# \n");
        surface[i] = load(trim, format);
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
