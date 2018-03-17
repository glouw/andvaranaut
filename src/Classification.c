#include "Classification.h"

#include "util.h"

int xcindex(const Classification c)
{
    return '~' + 1 - ' ' + c;
}

int xcindices(const Classification c, const Surfaces ss)
{
    const int index = xcindex(c);
    return c == NONE ? 1 : ss.surface[index]->h / ss.surface[index]->w;
}

int xismelee(const Classification c)
{
    (void) c;
    return false;
}

int xisranged(const Classification c)
{
    (void) c;
    return false;
}

int xismagic(const Classification c)
{
    (void) c;
    return false;
}
