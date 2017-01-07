#include "geom.h"

#include "map.h"

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
fn(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && map_wallings[y + 0][x + 0] && map_wallings[y - 1][x + 0] == 0;
}

static inline bool
fe(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && map_wallings[y + 0][x + 0] == 0 && map_wallings[y + 0][x - 1];
}

static inline bool
fs(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && map_wallings[y + 0][x + 0] == 0 && map_wallings[y - 1][x + 0];
}

static inline bool
fw(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && map_wallings[y + 0][x + 0] && map_wallings[y + 0][x - 1] == 0;
}

static inline bool
hor(const struct point point)
{
    return fn(point) || fs(point);
}

static inline bool
ver(const struct point point)
{
    return fe(point) || fw(point);
}

static inline bool
wall(const struct point point)
{
    return hor(point) || ver(point);
}

static inline struct point
closest(const struct point hero, const struct point i, const struct point j)
{
    return geom_mag(geom_sub(i, hero)) < geom_mag(geom_sub(j, hero)) ? i : j;
}

static inline struct point
step(const struct point hero, const double m, const double b, const int q)
{
    struct point point;
    switch(q)
    {
        case 0: point = closest(hero, se(hero, m, b), ss(hero, m, b)); break;
        case 1: point = closest(hero, sw(hero, m, b), ss(hero, m, b)); break;
        case 2: point = closest(hero, sw(hero, m, b), sn(hero, m, b)); break;
        case 3: point = closest(hero, se(hero, m, b), sn(hero, m, b)); break;
    }
    if(geom_outofbounds(point))
        return point;
    return wall(point) ? point : step(point, m, b, q);
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
geom_mag(const struct point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

double
geom_wallpercentage(const struct point point)
{
    if(fn(point)) return 0.0 + geom_mod(point.x);
    if(fe(point)) return 1.0 - geom_mod(point.y);
    if(fs(point)) return 1.0 - geom_mod(point.x);
    if(fw(point)) return 0.0 + geom_mod(point.y);
    return 0.0;
}

struct point
geom_cast(const struct point hero, const double radians)
{
    const double m = tan(radians);
    const double b = hero.y - m * hero.x;
    const double q = quadrant(radians);
    const struct point hit = step(hero, m, b, q);
    return hit;
}

bool
geom_wallcollision(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return map_wallings[y][x];
}

int
geom_getbankingtile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return map_bankings[y][x];
}

int
geom_getwallingtile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    if(fn(point)) return map_wallings[y + 0][x + 0];
    if(fe(point)) return map_wallings[y + 0][x - 1];
    if(fw(point)) return map_wallings[y + 0][x + 0];
    if(fs(point)) return map_wallings[y - 1][x + 0];
    return -1;
}

int
geom_getflooringtile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return map_floorings[y][x];
}

int
geom_getceilingtile(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return map_ceilings[y][x];
}

bool
geom_outofbounds(const struct point point)
{
    return point.x >= map_xmax || point.y >= map_ymax || point.x < 0 || point.y < 0;
}
