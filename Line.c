#include "Line.h"

Line xrotate(const Line line, const float t)
{
    const Line rotated = { xtrn(line.a, t), xtrn(line.b, t) };
    return rotated;
}

// Used in floor renderer for almost half the screens pixels.
// Needs to be optimized like crazy
Point xlerp(const Line line, const float n)
{
    return xadd(line.a, xmul(xsub(line.b, line.a), n));
}
