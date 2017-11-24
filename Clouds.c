#include "Clouds.h"

#include "util.h"

Clouds xrise()
{
    Clouds c;
    xzero(c);
    c.acceleration = 0.00001;
    c.speed = 0.01;
    c.direction.x = 1.0;
    c.direction.y = 1.0;
    c.height = 10.0;
    return c;
}

Clouds xblow(Clouds c)
{
    const Point vect = xmul(c.direction, c.acceleration);
    c.velocity = xadd(c.velocity, vect);
    // And then check top speed...
    if(xmag(c.velocity) > c.speed)
        // And cap speed if the top speed is surpassed.
        c.velocity = xmul(xunt(c.velocity), c.speed);
    c.where = xadd(c.where, c.velocity);
    return c;
}
