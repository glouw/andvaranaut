#include "Line.h"

Line rotate(const Line line, const float t)
{
    const Line rotated = { trn(line.a, t), trn(line.b, t) };
    return rotated;
}

Point lerp(const Line line, const float n)
{
    return add(line.a, mul(sub(line.b, line.a), n));
}

float focal(const Line line)
{
    return line.a.x / line.b.y;
}

float fcast(const Line fov, const int res, const int x, const float xx)
{
    return -focal(fov) * res / (2 * x - (res - 1)) / xx;
}
