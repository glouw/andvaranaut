#include "Hero.h"

#include "Portals.h"
#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Line.h"
#include "Light.h"
#include "Map.h"
#include "Console.h"

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
    const float brightness = 750.0;
    const float dtorch = brightness / 10.0;
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
    if(match(field, "brightness"))
        temp.light.brightness = floating(value);
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
    const Point where = { 1.5, 1.5 };
    const Point velocity = { 0.0, 0.0 };
    const float speed = 0.12;
    const float acceleration = 0.0150;
    const float theta = 0.0;
    const Light light = reset();
    const int block = ' ';
    const Party party = WALLING;
    const bool consoling = false;
    const Hero hero = {
        fov, where, velocity, speed, acceleration, theta, light, block, party, consoling
    };
    const Hero fixed = overturn(hero, name);
    return fixed;
}

static Hero spin(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    const float dtheta = 0.1;
    if(key[SDL_SCANCODE_H]) temp.theta -= dtheta;
    if(key[SDL_SCANCODE_L]) temp.theta += dtheta;
    return temp;
}

static Hero move(const Hero hero, char** const walling, const uint8_t* const key)
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
    // Mass-Spring damping
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

static Hit shoot(const Hero hero, char** const walling, const uint8_t* const key)
{
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, hero.theta);
        return cast(hero.where, direction, walling);
    }
    const Hit hit = { 0, 0.0, zro(), W };
    return hit;
}

int handle(const Hero hero, char** const walling, const uint8_t* const key)
{
    const Hit hit = shoot(hero, walling, key);
    const int ch = block(hit.where, walling);
    const float reach = 1.0;
    const int nearby = mag(sub(hero.where, hit.where)) < reach;
    return nearby && isportal(ch) ? ch : false;
}

Impact march(const Hero hero, const Range range, const int res)
{
    const Hit hit = cast(hero.where, range.column, range.block);
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

static Hero zoom(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_P]) temp.fov = zoomed();
    if(key[SDL_SCANCODE_O]) temp.fov = normal();
    return temp;
}

static Hero type(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    const int pressed = lookup(key);
    if(pressed == -1)
        return hero;
    temp.block = pressed;
    if(temp.block < ' ') temp.block = ' ';
    if(temp.block > '~') temp.block = '~';
    return temp;
}

static Hero pick(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_1]) temp.party = FLORING;
    if(key[SDL_SCANCODE_2]) temp.party = WALLING;
    if(key[SDL_SCANCODE_3]) temp.party = CEILING;
    return temp;
}

static char** select(const Map map, const Party party)
{
    switch(party)
    {
        case CEILING: return map.ceiling;
        case WALLING: return map.walling;
        case FLORING: return map.floring;
    }
    return map.walling;
}

static void edit(const Hero hero, const Map map, const uint8_t* const key)
{
    const float reach = 1.1;
    const Point reference = { reach, 0.0 };
    const Point direction = trn(reference, hero.theta);
    const Point where = add(hero.where, direction);
    // Wall operations
    if(key[SDL_SCANCODE_Y])
    {
        const int x = where.x;
        const int y = where.y;
        char** const blocks = select(map, hero.party);
        if(block(where, blocks) != '!')
            blocks[y][x] = hero.block;
    }
}

static Hero console(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    const bool insert = key[SDL_SCANCODE_I];
    const bool normal = key[SDL_SCANCODE_CAPSLOCK]
        || key[SDL_SCANCODE_ESCAPE]
        || key[SDL_SCANCODE_RETURN];
    if(insert) temp.consoling = true;
    if(normal) temp.consoling = false;
    return temp;
}

Hero touch(const Hero hero, const Map map, const uint8_t* const key)
{
    Hero temp = hero;
    temp = console(temp, key);
    if(temp.consoling)
        temp = type(temp, key);
    else
    {
        temp = spin(temp, key);
        temp = move(temp, map.walling, key);
        temp = zoom(temp, key);
        temp = fade(temp);
        temp = pick(temp, key);
        edit(temp, map, key);
    }
    return temp;
}
