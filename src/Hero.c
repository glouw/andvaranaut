#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

Hero xzhero(void)
{
    static Hero hero;
    return hero;
}

static Line lens(const float focal)
{
    const Line fov = {
        { focal, -1.0f },
        { focal, +1.0f },
    };
    return fov;
}

Hero xspawn(const float focal, const Point where, const int floor)
{
    Hero hero = xzhero();
    hero.floor = floor;
    hero.fov = lens(focal);
    hero.where = where;
    hero.speed = 0.12f;
    hero.acceleration = 0.0150f;
    hero.torch = xsnuff();
    hero.reach = 0.75f;
    hero.aura = 12;
    hero.yaw = 1.0f;
    hero.tall = 0.5f;
    hero.height = hero.tall;
    hero.hps = hero.hpsmax = 9.0f;
    hero.mna = hero.mnamax = 10.0f;
    hero.ftg = hero.ftgmax = 30.0f;
    hero.warning = 0.25f;
    return hero;
}

static Hero spin(Hero hero, const Input input)
{
    hero.theta += input.dx * input.sx;
    return hero;
}

static Hero yaw(Hero hero, const Input input)
{
    // Mouse yaw control.
    hero.yaw += input.dy * input.sy;
    const float max = 1.80f;
    const float min = 0.20f;
    hero.yaw =
        hero.yaw > max ? max :
        hero.yaw < min ? min :
        hero.yaw;

    // Change in vertical yaw.
    hero.yaw -= (hero.dvyaw *= 0.8f);

    return hero;
}

static Hero look(const Hero hero, const Input input)
{
    if(input.l)
        return hero;
    return yaw(spin(hero, input), input);
}

static float hduck(const Hero hero)
{
    return 0.7f * hero.tall;
}

static float hswim(const Hero hero)
{
    return 0.1f * hero.tall;
}

static Hero vert(Hero hero, const Map map, const Input input)
{
    const int onland = xtile(hero.where, map.floring);
    const float tall = onland ? hero.tall : hswim(hero);
    const int jumped = input.key[SDL_SCANCODE_SPACE];
    const int crouch = input.key[SDL_SCANCODE_LCTRL];

    if(jumped && hero.height <= tall)
        hero.vvel = 0.05f;

    hero.height += hero.vvel;

    if(hero.height > tall)
        hero.vvel -= 0.005f;
    else
    {
        hero.vvel = 0.0f;
        hero.height = tall;
    }

    const float max = 0.95f;
    const float min = 0.05f;
    if(hero.height > max) hero.height = max;
    if(hero.height < min) hero.height = min;

    if(crouch && onland)
        hero.height = hduck(hero);

    return hero;
}

Point xtouch(const Hero hero)
{
    const Point reference = { hero.reach, 0.0f };
    const Point direction = xtrn(reference, hero.theta);
    return xadd(hero.where, direction);
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0f, 0.0f };
    const Point direction = xtrn(xunt(reference), hero.theta);
    return xmul(direction, hero.acceleration);
}

static Hero move(Hero hero, const Map map, const Input input, const Flow current)
{
    const Point last = hero.where;
    const int swimming = hero.height <= hswim(hero);
    const int crouching = hero.height <= hduck(hero);
    const float speed = swimming ? 0.3f * hero.speed : crouching ? 0.5f * hero.speed : hero.speed;

    // Accelerate?
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
    else // Slow down.
        hero.velocity = xmul(hero.velocity, 1.0f - hero.acceleration / speed);

    if(swimming)
        hero.velocity = xadd(hero.velocity, current.velocity);

    // Top speed check.
    if(xmag(hero.velocity) > speed)
        hero.velocity = xmul(xunt(hero.velocity), speed);

    // Speed apply.
    hero.where = xadd(hero.where, hero.velocity);

    // Collision detection.
    if(xtile(hero.where, map.walling))
    {
        hero.velocity = xzpoint();
        hero.where = last;
    }
    return hero;
}

static int telup(const Hero hero, const Map map)
{
    return hero.yaw < 1.0f && xmisportal(map.ceiling, hero.where);
}

static int teldn(const Hero hero, const Map map)
{
    return hero.yaw > 1.0f && xmisportal(map.floring, hero.where);
}

Hero xteleporting(Hero hero, const Map map, const Input input, const Timer tm)
{
    hero.teleporting = false;
    if(tm.ticks < hero.teleported + 2 || !input.key[SDL_SCANCODE_E])
        return hero;
    hero.teleported = tm.ticks;
    hero.teleporting = telup(hero, map) || teldn(hero, map);
    return hero;
}

Hero xteleport(Hero hero, const Map map)
{
    if(telup(hero, map))
        hero.floor--;
    if(teldn(hero, map))
    {
        hero.floor++;
        hero.height = 0.90;
    }
    hero.yaw = 1.0f;
    hero.torch = xsnuff();
    return hero;
}

Ray xcalc(const Hero hero, const Hit hit, const Sheer sheer, const int yres, const int xres)
{
    const Point end = xsub(hit.where, hero.where);
    const Point corrected = xtrn(end, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, xres, hero.fov.a.x, hero.yaw, corrected, hero.height);
    const Ray ray = { trace, corrected, xsheer(projection, sheer), hit.surface, hit.offset, hero.torch };
    return ray;
}

static Hero recoil(Hero hero, const Method last)
{
    if(last == RANGE)
        hero.dvyaw = 0.12f; // TODO: Different weapons have different recoil?
    return hero;
}

Hero xsustain(Hero hero, const Map map, const Input input, const Flow current, const Method last)
{
    hero = look(hero, input);
    hero = vert(hero, map, input);
    hero = move(hero, map, input, current);
    hero = recoil(hero, last);
    hero.torch = xburn(hero.torch);
    return hero;
}
