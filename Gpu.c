#include "Gpu.h"

#include "misc.h"

SDL_Surface* load(const char* const path, const uint32_t format)
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
    SDL_Surface** surface = calloc(count, sizeof(*surface));
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

Gpu setup(const int res)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    return (Gpu) { pull("surfaces.cfg", format), window, renderer, texture };
}

void release(const Gpu gpu)
{
    SDL_DestroyTexture(gpu.texture);
    SDL_Quit();
    SDL_DestroyWindow(gpu.window);
    SDL_DestroyRenderer(gpu.renderer);
    for(int i = 0; i < gpu.surfaces.count; i++) SDL_FreeSurface(gpu.surfaces.surface[i]);
    free(gpu.surfaces.surface);
}

void present(const Gpu gpu)
{
    SDL_RenderCopyEx(gpu.renderer, gpu.texture, NULL, NULL, -90.0, NULL, 0);
    SDL_RenderPresent(gpu.renderer);
}
