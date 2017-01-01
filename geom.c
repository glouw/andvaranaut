#include "geom.h"

#include "map.h"

#include <math.h>

static inline struct point sn(const struct point hero, const double m, const double b) // Step north
{
    const double y = ceil(hero.y - 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static inline struct point se(const struct point hero, const double m, const double b) // Step east
{
    const double x = floor(hero.x + 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static inline struct point ss(const struct point hero, const double m, const double b) // Step south
{
    const double y = floor(hero.y + 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static inline struct point sw(const struct point hero, const double m, const double b) // Step west
{
    const double x = ceil(hero.x - 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static inline bool fn(const struct point point) // Facing north
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && wallings[y + 0][x + 0] && wallings[y - 1][x + 0] == 0;
}

static inline bool fe(const struct point point) // Facing east
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && wallings[y + 0][x + 0] == 0 && wallings[y + 0][x - 1];
}

static inline bool fs(const struct point point) // Facing south
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && wallings[y + 0][x + 0] == 0 && wallings[y - 1][x + 0];
}

static inline bool fw(const struct point point) // Facing west
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && wallings[y + 0][x + 0] && wallings[y + 0][x - 1] == 0;
}

static inline bool hor(const struct point point)
{
    return fn(point) || fs(point);
}

static inline bool ver(const struct point point)
{
    return fe(point) || fw(point);
}

static inline bool wall(const struct point point)
{
    return hor(point) || ver(point);
}

static inline struct point closest(const struct point hero, const struct point i, const struct point j)
{
    return mag(sub(i, hero)) < mag(sub(j, hero)) ? i : j;
}

static inline struct point step(const struct point hero, const double m, const double b, const int q)
{
    struct point point;
    switch(q)
    {
        case 0: point = closest(hero, se(hero, m, b), ss(hero, m, b)); break;
        case 1: point = closest(hero, sw(hero, m, b), ss(hero, m, b)); break;
        case 2: point = closest(hero, sw(hero, m, b), sn(hero, m, b)); break;
        case 3: point = closest(hero, se(hero, m, b), sn(hero, m, b)); break;
    }
    if(out(point))
        return (struct point){ -1.0, -1.0 };
    return wall(point) ? point : step(point, m, b, q);
}

static inline int quadrant(const double radians)
{
    const double x = cos(radians);
    const double y = sin(radians);
    if(x >= 0.0 && y >= 0.0) return 0;
    if(x <= 0.0 && y >= 0.0) return 1;
    if(x <= 0.0 && y <= 0.0) return 2;
    if(x >= 0.0 && y <= 0.0) return 3;
    return -1;
}

struct point sub(const struct point i, const struct point j)
{
    return (struct point){ i.x - j.x, i.y - j.y };
}

struct point add(const struct point i, const struct point j)
{
    return (struct point){ i.x + j.x, i.y + j.y };
}

struct point mul(const struct point i, const double n)
{
    return (struct point){ i.x * n, i.y * n };
}

double mag(const struct point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

double percentage(const struct point point)
{
    if(fn(point)) return 0.0 + (point.x - floor(point.x));
    if(fe(point)) return 1.0 - (point.y - floor(point.y));
    if(fs(point)) return 1.0 - (point.x - floor(point.x));
    if(fw(point)) return 0.0 + (point.y - floor(point.y));
    return 0.0;
}

bool out(const struct point point)
{
    return point.x >= cols || point.y >= rows || point.x < 0 || point.y < 0;
}

struct point cast(const struct point hero, const double radians)
{
    const double m = tan(radians);
    const double b = hero.y - m * hero.x;
    const double q = quadrant(radians);
    const struct point hit = step(hero, m, b, q);
    return hit;
}

int getwalling(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    if(fn(point)) return wallings[y + 0][x + 0];
    if(fe(point)) return wallings[y + 0][x - 1];
    if(fw(point)) return wallings[y + 0][x + 0];
    if(fs(point)) return wallings[y - 1][x + 0];
    return -1;
}

bool collision(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return wallings[y][x];
}

int getflooring(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return floorings[y][x];
}

int getceiling(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return ceilings[y][x];
}
