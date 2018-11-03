#include "Flow.h"

#include "util.h"

Flow f_start(const float height)
{
    static Flow zero;
    Flow f = zero;
    f.acceleration = 0.00001f;
    f.speed = 0.01f;
    f.direction.x = 1.0f;
    f.direction.y = 1.0f;
    f.velocity = p_mul(f.direction, f.speed);
    f.height = height;
    return f;
}

Flow f_stream(Flow f)
{
    f.velocity = p_add(f.velocity, p_mul(f.direction, f.acceleration));
    if(p_mag(f.velocity) > f.speed)
        f.velocity = p_mul(p_unit(f.velocity), f.speed);
    f.where = p_add(f.where, f.velocity);
    return f;
}
