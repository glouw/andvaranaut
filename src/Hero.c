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

Hero h_birth(const float focal, const Map map[], const int floor)
{
    static Hero zero;
    Hero hero = zero;

    // From zero to hero.
    hero.floor = floor;
    hero.fov = get_lens(focal);
    hero.where = map[floor].trapdoors.point[0];
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
    hero.pitch += input.dy * input.sy;
    const float max = 1.80f;
    const float min = 0.20f;
    hero.pitch =
        hero.pitch > max ? max :
        hero.pitch < min ? min :
        hero.pitch;

    hero.pitch -= (hero.d_pitch *= 0.8f);

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

Hero h_teleport(Hero hero, const Map map, const Point where)
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
    hero.where = where;
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
    // TODO: Different weapons have different recoil?
    if(last == RANGE)
        hero.d_pitch = 0.12f;
    return hero;
}

Hero h_sustain(Hero hero, const Map map, const Input input, const Flow current, const Method last, const Timer tm)
{
    hero = do_vert_math(hero, map, input);
    hero = look(hero, input);
    hero = move(hero, map, input, current);
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

Hero h_transport(Hero hero, const Map map, const Input in)
{
    int key = EOF;
    if(in.key[SDL_SCANCODE_A]) key = 'A';
    if(in.key[SDL_SCANCODE_B]) key = 'B';
    if(in.key[SDL_SCANCODE_C]) key = 'C';
    if(in.key[SDL_SCANCODE_D]) key = 'D';
    if(in.key[SDL_SCANCODE_E]) key = 'E';
    if(in.key[SDL_SCANCODE_F]) key = 'F';
    if(in.key[SDL_SCANCODE_G]) key = 'G';
    if(in.key[SDL_SCANCODE_H]) key = 'H';
    if(in.key[SDL_SCANCODE_I]) key = 'I';
    if(in.key[SDL_SCANCODE_J]) key = 'J';
    if(in.key[SDL_SCANCODE_K]) key = 'K';
    if(in.key[SDL_SCANCODE_L]) key = 'L';
    if(in.key[SDL_SCANCODE_M]) key = 'M';
    if(in.key[SDL_SCANCODE_N]) key = 'N';
    if(in.key[SDL_SCANCODE_O]) key = 'O';
    if(in.key[SDL_SCANCODE_P]) key = 'P';
    if(in.key[SDL_SCANCODE_Q]) key = 'Q';
    if(in.key[SDL_SCANCODE_R]) key = 'R';
    if(in.key[SDL_SCANCODE_S]) key = 'S';
    if(in.key[SDL_SCANCODE_T]) key = 'T';
    if(in.key[SDL_SCANCODE_U]) key = 'U';
    if(in.key[SDL_SCANCODE_V]) key = 'V';
    if(in.key[SDL_SCANCODE_W]) key = 'W';
    if(in.key[SDL_SCANCODE_X]) key = 'X';
    if(in.key[SDL_SCANCODE_Y]) key = 'Y';
    if(in.key[SDL_SCANCODE_Z]) key = 'Z';

    if(key != EOF)
    {
        const int index = key - 'A';
        if(index < map.rooms.count)
            hero = h_teleport(hero, map, map.rooms.wheres[index]);
    }
    return hero;
}
