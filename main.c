#include <stdio.h>
#include <math.h>
#include <stdbool.h>

const int xres = 100;

const double distance = 0.66;

const int map[][5] = {
    { 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1 },
};

struct sprite
{
    double x;
    double y;
    double rad;
};

struct hit
{
    double offset;
    double distance;
};

int quadrant(const double rad)
{
    if(rad < 0.0)
        return -1;
    if(rad <= 1.0 * M_PI / 2.0) return 0;
    if(rad <= 1.0 * M_PI / 1.0) return 1;
    if(rad <= 3.0 * M_PI / 2.0) return 2;
    if(rad <= 2.0 * M_PI / 1.0) return 3;
    // Anything else
    return -1;
}

struct hit dda(const struct sprite sprite, const double rad)
{
    const double angle = sprite.rad + rad;
    double x = 0.0;
    double y = 0.0;
    for(;;)
    {
        double dx = 0.0;
        double dy = 0.0;
        switch(quadrant(angle))
        {
            case 0: // rise(+) run(+)
            case 3: // rise(-) run(+)
                dx = floor(sprite.x + 1.0) - sprite.x;
                dy = dx * tan(angle);
                break;
            case 1: // rise(+) run(-)
            case 2: // rise(-) run(-)
                dx = ceil(sprite.x - 1.0) - sprite.x;
                dy = dx * tan(angle);
                break;
        }
        x += dx;
        y += dy;
        const int px = sprite.x + x;
        const int py = sprite.y + y;
        if(map[py][px])
        {
            printf("%f: %d %d\n", angle, px, py);
            break;
        }
    }
    return (struct hit){ 0.0, 0.0 };
}

int main(void)
{
    struct sprite hero;
    hero.x = 1.5;
    hero.y = 7.5;
    hero.rad = 1.0 * M_PI / 2.0;
    const int half = xres / 2;
    for(double i = half; i > -half; i--)
    {
        const double line = i / half;
        const double rad = atan2(line, distance);
        const struct hit wall = dda(hero, rad);
    }
    return 0;
}
