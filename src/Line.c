#include "Line.h"

Line xzline()
{
    static Line line;
    return line;
}

Line xrotate(const Line line, const float t)
{
    const Line rotated = { xtrn(line.a, t), xtrn(line.b, t) };
    return rotated;
}

Point xlerp(const Line line, const float n)
{
    return xadd(line.a, xmul(xsub(line.b, line.a), n));
}
