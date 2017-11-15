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
    hero.torch = xout();
    hero.arm = 0.75;
    hero.scent = 6;
    return hero;
}

static Hero spin(Hero hero, const Input input)
{
    hero.theta += input.dx * input.sx;
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
    if(xtile(hero.where, walling))
    {
        xzero(hero.velocity);
        hero.where = last;
    }
    return hero;
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
    if(ticks < last + 3)
        return false;
    if(!input.key[SDL_SCANCODE_E])
        return false;
    last = ticks;
    return xisportal(map, hero.where);
}

Hero xteleport(Hero hero, const Map map)
{
    hero.torch = xout();
    if(xblok(hero.where, map.floring) == '~') hero.floor++;
    if(xblok(hero.where, map.ceiling) == '~') hero.floor--;
    return hero;
}

Hero xsustain(Hero hero, const Map map, const Input input)
{
    hero = spin(hero, input);
    hero = move(hero, map.walling, input);
    hero.torch = xburn(hero.torch);
    return hero;
}
