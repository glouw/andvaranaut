#include "Hero.h"

#include "util.h"

#include <SDL2/SDL.h>

// A focal value of 1.0 will create a 90 degree field of view.
static Line lens(const float focal)
{
    Line fov;
    xzero(fov);
    fov.a.x = focal;
    fov.a.y = -1.0f;
    fov.b.x = focal;
    fov.b.y = +1.0f;
    return fov;
}

Hero xspawn(const float focal, const Point where, const int floor)
{
    Hero hero;
    xzero(hero);
    hero.floor = floor;
    hero.fov = lens(focal);
    hero.where = where;
    hero.speed = 0.12f;
    hero.acceleration = 0.0150f;
    hero.torch = xsnuff();
    hero.arm = 0.75f;
    hero.aura = 12;
    hero.yaw = 1.0;
    hero.tall = 0.5f;
    hero.height = hero.tall;
    hero.hlth = 0.4f;
    hero.maxhlth = 6;
    hero.mana = 0.6f;
    hero.maxmana = 6;
    hero.fatg = 0.6f;
    hero.maxfatg = 6;
    hero.wep = SHORT;
    return hero;
}

static Hero spin(Hero hero, const Input input)
{
    if(input.l && hero.wep != HANDS)
        return hero;
    hero.theta += input.dx * input.sx;
    return hero;
}

// Head up and down.
static Hero yaw(Hero hero, const Input input)
{
    if(input.l && hero.wep != HANDS)
        return hero;
    hero.yaw += input.dy * input.sy;
    const float max = 1.99f;
    const float min = 0.01f;
    hero.yaw =
        hero.yaw > max ? max : /* Max clamp. */
        hero.yaw < min ? min : /* Min clamp. */
        hero.yaw;
    return hero;
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

static Hero vert(Hero hero, const Map map, const Input input)
{
    const int land = xtile(hero.where, map.floring);
    const float tall = land ? hero.tall : hswim(hero);
    // Jump.
    if(input.key[SDL_SCANCODE_SPACE] && hero.height <= tall) hero.vvel = 0.05f;
    // Apply.
    hero.height += hero.vvel;
    // Fall.
    if(hero.height > tall) hero.vvel -= 0.005f; else hero.vvel = 0.0f, hero.height = tall;
    // Clamp jumping and falling.
    const float max = 0.95f;
    const float min = 0.05f;
    if(hero.height > max) hero.height = max;
    if(hero.height < min) hero.height = min;
    // Crouch overload - only works on land.
    if(input.key[SDL_SCANCODE_LCTRL] && land) hero.height = hduck(hero);
    return hero;
}

Point xtouch(const Hero hero, const float reach)
{
    const Point reference = { reach, 0.0f };
    const Point direction = xtrn(reference, hero.theta);
    return xadd(hero.where, direction);
}

// Returns an acceleration vector based on the hero's position.
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
        xzero(hero.velocity);
        hero.where = last;
    }
    return hero;
}

// Neck is craned up or down, not eye level.
static int iscraned(const float yaw)
{
    return yaw > 1.0f || yaw < 1.0f;
}

int xteleporting(const Hero hero, const Map map, const Input input, const int ticks)
{
    static int last;
    // A delay is required between teleports else hero will
    // quickly teleport between adjacent floors with a single key press.
    // The delay is arbitrary (whatever feels best).
    const int delay = 2;
    if(ticks < last + delay)
        return false;
    if(!input.key[SDL_SCANCODE_E])
        return false;
    last = ticks;
    return iscraned(hero.yaw) && xisportal(map, hero.where);
}

Hero xteleport(Hero hero, const Map map)
{
    // Look up to teleport a floor up. Look down to teleport a floor down.
    if(xblok(hero.where, map.floring) == '~' && hero.yaw > 1.0f) hero.floor++;
    if(xblok(hero.where, map.ceiling) == '~' && hero.yaw < 1.0f) hero.floor--;
    // The teleport effect is done by reseting the hero yaw to the horizon.
    // The torch is also put out.
    hero.yaw = 1.0f;
    hero.torch = xsnuff();
    return hero;
}

Ray xcalc(const Hero hero, const Hit hit, const float shift, const int yres, const int xres)
{
    const Point end = xsub(hit.where, hero.where);
    // The corrected point is the normal distance from the hero to where the ray hit.
    const Point corrected = xtrn(end, -hero.theta);
    // Wall projections are calculated calculated based hero view parameters, and the corrected distance.
    const Line trace = { hero.where, hit.where };
    const Projection projection = xproject(yres, xres, hero.fov.a.x, hero.yaw, corrected, hero.height);
    // The engine supports lower, eye level, and upper walls. A shift value greater than zero will
    // stack the wall projection above the eye level walls while a shift value less than zero will
    // drop the wall projection below the eye level walls. The shift value determines the height of
    // the ceiling or the flooring. If a value of 0.0 is given for the shift, wall stacking, nor
    // dropping, will occur; the ceiling and flooring will not be shifted either.
    const Ray ray = {
        trace,
        corrected,
        shift > 0.0f ? xstack(projection, shift) : shift < 0.0f ? xdrop(projection, shift) : projection,
        hit.surface,
        hit.offset,
        hero.torch
    };
    // A ray object will hold enough information to draw a wall
    // projection on the screen with the right lighting value.
    return ray;
}

Hero xsustain(Hero hero, const Map map, const Input input, const Flow current)
{
    hero = spin(hero, input);
    hero = vert(hero, map, input);
    hero = move(hero, map, input, current);
    hero = yaw(hero, input);
    hero.torch = xburn(hero.torch);
    return hero;
}
