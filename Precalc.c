#include "Precalc.h"

#include "Display.h"

#include <stdlib.h>
#include <math.h>

static void
OptimizeRows()
{
    Precalc_distances = malloc(sizeof(double) * Display_yres);
    for(int row = 0; row < Display_yres; row++)
    {
        Precalc_distances[row] = Display_focal * Display_yres / (2 * (row + 1) - Display_yres);
    }
}

static void
OptimizeCols()
{
    Precalc_sigmas = malloc(sizeof(double) * Display_xres);
    for(int col = 0; col < Display_xres; col++)
    {
        const double pan = 2.0 * (double)col / Display_xres - 1.0;
        Precalc_sigmas[col] = atan2(pan, Display_focal);
    }
}

void
Precalc_Optimize()
{
    OptimizeRows();
    OptimizeCols();
}

void
Precalc_Shutdown()
{
    free(Precalc_distances);
    free(Precalc_sigmas);
}
