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
    where.x = 27.5;
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
    hero.tall = 0.5;
    hero.height = hero.tall;
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
    const float max = 1.99;
    const float min = 0.01;
    hero.yaw =
        hero.yaw > max ? max : // Max clamp.
        hero.yaw < min ? min : // Min clamp.
        hero.yaw;
    return hero;
}

static Hero vert(Hero hero, const Map map, const Input input)
{
    // Being in water takes precedence over crouching.
    if(!xtile(hero.where, map.floring)) { hero.height = 0.0; return hero; }
    // Crouching. Takes precedence over jumping.
    if(input.key[SDL_SCANCODE_LCTRL]) { hero.height = hero.tall / 1.50; return hero; }
    // Jump.
    if(hero.height <= hero.tall && input.key[SDL_SCANCODE_SPACE]) hero.vvel = 0.05;
    // Apply.
    hero.height += hero.vvel, hero.vvel -= 0.005;
    // Clamp.
    const float maxheight = 0.95;
    if(hero.height < hero.tall) hero.vvel = 0.0, hero.height = hero.tall; // Max clamp.
    if(hero.height > maxheight) hero.vvel = 0.0, hero.height = maxheight; // Min clamp.
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

static Hero move(Hero hero, const Map map, const Input input)
{
    const Point last = hero.where;
    const float speed =
        hero.height < hero.tall ? hero.speed / 2.5 :
        !xtile(hero.where, map.floring) ? hero.speed / 2.5 :
        hero.speed;
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
    else hero.velocity = xmul(hero.velocity, 1.0 - hero.acceleration / speed);
    // Top speed check.
    if(xmag(hero.velocity) > speed) hero.velocity = xmul(xunt(hero.velocity), speed);
    // Moves and checks for a collision.
    hero.where = xadd(hero.where, hero.velocity);
    // Reset hero if collision.
    if(xtile(hero.where, map.walling))
    {
        xzero(hero.velocity);
        hero.where = last;
    }
    return hero;
}

int xteleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
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

Ray xcalc(const Hero hero, const Hit hit, const int level, const int yres)
{
    const Point end = xsub(hit.where, hero.where);
    const Point corrected = xtrn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, hero.fov.a.x, hero.yaw, corrected, hero.height);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Ray ray = {
        traceline,
        level == 1 ? xstack(projection) : level == -1 ? xdrop(projection) : projection,
        hit, hero.torch
    };
    return ray;
}

Hero xsustain(Hero hero, const Map map, const Input input)
{
    hero = spin(hero, input);
    hero = vert(hero, map, input);
    hero = move(hero, map, input);
    hero = yaw(hero, input);
    hero.torch = xburn(hero.torch);
    return hero;
}
