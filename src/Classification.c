#include "Classification.h"

int xcindex(const Classification c)
{
    return '~' + 1 - ' ' + c;
}

int xcindices(const Classification c, const Surfaces ss)
{
    const int index = xcindex(c);
    return c == NONE ? 1 :
        ss.surface[index]->h / ss.surface[index]->w;
}
