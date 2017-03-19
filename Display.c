#include "Display.h"

Display lock(const Gpu gpu)
{
    void* screen;
    int pitch;
    SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
    return (Display) { screen, pitch / sizeof(uint32_t) };
}

void unlock(const Gpu gpu)
{
    SDL_UnlockTexture(gpu.texture);
}
