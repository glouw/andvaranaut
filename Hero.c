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
    hero.aura = 12;
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

static float hduck(const Hero hero)
{
    return 0.7 * hero.tall;
}

static float hswim(const Hero hero, const Current current)
{
    return (0.5 + current.height) * hero.tall;
}

static Hero vert(Hero hero, const Map map, const Input input, const Current current)
{
    const int land = xtile(hero.where, map.floring);
    const float tall = land ? hero.tall : hswim(hero, current);
    // Jump.
    if(input.key[SDL_SCANCODE_SPACE] && hero.height <= tall) hero.vvel = 0.05;
    // Apply.
    hero.height += hero.vvel;
    // Fall.
    if(hero.height > tall) hero.vvel -= 0.005; else hero.vvel = 0.0, hero.height = tall;
    // Clamp.
    const float max = 0.95;
    const float min = 0.05;
    if(hero.height > max) hero.height = max;
    if(hero.height < min) hero.height = min;
    // Crouch - only works on land.
    if(input.key[SDL_SCANCODE_LCTRL] && land) hero.height = hduck(hero);
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

static Hero move(Hero hero, const Map map, const Input input, const Current current)
{
    const Point last = hero.where;
    const int swimming = hero.height <= hswim(hero, current);
    const int crouching = hero.height <= hduck(hero);
    const float speed = swimming ? 0.3 * hero.speed : crouching ? 0.5 * hero.speed : hero.speed;
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
    // Current sweep.
    if(swimming) hero.velocity = xadd(hero.velocity, current.velocity);
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
    hero.yaw = 1.0;
    if(xblok(hero.where, map.floring) == '~') hero.floor++;
    if(xblok(hero.where, map.ceiling) == '~') hero.floor--;
    return hero;
}

Ray xcalc(const Hero hero, const Hit hit, const float shift, const int yres)
{
    const Point end = xsub(hit.where, hero.where);
    const Point corrected = xtrn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, hero.fov.a.x, hero.yaw, corrected, hero.height);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Ray ray = {
        traceline,
        shift > 0.0 ? xstack(projection, shift) : shift < 0.0 ? xdrop(projection, shift) : projection,
        hit, hero.torch
    };
    return ray;
}

Hero xsustain(Hero hero, const Map map, const Input input, const Current current)
{
    hero = spin(hero, input);
    hero = vert(hero, map, input, current);
    hero = move(hero, map, input, current);
    hero = yaw(hero, input);
    hero.torch = xburn(hero.torch);
    return hero;
}
