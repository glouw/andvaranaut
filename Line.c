#include "Line.h"

Line xrotate(const Line line, const float t)
{
    const Line rotated = { xtrn(line.a, t), xtrn(line.b, t) };
    return rotated;
}

Point xlerp(const Line line, const float n)
{
    return xadd(line.a, xmul(xsub(line.b, line.a), n));
}

float xfocal(const Line line)
{
    return line.a.x / line.b.y;
}

float xfcast(const Line fov, const int yres, const int x, const float xx)
{
    return -xfocal(fov) * yres / (2 * x - (yres - 1)) / xx;
}
