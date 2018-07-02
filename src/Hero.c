#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

Hero xzhero()
{
    static Hero hero;
    return hero;
}

// A focal value of 1.0 will create a 90 degree field of view.
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
    hero.hps = 4.4f;
    hero.hpsmax = 10.0f;
    hero.mna = 4.0f;
    hero.mnamax = 4.0f;
    hero.ftg = 6.0f;
    hero.ftgmax = 6.0f;
    return hero;
}

static Hero spin(Hero hero, const Input input)
{
    hero.theta += input.dx * input.sx;
    return hero;
}

static Hero yaw(Hero hero, const Input input)
{
    hero.yaw += input.dy * input.sy;
    const float max = 1.80f;
    const float min = 0.20f;
    hero.yaw =
        hero.yaw > max ? max : // Max clamp.
        hero.yaw < min ? min : // Min clamp.
        hero.yaw;
    return hero;
}

static Hero look(const Hero hero, const Input input)
{
    if(input.l)
        return hero;

    return yaw(spin(hero, input), input);
}

// Ducking height.
static float hduck(const Hero hero)
{
    return 0.7f * hero.tall;
}

// Swimming height.
static float hswim(const Hero hero)
{
    return 0.1f * hero.tall;
}

// Handles vertical physics like jumping and faling into pits.
static Hero vert(Hero hero, const Map map, const Input input)
{
    const int onland = xtile(hero.where, map.floring);
    const float tall = onland ? hero.tall : hswim(hero);
    const int jumped = input.key[SDL_SCANCODE_SPACE];
    const int crouch = input.key[SDL_SCANCODE_LCTRL];

    // Jump.
    if(jumped && hero.height <= tall)
        hero.vvel = 0.05f;

    // Apply.
    hero.height += hero.vvel;

    // Fall.
    if(hero.height > tall)
        hero.vvel -= 0.005f;
    else
    {
        hero.vvel = 0.0f;
        hero.height = tall;
    }

    // Clamp jumping and falling.
    const float max = 0.95f;
    const float min = 0.05f;
    if(hero.height > max) hero.height = max;
    if(hero.height < min) hero.height = min;

    // Crouch override - only works on land.
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

    // Acceleration.
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
    // Mass spring damper if not accelerating.
    else hero.velocity = xmul(hero.velocity, 1.0f - hero.acceleration / speed);

    // Current velocity is added to hero velocity if hero is swimming.
    if(swimming) hero.velocity = xadd(hero.velocity, current.velocity);

    // Top speed check.
    if(xmag(hero.velocity) > speed) hero.velocity = xmul(xunt(hero.velocity), speed);

    // Moves and checks for a collision.
    hero.where = xadd(hero.where, hero.velocity);

    // Reset hero if collision.
    if(xtile(hero.where, map.walling))
    {
        hero.velocity = xzpoint();
        hero.where = last;
    }
    return hero;
}

// Teleport up.
static int tup(const Hero hero, const Map map)
{
    return hero.yaw < 1.0f && xmisportal(map.ceiling, hero.where);
}

// Teleport down.
static int tdn(const Hero hero, const Map map)
{
    return hero.yaw > 1.0f && xmisportal(map.floring, hero.where);
}

int xteleporting(const Hero hero, const Map map, const Input input, const Timer tm)
{
    static int last;

    // A delay is required between teleports else hero will
    // quickly teleport between adjacent floors with a single key press.
    // The delay is arbitrary.
    const int delay = 2;
    if(tm.ticks < last + delay)
        return false;

    if(!input.key[SDL_SCANCODE_E])
        return false;

    last = tm.ticks;
    return tup(hero, map)
        || tdn(hero, map);
}

Hero xteleport(Hero hero, const Map map)
{
    // Look up to teleport a floor up. Look down to teleport a floor down.
    if(tup(hero, map))
        hero.floor--;
    if(tdn(hero, map))
    {
        hero.floor++;
        // Gives a falling effect.
        hero.height = 0.80;
    }

    // The teleport effect is done by reseting the hero yaw to the horizon. The torch is also put out.
    hero.yaw = 1.0f;
    hero.torch = xsnuff();
    return hero;
}

Ray xcalc(const Hero hero, const Hit hit, const Sheer sheer, const int yres, const int xres)
{
    const Point end = xsub(hit.where, hero.where);

    // The corrected point is the normal distance from the hero to where the ray hit.
    const Point corrected = xtrn(end, -hero.theta);

    // Wall projections are calculated calculated based hero view parameters, and the corrected distance.
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, xres, hero.fov.a.x, hero.yaw, corrected, hero.height);

    // The engine supports lower, eye level, and upper walls.
    const Ray ray = { trace, corrected, xsheer(projection, sheer), hit.surface, hit.offset, hero.torch };

    // A ray object will hold enough information to draw a wall projection on the screen with the right lighting value.
    return ray;
}

Hero xrecoil(Hero hero, const Method method)
{
    // Generate recoil if ranged.
    if(method == RANGE)
        // TODO
        // Different weapons have different recoil. Add this in.
        hero.vyaw = 0.12f;

    // Apply recoil.
    hero.yaw -= (hero.vyaw *= 0.8f);

    return hero;
}

Hero xsustain(Hero hero, const Map map, const Input input, const Flow current)
{
    hero = look(hero, input);
    hero = vert(hero, map, input);
    hero = move(hero, map, input, current);
    hero.torch = xburn(hero.torch);
    return hero;
}
