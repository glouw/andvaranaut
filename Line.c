#include "Line.h"

#include "Point.h"

extern Line rotate(const Line line, const float t)
{
    const Line rotated = { trn(line.a, t), trn(line.b, t) };
    return rotated;
}

extern Point lerp(const Line line, const float n)
{
    return add(line.a, mul(sub(line.b, line.a), n));
}

extern float focal(const Line line)
{
    return line.a.x / (line.b.y - line.a.y);
}

extern float ratio(const Line line)
{
    return line.a.x / line.b.y;
}

extern float ccast(const Line fov, const int res, const int x)
{
    return focal(fov) * res / (2 * x - (res - 1));
}

extern float fcast(const Line fov, const int res, const int x)
{
    return -ccast(fov, res, x);
}
