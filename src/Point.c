#include "Point.h"

#include "util.h"

#include <stdio.h>
#include <math.h>
#include <float.h>

Point p_turn(const Point a, const float t)
{
    const Point out = {
        a.x * cosf(t) - a.y * sinf(t),
        a.x * sinf(t) + a.y * cosf(t),
    };
    return out;
}

Point p_rot90(const Point a)
{
    const Point out = {
        -a.y,
        +a.x,
    };
    return out;
}

Point p_sub(const Point a, const Point b)
{
    const Point out = {
        a.x - b.x,
        a.y - b.y,
    };
    return out;
}

Point p_add(const Point a, const Point b)
{
    const Point out = {
        a.x + b.x,
        a.y + b.y,
    };
    return out;
}

Point p_mul(const Point a, const float n)
{
    const Point out = {
        a.x * n,
        a.y * n,
    };
    return out;
}

float p_mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point p_div(const Point a, const float n)
{
    const Point out = {
        a.x / n,
        a.y / n,
    };
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

Point p_jump_hor(const Point a, const Point b)
{
    Point out;
    out.x = b.x > 0.0f ? u_fl(a.x + 1.0f) : u_cl(a.x - 1.0f);
    out.y = p_slope(b) * (out.x - a.x) + a.y;
    return out;
}

Point p_jump_ver(const Point a, const Point b)
{
    Point out;
    out.y = b.y > 0.0f ? u_fl(a.y + 1.0f) : u_cl(a.y - 1.0f);
    out.x = (out.y - a.y) / p_slope(b) + a.x;
    return out;
}

Point p_mid(const Point a)
{
    const Point out = {
        u_fl(a.x) + 0.5f,
        u_fl(a.y) + 0.5f,
    };
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
    const Point b = {
        fabsf(a.x),
        fabsf(a.y),
    };
    return b;
}

Point p_snap(const Point a, const int grid)
{
    const Point out = {
        (float) u_fl(a.x / grid) * grid,
        (float) u_fl(a.y / grid) * grid,
    };
    return out;
}

void p_print(const Point a)
{
    printf("%f %f\n", (double) a.x, (double) a.y);
}
