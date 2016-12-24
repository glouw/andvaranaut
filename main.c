#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct point
{
    double x, y;
};

struct vector
{
    struct point a, b;
};

static const int xres = 800;

static const int map[][13] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

static double mag(const struct vector vector)
{
    const double x = pow(vector.b.x - vector.a.x, 2.0);
    const double y = pow(vector.b.y - vector.a.y, 2.0);
    return sqrt(x + y);
}

static struct vector sub(const struct vector i, const struct vector j)
{
    struct vector s = i;
    s.a.x -= j.a.x; s.a.y -= j.a.y;
    s.b.x -= j.b.x; s.b.y -= j.b.y;
    return s;
}

// Step North
static struct vector sn(const struct vector player, const double m, const double b)
{
    const double y = ceil(player.b.y - 1);
    const double x = (y - b) / m;
    return (struct vector) { { 0.0, 0.0 }, { x, y } };
}

// Step East
static struct vector se(const struct vector player, const double m, const double b)
{
    const double x = floor(player.b.x + 1);
    const double y = m * x + b;
    return (struct vector) { { 0.0, 0.0 }, { x, y } };
}

// Step South
static struct vector ss(const struct vector player, const double m, const double b)
{
    const double y = floor(player.b.y + 1);
    const double x = (y - b) / m;
    return (struct vector) { { 0.0, 0.0 }, { x, y } };
}

// Step West
static struct vector sw(const struct vector player, const double m, const double b)
{
    const double x = ceil(player.b.x - 1);
    const double y = m * x + b;
    return (struct vector) { { 0.0, 0.0 }, { x, y } };
}

static int quadrant(const struct vector camera)
{
    if(camera.b.x >  0.0 && camera.b.y >= 0.0) return 0;
    if(camera.b.x <= 0.0 && camera.b.y >  0.0) return 1;
    if(camera.b.x <  0.0 && camera.b.y <= 0.0) return 2;
    if(camera.b.x >= 0.0 && camera.b.y <  0.0) return 3;
    return -1;
}

static struct vector step(const struct vector player, const struct vector camera)
{
    const double m = camera.b.y / camera.b.x;
    const double b = player.b.y - m * player.b.x;
    const struct vector n = sn(player, m, b); // Step North
    const struct vector e = se(player, m, b); // Step East
    const struct vector s = ss(player, m, b); // Step South
    const struct vector w = sw(player, m, b); // Step West
    // Step to the next line
    struct vector next;
    switch(quadrant(camera))
    {
        case 0: next = mag(sub(e, player)) < mag(sub(s, player)) ? e : s; break;
        case 1: next = mag(sub(w, player)) < mag(sub(s, player)) ? w : s; break;
        case 2: next = mag(sub(w, player)) < mag(sub(n, player)) ? w : n; break;
        case 3: next = mag(sub(e, player)) < mag(sub(n, player)) ? e : n; break;
        default: fprintf(stderr, "Engine's busted. Great...\n"); exit(1); break;
    }
    const int x = next.b.x;
    const int y = next.b.y;
    // Is it a horizontal line?
    if(next.b.y == floor(next.b.y))
    {
        if(map[y][x] || map[y - 1][x]) return next; // Wall?
    }
    else // its a vertical line!
    {
        if(map[y][x] || map[y][x - 1]) return next; // Wall?
    }
    // Seems like we didn't find a wall so let's move onto the next line
    return step(next, camera);
}

int main(void)
{
    const int half = xres / 2;
    for(int col = -half; col < half; col++)
    {
        const double pan = (double)col / half;
        const struct vector player = { { 0.0, 0.0 }, { 3.5, 3.5 } };
        const struct vector camera = { { 0.0, 0.0 }, { .66, pan } };
        const struct vector hit = step(player, camera);
        const struct vector view = sub(hit, player);
        // Fish eye correction
        printf("%f\n", view.b.x);
    }
    return 0;
}
