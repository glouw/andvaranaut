#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

static Line get_lens(const float focal)
{
    const Line fov = {
        { focal, -1.0f },
        { focal, +1.0f },
    };
    return fov;
}

Hero h_birth(const float focal, const Room room)
{
    static Hero zero;
    Hero hero = zero; // From zero to hero.
    hero.floor = room.floor;
    hero.fov = get_lens(focal);
    hero.where = room.where;
    hero.speed = 0.12f;
    hero.acceleration = 0.0150f;
    hero.torch = t_snuff();
    hero.aura = 12;
    hero.pitch = 1.0f;
    hero.tall = 0.5f;
    hero.height = hero.tall;
    hero.health = hero.health_max = 9.0f;
    hero.mana = hero.mana_max = 10.0f;
    hero.fatigue = hero.fatigue_max = 200.0f;
    hero.warning = 0.25f;
    hero.gauge = g_new(hero.fatigue_max);
    hero.inventory = i_create();
    return hero;
}

static Hero calc_yaw(Hero hero, const Input input)
{
    hero.yaw += input.dx * input.sx;
    hero.yaw -= (hero.d_yaw *= 0.8f);
    return hero;
}

static Hero calc_pitch(Hero hero, const Input input)
{
    const float limit = 0.4f;
    const float max = 2.0f - limit;
    const float min = limit;
    hero.pitch += input.dy * input.sy;
    hero.pitch =
        hero.pitch > max ? max :
        hero.pitch < min ? min :
        hero.pitch;

    hero.pitch -= (hero.d_pitch *= 0.8f);

    return hero;
}

static Hero look(const Hero hero, const Input input)
{
    if(hero.gauge.count > 0)
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

    if(jumped)
        if(hero.height <= tall)
            hero.vertical_velocity = 0.05f;

    hero.height += hero.vertical_velocity;

    if(hero.height > tall)
        hero.vertical_velocity -= 0.005f;
    else
    {
        hero.vertical_velocity = 0.0f;
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

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0f, 0.0f };
    const Point direction = p_turn(p_unit(reference), hero.yaw);
    return p_mul(direction, hero.acceleration);
}

static Hero move(Hero hero, const Map map, const Input input, const Flow current, const Timer tm)
{
    const Point last = hero.where;
    const int swimming = hero.height <= calc_swim_height(hero);
    const int crouchin = hero.height <= calc_duck_height(hero);
    const float slowdown = tm.slowmo ? tm.slowdown : 1.0f;
    const float speed = (1.0f / slowdown) * (swimming ? 0.3f * hero.speed : crouchin ? 0.5f * hero.speed : hero.speed);
    const float accel = (1.0f / slowdown) * hero.acceleration;

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
        hero.velocity = p_mul(hero.velocity, 1.0f - accel / speed);

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

Hero h_place(Hero hero, const Point where, const int floor, const float height)
{
    hero.pitch = 1.0f;
    hero.torch = t_snuff();
    hero.where = where;
    hero.floor = floor;
    hero.height = height;
    return hero;
}

Hero h_teleport(Hero hero, const Map map, const Point where)
{
    if(teleporting_up(hero, map))
        return h_place(hero, where, hero.floor - 1, hero.height);

    if(teleporting_down(hero, map))
        return h_place(hero, where, hero.floor + 1, 0.90);

    return h_place(hero, where, hero.floor, hero.height);
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

// TODO: Different weapons have different recoil?
static Hero recoil(Hero hero, const Method last)
{
    if(last == RANGE)
        hero.d_pitch = 0.12f;
    return hero;
}

Hero h_sustain(Hero hero, const Map map, const Input input, const Flow current, const Method last, const Timer tm)
{
    hero = do_vert_math(hero, map, input);
    hero = look(hero, input);
    hero = move(hero, map, input, current, tm);
    hero = recoil(hero, last);
    hero.torch = t_burn(hero.torch);
    hero.gauge = g_wind(hero.gauge, input, tm);
    return hero;
}

Hero h_struck(Hero hero, const State state, const float damage)
{
    hero.health -= damage;
    if(state == ATTACK_N) hero.d_pitch = +0.025f;
    if(state == ATTACK_S) hero.d_pitch = -0.025f;
    if(state == ATTACK_W) hero.d_yaw = +0.025f;
    if(state == ATTACK_E) hero.d_yaw = -0.025f;
    return hero;
}

int h_close_enough(const Hero hero, const Point other)
{
    return p_eql(hero.where, other, 2.2f);
}
