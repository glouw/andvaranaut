#include "Point.h"

#include "util.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

Point pt(const float x, const float y)
{
    Point out;
    out.x = x;
    out.y = y;
    return out;
}

Point trn(const Point a, const float t)
{
    Point out;
    out.x = a.x * cosf(t) - a.y * sinf(t);
    out.y = a.x * sinf(t) + a.y * cosf(t);
    return out;
}

Point rag(const Point a)
{
    Point out;
    out.x = -a.y;
    out.y = +a.x;
    return out;
}

Point sub(const Point a, const Point b)
{
    Point out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    return out;
}

Point add(const Point a, const Point b)
{
    Point out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    return out;
}

Point mul(const Point a, const float n)
{
    Point out;
    out.x = a.x * n;
    out.y = a.y * n;
    return out;
}

float mag(const Point a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

Point dvd(const Point a, const float n)
{
    Point out;
    out.x = a.x / n;
    out.y = a.y / n;
    return out;
}

Point unt(const Point a)
{
    return dvd(a, mag(a));
}

float slp(const Point a)
{
    return a.y / a.x;
}

Point shr(const Point a, const Point b)
{
    Point out;
    out.x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    out.y = slp(b) * (out.x - a.x) + a.y;
    return out;
}

Point svr(const Point a, const Point b)
{
    Point out;
    out.y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    out.x = (out.y - a.y) / slp(b) + a.x;
    return out;
}

Point mid(const Point a)
{
    Point out;
    out.x = fl(a.x) + 0.5;
    out.y = fl(a.y) + 0.5;
    return out;
}

bool inf(const Point a)
{
    return isinf(a.x) || isinf(a.y);
}

Point cmp(const Point a, const Point b, const Point c)
{
    return inf(c) ? b : inf(b) ? c : mag(sub(b, a)) < mag(sub(c, a)) ? b : c;
}

bool eql(const Point a, const Point b, const float e)
{
    return a.x < b.x + (e / 2.0)
        && a.x > b.x - (e / 2.0)
        && a.y < b.y + (e / 2.0)
        && a.y > b.y - (e / 2.0);
}

int block(const Point a, char** const blocks)
{
    const int y = a.y;
    const int x = a.x;
    return blocks[y][x];
}

int tile(const Point a, char** const blocks)
{
    return block(a, blocks) - ' ';
}

Point force(float** const field, const Point to, const Point from)
{
    // Target reached
    if(eql(to, from, 1.0))
    {
        Point z;
        zero(z);
        return z;
    }
    // Try all immediate directions
    const Point points[] = {
        {  1, -0 }, // E
        {  1,  1 }, // SE
        {  0,  1 }, // S
        { -1,  1 }, // SW
        { -1,  0 }, // W
        { -1, -1 }, // NW
        {  0, -1 }, // N
        {  1, -1 }, // NE
    };
    float max = FLT_MIN;
    int index = 0;
    // Find the direction with the largest gradient
    for(int i = 0; i < len(points); i++)
    {
        const Point dir = add(points[i], from);
        const int x = from.x;
        const int y = from.y;
        const int yy = dir.y;
        const int xx = dir.x;
        const float gradient = field[yy][xx] - field[y][x];
        if(gradient > max)
        {
            max = gradient;
            index = i;
        }
    }
    return points[index];
}
