#include "Classification.h"

int xcindices(const Classification c, const Surfaces ss)
{
    const int pixels = 16;
    return c == NONE ? 1 :
        ss.surface['~' + 1 - ' ' + c]->h / pixels;
}
