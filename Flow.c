#include "Flow.h"

#include "util.h"

Flow xstart()
{
    Flow f;
    xzero(f);
    f.acceleration = 0.00001;
    f.speed = 0.01;
    f.direction.x = 1.0;
    f.direction.y = 1.0;
    f.height = -1.0 / 6.0;
    return f;
}

Flow xstream(Flow f)
{
    f.velocity = xadd(f.velocity, xmul(f.direction, f.acceleration));
    // And then check top speed...
    if(xmag(f.velocity) > f.speed)
        // And cap speed if the top speed is surpassed.
        f.velocity = xmul(xunt(f.velocity), f.speed);
    f.where = xadd(f.where, f.velocity);
    return f;
}
