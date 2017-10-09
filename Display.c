#include "Display.h"

Display xlock(const Sdl sdl)
{
    void* screen;
    int pitch;
    SDL_LockTexture(sdl.texture, NULL, &screen, &pitch);
    const int width = pitch / sizeof(uint32_t);
    const Display display = { (uint32_t*) screen, width };
    return display;
}

void xunlock(const Sdl sdl)
{
    SDL_UnlockTexture(sdl.texture);
}
