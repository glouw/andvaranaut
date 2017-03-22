#include "Surfaces.h"

#include "util.h"

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    return converted;
}

Surfaces pull(const char* const path, const uint32_t format)
{
    FILE* const fp = fopen(path, "r");
    char* line = NULL;
    unsigned reads = 0;
    const int count = newlines(fp);
    SDL_Surface** const surface = calloc(count, sizeof(*surface));
    for(int i = 0; i < count; i++)
    {
        getline(&line, &reads, fp);
        line = strtok(line, "# \n");
        surface[i] = load(line, format);
    }
    free(line);
    fclose(fp);
    return (Surfaces) { surface, count };
}
