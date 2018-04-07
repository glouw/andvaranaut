#include "Scroll.h"

#include "util.h"

#include <float.h>

Scroll xsczero()
{
    static Scroll sc;
    return sc;
}

Scroll xscnew(const Surfaces ss)
{
    const int scroll = '~' - ' ' + 20;
    SDL_Surface* const surface = ss.surface[scroll];
    Scroll sc = xsczero();
    sc.grid = 8;
    sc.squares = sc.grid * sc.grid;
    sc.casting = xtoss(int, sc.squares);
    sc.count = surface->h / surface->w;
    sc.castables = xtoss(int*, sc.count);
    for(int i = 0; i < sc.count; i++)
        sc.castables[i] = xtoss(int, sc.squares);
    return sc;
}

static float err(const float a, const float b)
{
    return 0.5f * powf(a - b, 2.0f);
}

int xcsminerr(const Scroll sc)
{
    int index = 0;
    float max = FLT_MAX;
    for(int j = 0; j < sc.count; j++)
    {
        float sum = 0;
        for(int i = 0; i < sc.squares; i++)
            sum += err(sc.casting[i], sc.castables[j][i]);
        if(sum < max)
        {
            max = sum;
            index = j;
        }
    }
    return index;
}
