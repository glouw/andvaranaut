#include "Hero.h"

#include "Portals.h"
#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Line.h"
#include "Light.h"

static Line normal()
{
    const Line fov = {
        { 1.0, -1.0 },
        { 1.0, +1.0 }
    };
    return fov;
}

static Line zoomed()
{
    const Line nrm = normal();
    const Line fov = {
        { nrm.a.x, nrm.a.y / 4.0 },
        { nrm.b.x, nrm.b.y / 4.0 }
    };
    return fov;
}

static Light reset()
{
    const float torch = 0.0;
    const float brightness = 350.0;
    const float dtorch = 20.0;
    const Light light = { torch, brightness, dtorch };
    return light;
}

static Hero assign(const Hero hero, char* const line)
{
    Hero temp = hero;
    const char* const field = trim(strtok(line, "="));
    const char* const value = trim(strtok(NULL, "\t \n"));
    if(match(field, "speed"))
        temp.speed = floating(value);
    else
    if(match(field, "acceleration"))
        temp.acceleration = floating(value);
    else
    if(match(field, "theta"))
        temp.theta = floating(value);
    else
    if(match(field, "where"))
    {
        Point where;
        sscanf(value, "%f,%f", &where.x, &where.y);
        temp.where = where;
    }
    return temp;
}

static Hero overturn(const Hero hero, const char* const name)
{
    char* const path = concat("config/", name);
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    Hero temp = hero;
    for(int i = 0; i < count; i++)
    {
        char* const line = readln(file);
        temp = assign(temp, line);
        free(line);
    }
    fclose(file);
    free(path);
    return temp;
}

Hero spawn(const char* const name)
{
    const Line fov = normal();
    const Point where = { 5.5, 9.5 };
    const Point velocity = { 0.0, 0.0 };
    const float speed = 0.12;
    const float acceleration = 0.0150;
    const float theta = 0.0;
    const Light light = reset();
    const Hero hero = { fov, where, velocity, speed, acceleration, theta, light };
    const Hero temp = overturn(hero, name);
    return temp;
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
    Hero temp = hero;
    // Acceleration
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, temp.theta);
        const Point acceleration = mul(direction, temp.acceleration);
        if(key[SDL_SCANCODE_W]) temp.velocity = add(temp.velocity, acceleration);
        if(key[SDL_SCANCODE_S]) temp.velocity = sub(temp.velocity, acceleration);
        if(key[SDL_SCANCODE_D]) temp.velocity = add(temp.velocity, rag(acceleration));
        if(key[SDL_SCANCODE_A]) temp.velocity = sub(temp.velocity, rag(acceleration));
    }
    else temp.velocity = mul(temp.velocity, 1.0 - temp.acceleration / temp.speed);
    // Top speed check
    if(mag(temp.velocity) > temp.speed)
        temp.velocity = mul(unt(temp.velocity), temp.speed);
    // Move and check collision
    temp.where = add(temp.where, temp.velocity);
    if(tile(temp.where, walling))
        temp.velocity = zro(), temp.where = hero.where;
    return temp;
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
    return nearby && isportal(ch) ? ch : false;
}

static Hit plow(const Hero hero, const Range range)
{
    Hit hit;
    Point where = hero.where;
    for(int count = 0; count < range.hits; count++)
    {
        hit = cast(where, range.column, range.block);
        where = hit.where;
    }
    return hit;
}

Impact march(const Hero hero, const Range range, const int res)
{
    const Hit hit = plow(hero, range);
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
    if(key[SDL_SCANCODE_I]) temp.fov = zoomed();
    if(key[SDL_SCANCODE_U]) temp.fov = normal();
    return temp;
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
