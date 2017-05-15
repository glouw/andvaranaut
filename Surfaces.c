#include "Surfaces.h"

#include "Util.h"
#include "String.h"

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    const char* const extension = strchr(path, '.');
    // BMP: Wall, ceiling, and floorings
    if(match(extension, ".bmp"))
    {
        SDL_Surface* const bmp = SDL_LoadBMP(path);
        if(!bmp)
            bomb("Could not open %s\n", path);
        SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
        SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
        SDL_FreeFormat(allocation);
        SDL_FreeSurface(bmp);
        return converted;
    }
    // PNG: Sprites
    else
    {
        SDL_Surface* const img = IMG_Load(path);
        if(!img)
            bomb("Could not open %s\n", path);
        SDL_SetColorKey(img, SDL_TRUE, SDL_MapRGB(img->format, 0x00, 0xFF, 0xFF));
        return img;
    }
}

Surfaces pull(const uint32_t format)
{
    FILE* const file = fopen("surfaces/surfaces.cfg", "r");
    const int lines = lns(file);
    SDL_Surface** const surface = toss(SDL_Surface*, lines);
    for(int i = 0; i < lines; i++)
    {
        char* const line = readln(file);
        // Comments allowed
        char* const trim = strtok(line, "# \n");
        surface[i] = load(trim, format);
        free(line);
    }
    fclose(file);
    const Surfaces surfaces = { lines, surface };
    return surfaces;
}

void clean(const Surfaces surfaces)
{
    for(int i = 0; i < surfaces.count; i++)
        SDL_FreeSurface(surfaces.surface[i]);
    free(surfaces.surface);
}
