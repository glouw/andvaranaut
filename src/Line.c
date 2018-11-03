#include "Line.h"

Line l_rotate(const Line line, const float t)
{
    const Line rotated = { p_turn(line.a, t), p_turn(line.b, t) };
    return rotated;
}

Point l_lerp(const Line line, const float n)
{
    return p_add(line.a, p_mul(p_sub(line.b, line.a), n));
}
