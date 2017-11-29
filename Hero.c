#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

static Line lens(const float focal)
{
    Line fov;
    fov.a.x = focal;
    fov.a.y = -1.0;
    fov.b.x = focal;
    fov.b.y = +1.0;
    return fov;
}

static Point beginning()
{
    Point where;
    where.x = 1.5;
    where.y = 3.5;
    return where;
}

Hero xspawn(const float focal)
{
    Hero hero;
    xzero(hero);
    hero.fov = lens(focal);
    hero.where = beginning();
    hero.speed = 0.12;
    hero.acceleration = 0.0150;
    hero.torch = xsnuff();
    hero.arm = 0.75;
    hero.scent = 6;
    hero.yaw = 1.0;
    hero.floor = 1;
    return hero;
}

static Hero spin(Hero hero, const Input input)
{
    hero.theta += input.dx * input.sx;
    return hero;
}

static Hero yaw(Hero hero, const Input input)
{
    hero.yaw += input.dy * input.sy;
    if(hero.yaw > 1.99) hero.yaw = 1.99;
    if(hero.yaw < 0.01) hero.yaw = 0.01;
    return hero;
}

Point xtouch(const Hero hero, const float reach)
{
    const Point reference = { reach, 0.0 };
    const Point direction = xtrn(reference, hero.theta);
    return xadd(hero.where, direction);
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0, 0.0 };
    const Point direction = xtrn(xunt(reference), hero.theta);
    return xmul(direction, hero.acceleration);
}

static Hero move(Hero hero, char** const walling, const Input input)
{
    const Point last = hero.where;
    // Acceleration.
    if(input.key[SDL_SCANCODE_W]
    || input.key[SDL_SCANCODE_S]
    || input.key[SDL_SCANCODE_D]
    || input.key[SDL_SCANCODE_A])
    {
        const Point acceleration = accelerate(hero);
        if(input.key[SDL_SCANCODE_W]) hero.velocity = xadd(hero.velocity, acceleration);
        if(input.key[SDL_SCANCODE_S]) hero.velocity = xsub(hero.velocity, acceleration);
        if(input.key[SDL_SCANCODE_D]) hero.velocity = xadd(hero.velocity, xrag(acceleration));
        if(input.key[SDL_SCANCODE_A]) hero.velocity = xsub(hero.velocity, xrag(acceleration));
    }
    // Mass spring damping system.
    else hero.velocity = xmul(hero.velocity, 1.0 - hero.acceleration / hero.speed);
    // Top speed check.
    if(xmag(hero.velocity) > hero.speed) hero.velocity = xmul(xunt(hero.velocity), hero.speed);
    // Moves and checks for a collision.
    hero.where = xadd(hero.where, hero.velocity);
    // Reset hero if collision.
    const Point vectors[] = {
        {  1, -0 }, // E
        {  1,  1 }, // SE
        {  0,  1 }, // S
        { -1,  1 }, // SW
        { -1,  0 }, // W
        { -1, -1 }, // NW
        {  0, -1 }, // N
        {  1, -1 }, // NE
    };
    for(int i = 0; i < xlen(vectors); i++)
    {
        const Point out = xadd(hero.where, xmul(vectors[i], 0.01));
        if(xtile(out, walling))
        {
            xzero(hero.velocity);
            hero.where = last;
        }
    }
    return hero;
}

int xteleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
    // Time delay is arbitrary to feel;
    static int last;
    const int delay = 3;
    if(ticks < last + delay)
        return false;
    if(!input.key[SDL_SCANCODE_E])
        return false;
    last = ticks;
    return xisportal(map, hero.where);
}

Hero xteleport(Hero hero, const Map map)
{
    hero.torch = xsnuff();
    if(xblok(hero.where, map.floring) == '~') hero.floor++;
    if(xblok(hero.where, map.ceiling) == '~') hero.floor--;
    return hero;
}

Ray xcalc(const Hero hero, const Hit hit, const int upper, const int yres)
{
    const Point end = xsub(hit.where, hero.where);
    const Point corrected = xtrn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, hero.fov.a.x, hero.yaw, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Ray ray = { traceline, upper ? xstack(projection) : projection, hit, hero.torch };
    return ray;
}

Hero xsustain(Hero hero, const Map map, const Input input)
{
    hero = spin(hero, input);
    hero = move(hero, map.walling, input);
    hero = yaw(hero, input);
    hero.torch = xburn(hero.torch);
    return hero;
}
