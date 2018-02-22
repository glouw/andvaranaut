#include "Flow.h"

#include "util.h"

Flow xstart(const float height)
{
    Flow f;
    xzero(f);
    f.acceleration = 0.00001f;
    f.speed = 0.01f;
    f.direction.x = 1.0f;
    f.direction.y = 1.0f;
    f.height = height;
    return f;
}

Flow xstream(Flow f)
{
    f.velocity = xadd(f.velocity, xmul(f.direction, f.acceleration));
    if(xmag(f.velocity) > f.speed)
        f.velocity = xmul(xunt(f.velocity), f.speed);
    f.where = xadd(f.where, f.velocity);
    return f;
}
