#pragma once

double* precalc_distances;
double* precalc_sigmas;

double precalc_focal;

void
precalc_optimize(const int xres, const int yres);

void
precalc_unload(void);
