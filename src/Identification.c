#include "Identification.h"

int i_same_id(const Identification a, const Identification b)
{
    return a.index == b.index && a.clas == b.clas;
}
