#include "Line.h"

Line xzline(void)
{
    static Line line;
    return line;
}

Line xrotate(const Line line, const float t)
{
    const Line rotated = { p_turn(line.a, t), p_turn(line.b, t) };
    return rotated;
}

Point xlerp(const Line line, const float n)
{
    return p_add(line.a, p_mul(p_sub(line.b, line.a), n));
}
