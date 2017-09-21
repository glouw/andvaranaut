#include "Hero.h"

#include "util.h"
#include "SDL2/SDL.h"

static Line lens(const float scale)
{
    Line fov;
    fov.a.x = 1.0;
    fov.a.y = -scale;
    fov.b.x = 1.0;
    fov.b.y = scale;
    return fov;
}

static Point beginning()
{
    Point where;
    where.x = 5.5;
    where.y = 5.5;
    return where;
}

Hero spawn()
{
    Hero hero;
    zero(hero);
    hero.fov = lens(1.0);
    hero.where = beginning();
    hero.speed = 0.12;
    hero.acceleration = 0.0150;
    hero.torch = out();
    hero.arm = 0.75;
    return hero;
}

static Hero spin(const Hero hero, const Input input)
{
    Hero temp = hero;
    temp.theta += input.dx * input.sx;
    return temp;
}

Point touch(const Hero hero, const float reach)
{
    const Point reference = { reach, 0.0 };
    const Point direction = trn(reference, hero.theta);
    return add(hero.where, direction);
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0, 0.0 };
    const Point direction = trn(unt(reference), hero.theta);
    return mul(direction, hero.acceleration);
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
        if(input.key[SDL_SCANCODE_W]) temp.velocity = add(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_S]) temp.velocity = sub(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_D]) temp.velocity = add(temp.velocity, rag(acceleration));
        if(input.key[SDL_SCANCODE_A]) temp.velocity = sub(temp.velocity, rag(acceleration));
    }
    // Mass-spring damping system
    else temp.velocity = mul(temp.velocity, 1.0 - temp.acceleration / temp.speed);
    // Top speed check
    if(mag(temp.velocity) > temp.speed)
        temp.velocity = mul(unt(temp.velocity), temp.speed);
    // Moves and checks for a collision
    temp.where = add(temp.where, temp.velocity);
    // Reset hero if collision
    if(tile(temp.where, walling))
    {
        zero(temp.velocity);
        temp.where = hero.where;
    }
    return temp;
}

static Hero zoom(const Hero hero, const Input input)
{
    Hero temp = hero;
    if( input.m) temp.fov = lens(0.25);
    if(!input.m) temp.fov = lens(1.00);
    return temp;
}

Ray cast(const Hero hero, char** const block, const Point column, const int res)
{
    const Hit hit = march(hero.where, column, block);
    const Point end = sub(hit.where, hero.where);
    const Point corrected = trn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Ray ray = { traceline, projection, hit };
    return ray;
}

bool teleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
    static int last;
    // Arbitrary wait time - whatever has the best feel
    if(ticks < last + 3)
        return false;
    if(!input.key[SDL_SCANCODE_E])
        return false;
    last = ticks;
    return isportal(map, hero.where);
}

Hero teleport(const Hero hero, const Map map)
{
    Hero temp = hero;
    temp.torch = out();
    if(block(hero.where, map.floring) == '~') temp.floor++;
    if(block(hero.where, map.ceiling) == '~') temp.floor--;
    return temp;
}

Hero sustain(const Hero hero, const Map map, const Input input)
{
    Hero temp = hero;
    temp = spin(temp, input);
    temp = move(temp, map.walling, input);
    temp = zoom(temp, input);
    temp.torch = burn(temp.torch);
    return temp;
}
