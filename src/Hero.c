#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

static Line lens(const float focal)
{
    const Line fov = {
        { focal, -1.0f },
        { focal, +1.0f },
    };
    return fov;
}

Hero h_birth(const float focal, const Point where, const int floor, const Gauge gg)
{
    static Hero zero; // From zero to hero.
    Hero hero = zero;
    hero.floor = floor;
    hero.fov = lens(focal);
    hero.where = where;
    hero.speed = 0.12f;
    hero.acceleration = 0.0150f;
    hero.torch = t_snuff();
    hero.reach = 0.75f;
    hero.aura = 12;
    hero.pitch = 1.0f;
    hero.tall = 0.5f;
    hero.height = hero.tall;
    hero.hps = hero.hpsmax = 9.0f;
    hero.mna = hero.mnamax = 10.0f;
    hero.ftg = hero.ftgmax = gg.max / gg.divisor;
    hero.warning = 0.25f;
    return hero;
}

static Hero calc_yaw(Hero hero, const Input input)
{
    hero.yaw += input.dx * input.sx;
    return hero;
}

static Hero calc_pitch(Hero hero, const Input input)
{
    hero.pitch += input.dy * input.sy;
    const float max = 1.80f;
    const float min = 0.20f;
    hero.pitch =
        hero.pitch > max ? max :
        hero.pitch < min ? min :
        hero.pitch;

    hero.pitch -= (hero.dpitch *= 0.8f);

    return hero;
}

static Hero look(const Hero hero, const Input input)
{
    if(input.l)
        return hero;
    return calc_pitch(calc_yaw(hero, input), input);
}

static float calc_duck_height(const Hero hero)
{
    return 0.7f * hero.tall;
}

static float calc_swim_height(const Hero hero)
{
    return 0.1f * hero.tall;
}

static Hero do_vert_math(Hero hero, const Map map, const Input input)
{
    const int onland = p_tile(hero.where, map.floring);
    const float tall = onland ? hero.tall : calc_swim_height(hero);
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
        hero.height = calc_duck_height(hero);

    return hero;
}

Point h_touch(const Hero hero)
{
    const Point reference = { hero.reach, 0.0f };
    const Point direction = p_turn(reference, hero.yaw);
    return p_add(hero.where, direction);
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0f, 0.0f };
    const Point direction = p_turn(p_unit(reference), hero.yaw);
    return p_mul(direction, hero.acceleration);
}

static Hero move(Hero hero, const Map map, const Input input, const Flow current)
{
    const Point last = hero.where;
    const int swimming = hero.height <= calc_swim_height(hero);
    const int crouchin = hero.height <= calc_duck_height(hero);
    const float speed = swimming ? 0.3f * hero.speed : crouchin ? 0.5f * hero.speed : hero.speed;

    // Accelerate?
    if(input.key[SDL_SCANCODE_W]
    || input.key[SDL_SCANCODE_S]
    || input.key[SDL_SCANCODE_D]
    || input.key[SDL_SCANCODE_A])
    {
        const Point acceleration = accelerate(hero);
        if(input.key[SDL_SCANCODE_W]) hero.velocity = p_add(hero.velocity, acceleration);
        if(input.key[SDL_SCANCODE_S]) hero.velocity = p_sub(hero.velocity, acceleration);
        if(input.key[SDL_SCANCODE_D]) hero.velocity = p_add(hero.velocity, p_rot90(acceleration));
        if(input.key[SDL_SCANCODE_A]) hero.velocity = p_sub(hero.velocity, p_rot90(acceleration));
    }
    else // Slow down.
        hero.velocity = p_mul(hero.velocity, 1.0f - hero.acceleration / speed);

    if(swimming)
        hero.velocity = p_add(hero.velocity, current.velocity);

    // Top speed check.
    if(p_mag(hero.velocity) > speed)
        hero.velocity = p_mul(p_unit(hero.velocity), speed);

    // Speed apply.
    hero.where = p_add(hero.where, hero.velocity);

    // Collision detection.
    if(p_tile(hero.where, map.walling))
    {
        static Point zero;
        hero.velocity = zero;
        hero.where = last;
    }
    return hero;
}

static int teleporting_up(const Hero hero, const Map map)
{
    return hero.pitch < 1.0f && m_is_portal(map.ceiling, hero.where);
}

static int teleporting_down(const Hero hero, const Map map)
{
    return hero.pitch > 1.0f && m_is_portal(map.floring, hero.where);
}

Hero h_teleporting(Hero hero, const Map map, const Input input, const Timer tm)
{
    hero.teleporting = false;
    if(tm.ticks < hero.teleported + 2 || !input.key[SDL_SCANCODE_E])
        return hero;
    hero.teleported = tm.ticks;
    hero.teleporting = teleporting_up(hero, map) || teleporting_down(hero, map);
    return hero;
}

Hero h_teleport(Hero hero, const Map map)
{
    if(teleporting_up(hero, map))
        hero.floor--;
    if(teleporting_down(hero, map))
    {
        hero.floor++;
        hero.height = 0.90;
    }
    hero.pitch = 1.0f;
    hero.torch = t_snuff();
    return hero;
}

Ray h_cast(const Hero hero, const Hit hit, const Sheer sheer, const int yres, const int xres)
{
    const Point end = p_sub(hit.where, hero.where);
    const Point corrected = p_turn(end, -hero.yaw);
    const Line trace = { hero.where, hit.where };
    const Projection projection = p_project(yres, xres, hero.fov.a.x, hero.pitch, corrected, hero.height);
    const Ray ray = { trace, corrected, p_sheer(projection, sheer), hit.surface, hit.offset, hero.torch };
    return ray;
}

static Hero recoil(Hero hero, const Method last)
{
    if(last == RANGE)
        hero.dpitch = 0.12f; // TODO: Different weapons have different recoil?
    return hero;
}

Hero h_sustain(Hero hero, const Map map, const Input input, const Flow current, const Method last)
{
    hero = do_vert_math(hero, map, input);
    hero = look(hero, input);
    hero = move(hero, map, input, current);
    hero = recoil(hero, last);
    hero.torch = t_burn(hero.torch);
    return hero;
}
