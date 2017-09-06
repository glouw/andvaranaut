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
    hero.weapon = HANDS;
    hero.arm = 0.75;
    return hero;
}

static Hero spin(const Hero hero, const Input input)
{
    Hero temp = hero;
    // Keyboard
    if(input.key[SDL_SCANCODE_H]) temp.theta -= 0.1;
    if(input.key[SDL_SCANCODE_L]) temp.theta += 0.1;
    // Mouse
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
    if(input.key[SDL_SCANCODE_P] ||  input.m) temp.fov = lens(0.25);
    if(input.key[SDL_SCANCODE_O] || !input.m) temp.fov = lens(1.00);
    return temp;
}

Impact march(const Hero hero, char** const block, const Point column, const int res)
{
    const Hit hit = cast(hero.where, column, block);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = trn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

bool teleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
    static int last;
    const int buffer = 3;
    if(ticks < last + buffer)
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

static Attack swing(const Hero hero, const Point vect)
{
    Attack attack;
    zero(attack);
    attack.type.swing = true;
    attack.vect = vect;
    attack.power = power(hero.weapon) * mag(vect);
    attack.reach = hero.arm + reach(hero.weapon);
    attack.where = touch(hero, attack.reach);
    attack.area = 2 * attack.reach;
    return attack;
}

static Hero melee(const Hero hero, const Input input)
{
    // Cannot attack with hands - hands are for grabbing
    if(hero.weapon == HANDS)
        return hero;
    Hero temp = hero;
    zero(temp.attack);
    // Attack vector (persistent across function calls)
    static Point vect;
    // Held down mouse button, grow attack vector
    if(input.l)
    {
        vect.x += input.dx;
        vect.y += input.dy;
    }
    // Mouse button let go, calculate attack power from attack vector
    else
    {
        // Attack was a swing if there was weapon movement
        if(vect.x != 0 && vect.y != 0) temp.attack = swing(temp, vect);
        // Reset attack vector as the reset vector is persistent across function calls
        zero(vect);
    }
    return temp;
}

Hero sustain(const Hero hero, const Map map, const Input input)
{
    Hero temp = hero;
    temp = spin(temp, input);
    temp = move(temp, map.walling, input);
    temp = zoom(temp, input);
    temp.torch = burn(temp.torch);
    temp = melee(temp, input);
    return temp;
}
