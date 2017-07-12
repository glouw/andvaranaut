#include "Hero.h"

#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "Map.h"
#include "Sprites.h"
#include "Keys.h"
#include "Torch.h"
#include "Input.h"

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
    where.x = 5.5;
    where.y = 5.5;
    return where;
}

extern Hero spawn()
{
    Hero hero;
    hero.fov = lens(1.0);
    hero.where = init();
    hero.velocity = zro();
    hero.speed = 0.12;
    hero.acceleration = 0.0150;
    hero.theta = 0.0;
    hero.torch = out();
    hero.surface = ' ';
    hero.party = WALLING;
    hero.inserting = false;
    hero.consoling = false;
    hero.saved = false;
    hero.arm = 1.0;
    hero.level = 0;
    return hero;
}

static Hero spin(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    // Keyboard
    if(input.key[SDL_SCANCODE_H]) temp.theta -= 0.1;
    if(input.key[SDL_SCANCODE_L]) temp.theta += 0.1;
    // Mouse
    temp.theta += input.dx * input.sx;
    return temp;
}

extern Point touch(const Hero hero, const float reach)
{
    const Point reference = { reach, 0.0 };
    const Point direction = trn(reference, hero.theta);
    return add(hero.where, direction);
}

// Attack direction (dx, dy)
typedef struct
{
    int dx;
    int dy;
    bool swung;
    float power;
}
Attack;

static Attack swing(const Hero hero, const Sprites sprites, const Input input)
{
    static int dx = 0;
    static int dy = 0;
    Attack attack = { 0 };
    if(hero.consoling)
        return attack;
    if(hero.inserting)
        return attack;
    if(input.l)
    {
        dx += input.dx;
        dy += input.dy;
    }
    else
    {
        attack.dx = dx;
        attack.dy = dy;
        attack.power = hypotf(attack.dx, attack.dy);
        dx = 0;
        dy = 0;
        if(attack.dx != 0 && attack.dy != 0) attack.swung = true;
    }
    return attack;
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0, 0.0 };
    const Point direction = trn(unt(reference), hero.theta);
    return mul(direction, hero.acceleration);
}

static Hero move(const Hero hero, char** const walling, const Input input)
{
    if(hero.inserting)
        return hero;
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
        temp.velocity = zro(), temp.where = hero.where;
    return temp;
}

static Hero zoom(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    if(input.key[SDL_SCANCODE_P] ||  input.m) temp.fov = lens(0.25);
    if(input.key[SDL_SCANCODE_O] || !input.m) temp.fov = lens(1.00);
    return temp;
}

static Hero pick(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    if(input.key[SDL_SCANCODE_1]) temp.party = FLORING;
    if(input.key[SDL_SCANCODE_2]) temp.party = WALLING;
    if(input.key[SDL_SCANCODE_3]) temp.party = CEILING;
    return temp;
}

static char** interpret(const Map map, const Party party)
{
    return party == CEILING ? map.ceiling : party == WALLING ? map.walling : map.floring;
}

extern Impact march(const Hero hero, const Trajectory trajectory, const int res)
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

static Hero type(const Hero hero, const Input input)
{
    const int pressed = find(input.key);
    if(pressed == -1)
        return hero;
    Hero temp = hero;
    temp.surface = pressed;
    if(temp.surface < ' ') temp.surface = ' ';
    if(temp.surface > '~') temp.surface = '~';
    return temp;
}

static void edit(const Hero hero, const Map map, const Input input)
{
    if(hero.inserting)
        return;
    if(!hero.consoling)
        return;
    if(!(input.key[SDL_SCANCODE_K] || input.r))
        return;
    if(issprite(hero.surface))
        return;
    // 1.5 to avoid placing block on self (eg. ~sqrt(2.0))
    const Point hand = touch(hero, 1.5);
    const int x = hand.x;
    const int y = hand.y;
    // Out of bounds check - first the rows
    if(y < 0 || y >= map.rows)
        return;
    // Then the columns
    if(x < 0 || x >= (int) strlen(map.walling[y]))
        return;
    // Place the block
    char** const blocks = interpret(map, hero.party);
    if(block(hand, blocks) != '!')
        blocks[y][x] = hero.surface;
}

static Hero save(const Hero hero, const Map map, const Sprites sprites, const Input input)
{
    if(!hero.consoling)
        return hero;
    if(hero.inserting)
        return hero;
    if(!input.key[SDL_SCANCODE_F5])
        return hero;
    Hero temp = hero;
    dump(map, temp.level);
    entomb(sprites, temp.level);
    temp.saved = true;
    return temp;
}

static Hero consoling(const Hero hero, const Input input)
{
    if(hero.consoling)
        return hero;
    Hero temp = hero;
    temp.consoling = input.key[SDL_SCANCODE_GRAVE];
    return temp;
}

static Hero inserting(const Hero hero, const Input input)
{
    if(!hero.consoling)
        return hero;
    Hero temp = hero;
    const bool insert = input.key[SDL_SCANCODE_I];
    const bool normal = input.key[SDL_SCANCODE_CAPSLOCK]
        || input.key[SDL_SCANCODE_ESCAPE]
        || input.key[SDL_SCANCODE_RETURN];
    if(insert) temp.inserting = true, temp.saved = false;
    if(normal) temp.inserting = false;
    return temp.inserting ? type(temp, input) : temp;
}

extern bool teleporting(const Hero hero, const Map map, const Input input, const int ticks)
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

extern Hero teleport(const Hero hero, const Map map)
{
    Hero temp = hero;
    temp.torch = out();
    if(block(hero.where, map.floring) == '~') temp.level++;
    if(block(hero.where, map.ceiling) == '~') temp.level--;
    return temp;
}

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const Input input)
{
    Hero temp = consoling(hero, input);
    temp = inserting(temp, input);
    temp = spin(temp, input);
    temp = move(temp, map.walling, input);
    temp = zoom(temp, input);
    temp = pick(temp, input);
    temp.torch = burn(temp.torch);
    edit(temp, map, input);
    Attack attack = swing(temp, sprites, input);
    if(attack.swung)
        printf("%d %d %f\n", attack.dx, attack.dy, attack.power);
    temp = save(temp, map, sprites, input);
    return temp;
}
