#include "Surfaces.h"
#include "Util.h"

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_SetColorKey(converted, SDL_TRUE, 0xFFFFFF);
    SDL_SetSurfaceRLE(converted, SDL_TRUE);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    return converted;
}

Surfaces pull(const char* const path, const uint32_t format)
{
    FILE* const file = fopen(path, "r");
    const int lines = lns(file);
    SDL_Surface** const surface = (SDL_Surface**) calloc(lines, sizeof(*surface));
    for(int i = 0; i < lines; i++)
    {
        char* const line = readln(file);
        char* const trim = strtok(line, "# \n"); // Comments allowed
        surface[i] = load(trim, format);
    }
    fclose(file);
    const Surfaces surfaces = { surface, lines };
    return surfaces;
}
