#include "Surfaces.h"
#include "Util.h"
#include <SDL2/SDL_image.h>
#include <string.h>

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    const char* const extension = strchr(path, '.');
    if(strcmp(extension, ".bmp") == 0)
    {
        SDL_Surface* const bmp = SDL_LoadBMP(path);
        SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
        SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
        SDL_FreeFormat(allocation);
        SDL_FreeSurface(bmp);
        return converted;
    }
    else // Sprites
    {
        SDL_Surface* const img = IMG_Load(path);
        SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0x00, 0xFF, 0xFF));
        return img;
    }
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
