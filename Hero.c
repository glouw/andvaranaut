#include "Hero.h"

#include "util.h"
#include "SDL2/SDL.h"

static Line lens(const float focal)
{
    Line fov;
    fov.a.x = focal;
    fov.a.y = -1.0;
    fov.b.x = focal;
    fov.b.y = 1.0;
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
    hero.torch = xout();
    hero.arm = 0.75;
    hero.scent = 1e3;
    return hero;
}

static Hero spin(const Hero hero, const Input input)
{
    Hero temp = hero;
    temp.theta += input.dx * input.sx;
    return temp;
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

static Hero move(const Hero hero, char** const walling, const Input input)
{
    Hero temp = hero;
    // Acceleration
    if(input.key[SDL_SCANCODE_W]
    || input.key[SDL_SCANCODE_S]
    || input.key[SDL_SCANCODE_D]
    || input.key[SDL_SCANCODE_A])
    {
        const Point acceleration = accelerate(temp);
        if(input.key[SDL_SCANCODE_W]) temp.velocity = xadd(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_S]) temp.velocity = xsub(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_D]) temp.velocity = xadd(temp.velocity, xrag(acceleration));
        if(input.key[SDL_SCANCODE_A]) temp.velocity = xsub(temp.velocity, xrag(acceleration));
    }
    // Mass spring damping system
    else temp.velocity = xmul(temp.velocity, 1.0 - temp.acceleration / temp.speed);
    // Top speed check
    if(xmag(temp.velocity) > temp.speed)
        temp.velocity = xmul(xunt(temp.velocity), temp.speed);
    // Moves and checks for a collision
    temp.where = xadd(temp.where, temp.velocity);
    // Reset hero if collision
    if(xtile(temp.where, walling))
    {
        xzero(temp.velocity);
        temp.where = hero.where;
    }
    return temp;
}

Ray xcast(const Hero hero, char** const block, const Point column, const int yres)
{
    const Hit hit = xmarch(hero.where, column, block);
    const Point end = xsub(hit.where, hero.where);
    const Point corrected = xtrn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Ray ray = { traceline, projection, hit };
    return ray;
}

bool xteleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
    static int last;
    // Arbitrary wait time - whatever has the best feel
    if(ticks < last + 3)
        return false;
    if(!input.key[SDL_SCANCODE_E])
        return false;
    last = ticks;
    return xisportal(map, hero.where);
}

Hero xteleport(const Hero hero, const Map map)
{
    Hero temp = hero;
    temp.torch = xout();
    if(xblok(hero.where, map.floring) == '~') temp.floor++;
    if(xblok(hero.where, map.ceiling) == '~') temp.floor--;
    return temp;
}

Hero xsustain(const Hero hero, const Map map, const Input input)
{
    Hero temp = hero;
    temp = spin(temp, input);
    temp = move(temp, map.walling, input);
    temp.torch = xburn(temp.torch);
    return temp;
}
