#include "misc.h"

int fl(const double x)
{
    return (int) x - (x < (int) x);
}

int cl(const double x)
{
    return (int) x + (x > (int) x);
}

double dec(const double x)
{
    return x - (int) x;
}
