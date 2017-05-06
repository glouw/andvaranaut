#include "Hero.h"

#include "Portals.h"
#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Line.h"
#include "Light.h"

Hero spawn()
{
    const Line fov = { { 1.0, -1.0 }, { 1.0, 1.0 } };
    const Point where = { 5.5, 9.5 };
    const Point velocity = { 0.0, 0.0 };
    const float speed = 0.12;
    const float acceleration = 0.0150;
    const float theta = 0.0;
    const Light light = reset();
    const Hero hero = { fov, where, velocity, speed, acceleration, theta, light };
    return hero;
}

static Hero spin(const Hero hero, const uint8_t* key)
{
    Hero temp = hero;
    const float dtheta = 0.1;
    if(key[SDL_SCANCODE_H]) temp.theta -= dtheta;
    if(key[SDL_SCANCODE_L]) temp.theta += dtheta;
    return temp;
}

static Hero move(const Hero hero, char** const walling, const uint8_t* key)
{
    Hero step = hero;
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, step.theta);
        const Point acceleration = mul(direction, step.acceleration);
        if(key[SDL_SCANCODE_W]) step.velocity = add(step.velocity, acceleration);
        if(key[SDL_SCANCODE_S]) step.velocity = sub(step.velocity, acceleration);
        if(key[SDL_SCANCODE_D]) step.velocity = add(step.velocity, rag(acceleration));
        if(key[SDL_SCANCODE_A]) step.velocity = sub(step.velocity, rag(acceleration));
    }
    else step.velocity = mul(step.velocity, 1.0 - step.acceleration / step.speed);
    if(mag(step.velocity) > step.speed) step.velocity = mul(unt(step.velocity), step.speed);
    step.where = add(step.where, step.velocity);
    if(tile(step.where, walling)) step.velocity = zro(), step.where = hero.where;
    return step;
}

static Hit shoot(const Hero hero, char** const walling, const uint8_t* key)
{
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, hero.theta);
        return cast(hero.where, direction, walling);
    }
    const Hit hit = { 0, 0, 0.0, zro(), W };
    return hit;
}

int handle(const Hero hero, char** const walling, const uint8_t* key)
{
    const Hit hit = shoot(hero, walling, key);
    const int ch = hit.tile + ' ';
    const float reach = 1.0;
    const int nearby = mag(sub(hero.where, hit.where)) < reach;
    return nearby && isportal(ch) ? ch : 0;
}

static Hit plow(const Hero hero, char** const block, const Point column, const int hits)
{
    Hit hit;
    Point where = hero.where;
    for(int count = 0; count < hits; count++)
    {
        hit = cast(where, column, block);
        where = hit.where;
    }
    return hit;
}

Impact march(const Hero hero, char** const block, const Point column, const int res, const int hits)
{
    const Hit hit = plow(hero, block, column, hits);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = trn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

Hero teleport(const Hero hero, const Portal portal)
{
    Hero temp = hero;
    temp.light = reset();
    temp.where = portal.where;
    return temp;
}

static Hero fade(const Hero hero)
{
    Hero temp = hero;
    temp.light.torch += hero.light.dtorch;
    return temp.light.torch > hero.light.brightness ? hero : temp;
}

static Hero zoom(const Hero hero, const uint8_t* key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_U]) temp.fov.a.y += 0.01, temp.fov.b.y -= 0.01;
    if(key[SDL_SCANCODE_I]) temp.fov.a.y -= 0.01, temp.fov.b.y += 0.01;
    const float max = 4.0; // Arbitrary
    return ratio(temp.fov) > max ? hero : temp;
}

Hero touch(const Hero hero, const Map map, const uint8_t* key)
{
    Hero temp = hero;
    temp = spin(temp, key);
    temp = move(temp, map.walling, key);
    temp = zoom(temp, key);
    temp = fade(temp);
    return temp;
}
