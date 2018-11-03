#include "Atom.h"

static float average(const Field field, const int y, const int x)
{
    float sum = 0.0f;
    int sums = 0;
    for(int j = y - 1; j <= y + 1; j++)
    for(int i = x - 1; i <= x + 1; i++)
    {
        if(!f_on(field, j, i))
            continue;
        if(j == y && i == x)
            continue;
        sum += field.mesh[j][i];
        sums++;
    }
    return sum / sums;
}

Atom a_materialize(const Field field, const int y, const int x)
{
    const Atom atom = { y, x, average(field, y, x) };
    return atom;
}
