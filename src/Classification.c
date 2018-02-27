#include "Classification.h"

int xcindices(const Classification c, const Surfaces ss)
{
    return ss.surface['~' + 1 - ' ' + c]->h / 16;
}
