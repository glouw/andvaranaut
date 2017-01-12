#include "Geom.h"

#include "Map.h"

#include <math.h>

static inline struct point
sn(const struct point hero, const double m, const double b)
{
    const double y = ceil(hero.y - 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static inline struct point
se(const struct point hero, const double m, const double b)
{
    const double x = floor(hero.x + 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static inline struct point
ss(const struct point hero, const double m, const double b)
{
    const double y = floor(hero.y + 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static inline struct point
sw(const struct point hero, const double m, const double b)
{
    const double x = ceil(hero.x - 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static inline bool
fn(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && enclosure[y + 0][x + 0] && enclosure[y - 1][x + 0] == 0;
}

static inline bool
fe(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && enclosure[y + 0][x + 0] == 0 && enclosure[y + 0][x - 1];
}

static inline bool
fs(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && enclosure[y + 0][x + 0] == 0 && enclosure[y - 1][x + 0];
}

static inline bool
fw(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && enclosure[y + 0][x + 0] && enclosure[y + 0][x - 1] == 0;
}

static inline bool
hor(const struct point point, uint8_t** enclosure)
{
    return fn(point, enclosure) || fs(point, enclosure);
}

static inline bool
ver(const struct point point, uint8_t** enclosure)
{
    return fe(point, enclosure) || fw(point, enclosure);
}

static inline bool
wall(const struct point point, uint8_t** enclosure)
{
    return hor(point, enclosure) || ver(point, enclosure);
}

static inline struct point
closest(const struct point hero, const struct point i, const struct point j)
{
    return Geom_mag(Geom_sub(i, hero)) < Geom_mag(Geom_sub(j, hero)) ? i : j;
}

static inline struct point
step(const struct point hero, const double m, const double b, const int q, uint8_t** enclosure)
{
    struct point point;
    switch(q)
    {
        case 0: point = closest(hero, se(hero, m, b), ss(hero, m, b)); break;
        case 1: point = closest(hero, sw(hero, m, b), ss(hero, m, b)); break;
        case 2: point = closest(hero, sw(hero, m, b), sn(hero, m, b)); break;
        case 3: point = closest(hero, se(hero, m, b), sn(hero, m, b)); break;
    }
    if(Geom_out(point)) return point;
    return wall(point, enclosure) ? point : step(point, m, b, q, enclosure);
}

static inline int
quadrant(const double radians)
{
    const double x = cos(radians);
    const double y = sin(radians);
    if(x >= 0.0 && y >= 0.0) return 0;
    if(x <= 0.0 && y >= 0.0) return 1;
    if(x <= 0.0 && y <= 0.0) return 2;
    if(x >= 0.0 && y <= 0.0) return 3;
    return -1;
}

double
Geom_mag(const struct point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

double
Geom_epercent(const struct point point, uint8_t** enclosure)
{
    if(fn(point, enclosure)) return 0.0 + Geom_mod(point.x);
    if(fe(point, enclosure)) return 1.0 - Geom_mod(point.y);
    if(fs(point, enclosure)) return 1.0 - Geom_mod(point.x);
    if(fw(point, enclosure)) return 0.0 + Geom_mod(point.y);
    return 0.0;
}

struct point
Geom_cast(const struct point hero, const double radians, uint8_t** enclosure)
{
    const double m = tan(radians);
    const double b = hero.y - m * hero.x;
    const double q = quadrant(radians);
    return step(hero, m, b, q, enclosure);
}

bool
Geom_ecoll(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    return enclosure[y][x];
}

int
Geom_etile(const struct point point, uint8_t** enclosure)
{
    const int x = point.x;
    const int y = point.y;
    if(fn(point, enclosure)) return enclosure[y + 0][x + 0];
    if(fe(point, enclosure)) return enclosure[y + 0][x - 1];
    if(fw(point, enclosure)) return enclosure[y + 0][x + 0];
    if(fs(point, enclosure)) return enclosure[y - 1][x + 0];
    return -1;
}

int
Geom_ctile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return Map_ceiling[y][x];
}

int
Geom_ftile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return Map_floring[y][x];
}

bool
Geom_in(const struct point point)
{
    const bool x = point.x < (double)Map_xsz && point.x > 0.0;
    const bool y = point.y < (double)Map_ysz && point.y > 0.0;
    return x && y;
}

bool
Geom_out(const struct point point)
{
    return !Geom_in(point);
}
