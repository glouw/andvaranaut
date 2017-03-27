#include "Display.h"

Display lock(const Gpu gpu)
{
    void* screen;
    int pitch;
    SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
    const int width = pitch / sizeof(uint32_t);
    const Display display = { (uint32_t*) screen, width };
    return display;
}

void unlock(const Gpu gpu)
{
    SDL_UnlockTexture(gpu.texture);
}
