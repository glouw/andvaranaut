#include "Hero.h"

#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "String.h"
#include "Map.h"
#include "Sprites.h"
#include "Console.h"
#include "Torch.h"

static Line lens(const float scale)
{
    Line fov;
    fov.a.x = 1.0;
    fov.a.y = -scale;
    fov.b.x = 1.0;
    fov.b.y = scale;
    return fov;
}

static Point init()
{
    Point where;
    where.x = 3.5;
    where.y = 3.5;
    return where;
}

static Hero spin(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_H]) temp.theta -= 0.1;
    if(key[SDL_SCANCODE_L]) temp.theta += 0.1;
    return temp;
}

static Hero move(const Hero hero, char** const walling, const uint8_t* const key)
{
    Hero temp = hero;
    // Acceleration
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        // Directional unit vector reference, not arm reach
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(unt(reference), temp.theta);
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

static Hero zoom(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_P]) temp.fov = lens(0.25);
    if(key[SDL_SCANCODE_O]) temp.fov = lens(1.00);
    return temp;
}

static bool issprite(const int ascii)
{
    return isalpha(ascii);
}

static Hero pick(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    if(key[SDL_SCANCODE_1]) temp.party = FLORING;
    if(key[SDL_SCANCODE_2]) temp.party = WALLING;
    if(key[SDL_SCANCODE_3]) temp.party = CEILING;
    return temp;
}

static char** interpret(const Map map, const Party party)
{
    return party == CEILING ? map.ceiling : party == WALLING ? map.walling : map.floring;
}

static Point touch(const Hero hero)
{
    const Point reference = { hero.arm, 0.0 };
    const Point direction = trn(reference, hero.theta);
    return add(hero.where, direction);
}

Hero spawn()
{
    Hero hero;
    hero.fov = lens(1.0);
    hero.where = init();
    hero.velocity = zro();
    hero.speed = 0.12;
    hero.acceleration = 0.0150;
    hero.theta = 0.0;
    hero.torch = reset();
    hero.surface = ' ';
    hero.party = WALLING;
    hero.consoling = false;
    hero.arm = 1.1;
    return hero;
}

Impact march(const Hero hero, const Trajectory trajectory, const int res)
{
    const Hit hit = cast(hero.where, trajectory.column, trajectory.block);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = trn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

Hero type(const Hero hero, const uint8_t* const key)
{
    Hero temp = hero;
    const int pressed = lookup(key);
    if(pressed == -1)
        return hero;
    temp.surface = pressed;
    if(temp.surface < ' ') temp.surface = ' ';
    if(temp.surface > '~') temp.surface = '~';
    return temp;
}

void grab(const Hero hero, const Sprites sprites, const uint8_t* const key)
{
    if(!key[SDL_SCANCODE_J])
        return;
    const Point hand = touch(hero);
    for(int i = 0; i < sprites.count; i++)
    {
        if(eql(hand, sprites.sprite[i].where, sprites.sprite[i].width))
        {
            sprites.sprite[i].where = hand;
            sprites.sprite[i].state = GRABBED;
            break;
        }
    }
}

void edit(const Hero hero, const Map map, const uint8_t* const key)
{
    if(!key[SDL_SCANCODE_K])
        return;
    if(issprite(hero.surface))
        return;
    const Point hand = touch(hero);
    char** const blocks = interpret(map, hero.party);
    if(block(hand, blocks) != '!')
    {
        const int x = hand.x;
        const int y = hand.y;
        blocks[y][x] = hero.surface;
    }
}

void save(const Map map, const Sprites sprites, const char* const start, const uint8_t* key)
{
    if(!key[SDL_SCANCODE_F5])
        return;
    dump(map, start);
    entomb(sprites, start);
}

Sprites place(const Hero hero, const Sprites sprites, const uint8_t* const key)
{
    if(!key[SDL_SCANCODE_K])
        return sprites;
    if(!issprite(hero.surface))
        return sprites;
    Sprites temp = sprites;
    if(temp.count == 0)
        retoss(temp.sprite, Sprite, temp.max = 1);
    if(temp.count >= temp.max)
        retoss(temp.sprite, Sprite, temp.max *= 2);
    const Point hand = touch(hero);
        temp.sprite[temp.count++] = registrar(hero.surface, hand);
    return temp;
}

Hero console(const Hero hero, const uint8_t* const key)
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

Hero sustain(const Hero hero, const Map map, const uint8_t* key)
{
    Hero temp = hero;
    temp = spin(temp, key);
    temp = move(temp, map.walling, key);
    temp = zoom(temp, key);
    temp = pick(temp, key);
    temp.torch = fade(temp.torch);
    return temp;
}
