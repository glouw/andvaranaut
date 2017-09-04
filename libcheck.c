#include "libcheck.h"

#include "SDL2/SDL_version.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#include <assert.h>

static void sdl2()
{
    SDL_version c;
    SDL_version l;
    SDL_VERSION(&c);
    SDL_GetVersion(&l);
    assert(c.major == l.major);
    assert(c.minor == l.minor);
    assert(c.patch == l.patch);
}

static void sdl2_ttf()
{
    SDL_version c;
    SDL_TTF_VERSION(&c)
    const SDL_version* l = TTF_Linked_Version();
    assert(c.major == l->major);
    assert(c.minor == l->minor);
    assert(c.patch == l->patch);
}

static void sdl2_image()
{
    SDL_version c;
    SDL_IMAGE_VERSION(&c);
    const SDL_version* l = IMG_Linked_Version();
    assert(c.major == l->major);
    assert(c.minor == l->minor);
    assert(c.patch == l->patch);
}

void libcheck()
{
    sdl2();
    sdl2_ttf();
    sdl2_image();
}
