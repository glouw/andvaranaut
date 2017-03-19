#include "Gpu.h"

Gpu setup(const int res)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    return (Gpu) { pull("config/surfaces.cfg", format), window, renderer, texture };
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
