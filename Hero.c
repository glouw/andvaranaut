#include "Hero.h"

#include "Frame.h"

#include <SDL2/SDL.h>

Hero spin(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_H]) step.theta -= 0.1;
    if(key[SDL_SCANCODE_L]) step.theta += 0.1;
    return step;
}

Hero move(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = turn(reference, step.theta);
        const Point acceleration = mul(direction, step.acceleration);
        if(key[SDL_SCANCODE_W]) step.velocity = add(step.velocity, acceleration);
        if(key[SDL_SCANCODE_S]) step.velocity = sub(step.velocity, acceleration);
        if(key[SDL_SCANCODE_D]) step.velocity = add(step.velocity, rag(acceleration));
        if(key[SDL_SCANCODE_A]) step.velocity = sub(step.velocity, rag(acceleration));
    }
    else step.velocity = mul(step.velocity, 1.0 - step.acceleration / step.speed);
    if(mag(step.velocity) > step.speed) step.velocity = mul(unt(step.velocity), step.speed);
    step.where = add(step.where, step.velocity);
    if(tile(step.where, walling)) step.velocity = (Point) { 0, 0 }, step.where = hero.where;
    return step;
}

Hit shoot(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = turn(reference, hero.theta);
        return cast(hero.where, direction, walling);
    }
    return (Hit) { 0, 0.0, (Point) { 0.0, 0.0 } };
}

int handle(const Hero hero, char** const walling)
{
    const Hit hit = shoot(hero, walling);
    const int ch = hit.tile + ' ';
    const int near = mag(sub(hero.where, hit.where)) < 1.0;
    const int stepable = ch >= 'a' && ch <= 'z';
    return near && stepable ? ch : 0;
}

typedef struct
{
    Traceline traceline;
    Wall wall;
    Hit hit;
}
Impact;

Impact march(const Hero hero, char** const block, const Point column, const int res)
{
    const Hit hit = cast(hero.where, column, block);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = turn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    return (Impact) { traceline, wall, hit };
}

void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.theta);
    const Display display = lock(gpu);
    for(int yy = 0; yy < res; yy++)
    {
        const Point column = lerp(camera, yy / (double) res);
        const Impact lower = march(hero, blocks.walling, column, res);
        const Frame frame = { gpu, display, yy, res };
        frend(frame, lower.wall, lower.traceline, blocks.floring);
        if(hero.inside)
            crend(frame, lower.wall, lower.traceline, blocks.ceiling);
        else
        {
            const Impact upper = march(hero, blocks.ceiling, column, res);
            wrend(frame, raise(upper.wall, res), upper.hit);
            srend(frame, raise(upper.wall, res));
        }
        wrend(frame, lower.wall, lower.hit);
    }
    unlock(gpu);
    present(gpu);
    const int t1 = SDL_GetTicks();
    const int ms = 15 - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
