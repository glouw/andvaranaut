#include "Attack.h"

#include "util.h"

Attack xzattack()
{
    static Attack attack;
    return attack;
}

Attack xgpower(const Gauge g, const Input input, const Classification c)
{
    const int last = g.count - 1;
    // The tail must be at least this long.
    const int tail = 6;
    if(g.count < tail)
        return xzattack();
    // On the mouse left up, calculate the magnitude and direction of attack.
    if(input.lu)
    {
        float mag = 0.0f;
        // Magnitude.
        for(int i = 0; i < g.count - 1; i++)
            mag += xmag(xsub(g.points[i + 1], g.points[i + 0]));
        // Direction.
        const Point dir = xsub(g.points[last], g.points[last - tail]);
        const Attack attack =  {
            mag,
            xunt(dir),
            // Certain weapons offer more "hurt" factors than others.
            // Hurt factors declare how many enemies may be hurt by a single swipe.
            // A greatword may be able to cleave three enemies at once,
            // while a short sword only one.
            3
        };
        // If direction magitude is zero, the unit vector will be NA, so none must be returned.
        return xmag(dir) > 0.0f ? attack : xzattack();
    }
    return xzattack();
}
