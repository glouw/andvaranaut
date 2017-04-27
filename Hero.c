#include "Hero.h"
#include "Portals.h"
#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Line.h"

typedef struct
{
    float torch;
    float brightness;
}
Light;

static Light reset()
{
    const Light light = { 0.0, 200.0 };
    return light;
}

Hero spawn()
{
    const Line fov = { { 1.0, -1.0 }, { 1.0, 1.0 } };
    const Point where = { 5.5, 9.5 };
    const Point velocity = { 0.0, 0.0 };
    const float speed = 0.12;
    const float acceleration = 0.0150;
    const float theta = 0.0;
    const float percent = 0.0;
    const Light light = reset();
    const Hero hero = {
        fov, where, velocity, speed, acceleration, { theta, percent },
        light.torch, light.brightness
    };
    return hero;
}

Hero spin(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero temp = hero;
    const float dtheta = 0.1;
    if(key[SDL_SCANCODE_H]) temp.angle.theta -= dtheta;
    if(key[SDL_SCANCODE_L]) temp.angle.theta += dtheta;
    // Angle theta percentage calculated using a sawtooth
    const float pi = acosf(-1.0);
    temp.angle.percent = 0.5 - atanf(1.0 / tanf(temp.angle.theta / 2.0)) / pi;
    return temp;
}

Hero move(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, step.angle.theta);
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

static Hit shoot(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, hero.angle.theta);
        return cast(hero.where, direction, walling);
    }
    const Hit hit = { 0, 0, 0.0, zro() };
    return hit;
}

int handle(const Hero hero, char** const walling)
{
    const Hit hit = shoot(hero, walling);
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
    const Point corrected = trn(ray, -hero.angle.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

Hero teleport(const Hero hero, const Portal portal)
{
    Hero temp = hero;
    const Light light = reset();
    temp.torch = light.torch;
    temp.brightness = light.brightness;
    temp.where = portal.where;
    return temp;
}

Hero burn(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero temp = hero;
    const float dtorch = 5.0;
    if(key[SDL_SCANCODE_K]) temp.brightness = temp.torch += dtorch;
    if(key[SDL_SCANCODE_J]) temp.brightness = temp.torch -= dtorch;
    temp.torch = temp.torch < 0.0 ? 0.0 : temp.torch;
    return temp;
}

Hero brighten(const Hero hero)
{
    Hero temp = hero;
    const float dtorch = 5.0;
    temp.torch += dtorch;
    return temp.torch > hero.brightness ? hero : temp;
}

Hero zoom(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero temp = hero;
    if(key[SDL_SCANCODE_U]) temp.fov.a.y += 0.01, temp.fov.b.y -= 0.01;
    if(key[SDL_SCANCODE_I]) temp.fov.a.y -= 0.01, temp.fov.b.y += 0.01;
    const float max = 4.0; // Arbitrary
    return ratio(temp.fov) > max ? hero : temp;
}
