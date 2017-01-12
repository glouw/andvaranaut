#pragma once

#include <stdbool.h>
#include <stdint.h>

struct point
{
    double x, y;
};

double
Geom_mag(const struct point point);

double
Geom_epercent(const struct point point, uint8_t** enclosure);

struct point
Geom_cast(const struct point hero, const double radians, uint8_t** enclosure);

bool
Geom_ecoll(const struct point point, uint8_t** enclosure);

int
Geom_etile(const struct point point, uint8_t** enclosure);

int
Geom_rtile(const struct point point);

int
Geom_ctile(const struct point point);

int
Geom_ftile(const struct point point);

bool
Geom_in(const struct point point);

bool
Geom_out(const struct point point);

static inline struct point
Geom_sub(const struct point i, const struct point j)
{
    return (struct point){ i.x - j.x, i.y - j.y };
}

static inline struct point
Geom_add(const struct point i, const struct point j)
{
    return (struct point){ i.x + j.x, i.y + j.y };
}

static inline struct point
Geom_mul(const struct point i, const double n)
{
    return (struct point){ i.x * n, i.y * n };
}

static inline double
Geom_mod(const double d)
{
    return d - (int)d;
}
