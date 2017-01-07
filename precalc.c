#include "precalc.h"

#include <stdlib.h>
#include <math.h>

double precalc_focal = 1.0;

void precalc_optimize(const int xres, const int yres)
{
    precalc_distances = malloc(sizeof(double) * yres);
    for(int row = 0; row < yres; row++)
    {
        precalc_distances[row] = precalc_focal * yres / (2 * (row + 1) - yres);
    }
    precalc_sigmas = malloc(sizeof(double) * xres);
    for(int col = 0; col < xres; col++)
    {
        const double pan = 2.0 * (double)col / xres - 1.0;
        precalc_sigmas[col] = atan2(pan, precalc_focal); // 90 FOV
    }
}

void precalc_unload(void)
{
    free(precalc_distances);
    free(precalc_sigmas);
}
