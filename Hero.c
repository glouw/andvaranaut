#include "Hero.h"

#include "Scanline.h"

Hero spawn(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    char* line = NULL;
    unsigned reads = 0;
    // Hero where
    Point where = { 0.0, 0.0 };
    getline(&line, &reads, fp);
    sscanf(line, "%f,%f", &where.x, &where.y);
    fclose(fp);
    const Hero hero = {
        { { +1.0, -1.0 }, { +1.0, +1.0 } },
        where,
        { 0.0, 0.0 },
        0.12,
        0.0150,
        { 0.0, 0.0 },
    };
    return hero;
}

Hero spin(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero temp = hero;
    if(key[SDL_SCANCODE_H]) temp.angle.theta -= 0.1;
    if(key[SDL_SCANCODE_L]) temp.angle.theta += 0.1;
    // Angle theta percentage [0..1] for [0..2*pi] calculated using a sawtooth
    const float pi = acos(-1.0);
    temp.angle.percent = 0.5 - atanf(1.0 / tanf(temp.angle.theta / 2.0)) / pi;
    return temp;
}

Hero move(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, step.angle.theta);
        const Point acceleration = mul(direction, step.acceleration);
        if(key[SDL_SCANCODE_W]) step.velocity = add(step.velocity, acceleration);
        if(key[SDL_SCANCODE_S]) step.velocity = sub(step.velocity, acceleration);
        if(key[SDL_SCANCODE_D]) step.velocity = add(step.velocity, rag(acceleration));
        if(key[SDL_SCANCODE_A]) step.velocity = sub(step.velocity, rag(acceleration));
    }
    else step.velocity = mul(step.velocity, 1.0 - step.acceleration / step.speed);
    if(mag(step.velocity) > step.speed) step.velocity = mul(unt(step.velocity), step.speed);
    step.where = add(step.where, step.velocity);
    if(tile(step.where, walling)) step.velocity = zro(), step.where = hero.where;
    return step;
}

static Hit shoot(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = trn(reference, hero.angle.theta);
        return cast(hero.where, direction, walling);
    }
    const Hit hit = { 0, 0.0, zro() };
    return hit;
}

int handle(const Hero hero, char** const walling)
{
    const Hit hit = shoot(hero, walling);
    const int ch = hit.tile + ' ';
    const int near = mag(sub(hero.where, hit.where)) < 1.0;
    const int stepable = ch >= 'a' && ch <= 'z';
    return near && stepable ? ch : 0;
}

static Hit plow(const Hero hero, char** const block, const Point column, const int hits)
{
    if(hits == 1)
        return cast(hero.where, column, block);
    Hit hit;
    Point where = hero.where;
    for(int count = 0; count < hits; count++)
    {
        hit = cast(where, column, block);
        where = hit.where;
    }
    return hit;
}

typedef struct
{
    Traceline traceline;
    Wall wall;
    Hit hit;
}
Impact;

static Impact march(const Hero hero, char** const block, const Point column, const int res, const int hits)
{
    const Hit hit = plow(hero, block, column, hits);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = trn(ray, -hero.angle.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.angle.theta);
    const Display display = lock(gpu);
    // Precomputes floor and ceiling casts
    float* const party = (float*) calloc(res, sizeof(*party));
    for(int x = 0; x < res; x++)
        party[x] = x < res / 2 ? fcast(hero.fov, res, x) : ccast(hero.fov, res, x);
    // Saves and reserves computations from floorcasting for ceiling casting
    for(int y = 0; y < res; y++)
    {
        const Point column = lerp(camera, y / (float) res);
        const Impact lower = march(hero, blocks.walling, column, res, 1);
        const Scanline scanline = { gpu, display, y, res };
        srend(scanline, hero.angle.percent);
        const int uppers = 5;
        for(int hits = uppers; hits > 0; hits--)
        {
            const Impact upper = march(hero, blocks.ceiling, column, res, hits);
            wrend(scanline, raise(upper.wall, res), upper.hit);
        }
        wrend(scanline, lower.wall, lower.hit);
        Point* const wheres = frend(scanline, lower.wall, lower.traceline, blocks.floring, party);
        crend(scanline, lower.wall, wheres, blocks.ceiling);
        free(wheres);
    }
    free(party);
    unlock(gpu);
    present(gpu);
    const int t1 = SDL_GetTicks();
    const int ms = 15 - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}

Hero teleport(const Hero hero, const Portal portal)
{
    Hero temp = hero;
    temp.where = portal.where;
    return temp;
}
