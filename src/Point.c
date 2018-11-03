#include "Point.h"

#include "util.h"

#include <stdio.h>
#include <math.h>
#include <float.h>

Point p_zero(void)
{
    static Point point;
    return point;
}

Point p_turn(const Point a, const float t)
{
    Point out;
    out.x = a.x * cosf(t) - a.y * sinf(t);
    out.y = a.x * sinf(t) + a.y * cosf(t);
    return out;
}

Point p_rot90(const Point a)
{
    Point out;
    out.x = -a.y;
    out.y = +a.x;
    return out;
}

Point p_sub(const Point a, const Point b)
{
    Point out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

Point p_add(const Point a, const Point b)
{
    Point out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

Point p_mul(const Point a, const float n)
{
    Point out;
    out.x = a.x * n;
    out.y = a.y * n;
    return out;
}

float p_mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point p_div(const Point a, const float n)
{
    Point out;
    out.x = a.x / n;
    out.y = a.y / n;
    return out;
}

Point p_unit(const Point a)
{
    return p_div(a, p_mag(a));
}

float p_slope(const Point a)
{
    return a.y / a.x;
}

Point p_jmphor(const Point a, const Point b)
{
    Point out;
    out.x = b.x > 0.0f ? xfl(a.x + 1.0f) : xcl(a.x - 1.0f);
    out.y = p_slope(b) * (out.x - a.x) + a.y;
    return out;
}

Point p_jmpver(const Point a, const Point b)
{
    Point out;
    out.y = b.y > 0.0f ? xfl(a.y + 1.0f) : xcl(a.y - 1.0f);
    out.x = (out.y - a.y) / p_slope(b) + a.x;
    return out;
}

Point p_mid(const Point a)
{
    Point out;
    out.x = xfl(a.x) + 0.5f;
    out.y = xfl(a.y) + 0.5f;
    return out;
}

int p_inf(const Point a)
{
    return isinf((long double) a.x)
        || isinf((long double) a.y);
}

Point p_cmp(const Point a, const Point b, const Point c)
{
    return p_inf(c) ? b : p_inf(b) ? c : p_mag(p_sub(b, a)) < p_mag(p_sub(c, a)) ? b : c;
}

int p_eql(const Point a, const Point b, const float e)
{
    return a.x < b.x + (e / 2.0f)
        && a.x > b.x - (e / 2.0f)
        && a.y < b.y + (e / 2.0f)
        && a.y > b.y - (e / 2.0f);
}

int p_same(const Point a, const Point b)
{
    return a.x == b.x && a.y == b.y;
}

int p_block(const Point a, char** const blocks)
{
    const int y = a.y;
    const int x = a.x;
    return blocks[y][x];
}

int p_tile(const Point a, char** const blocks)
{
    return p_block(a, blocks) - ' ';
}

Point p_abs(const Point a)
{
    Point b;
    b.x = fabsf(a.x);
    b.y = fabsf(a.y);
    return b;
}

Point p_snap(const Point a, const int grid)
{
    Point out;
    out.x = xfl(a.x / grid) * grid;
    out.y = xfl(a.y / grid) * grid;
    return out;
}

Point p_rand(Point a, const int grid)
{
    a.x += rand() % grid - grid / 2.0f;
    a.y += rand() % grid - grid / 2.0f;
    return a;
}

void p_print(const Point a)
{
    printf("%f %f\n", (double) a.x, (double) a.y);
}
