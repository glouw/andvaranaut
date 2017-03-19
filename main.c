#include <SDL2/SDL.h>

static inline int fl(const double x)
{
    return (int) x - (x < (int) x);
}

static inline int cl(const double x)
{
    return (int) x + (x > (int) x);
}

static inline double dec(const double x)
{
    return x - (int) x;
}

typedef struct
{
    double x;
    double y;
}
Point;

static Point turn(const Point a, const double t)
{
    return (Point) { a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t) };
}

static Point rag(const Point a)
{
    return (Point) { -a.y, a.x };
}

static Point sub(const Point a, const Point b)
{
    return (Point) { a.x - b.x, a.y - b.y };
}

static Point add(const Point a, const Point b)
{
    return (Point) { a.x + b.x, a.y + b.y };
}

static Point mul(const Point a, const double n)
{
    return (Point) { a.x * n, a.y * n };
}

static double mag(const Point a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

static Point dvd(const Point a, const double n)
{
    return (Point) { a.x / n, a.y / n };
}

static Point unt(const Point a)
{
    return dvd(a, mag(a));
}

static double slope(const Point a)
{
    return a.y / a.x;
}

static Point sh(const Point a, const Point b)
{
    const double x = b.x > 0.0 ? fl(a.x + 1.0) : cl(a.x - 1.0);
    const double y = slope(b) * (x - a.x) + a.y;
    return (Point) { x, y };
}

static Point sv(const Point a, const Point b)
{
    const double y = b.y > 0.0 ? fl(a.y + 1.0) : cl(a.y - 1.0);
    const double x = (y - a.y) / slope(b) + a.x;
    return (Point) { x, y };
}

static Point cmp(const Point a, const Point b, const Point c)
{
    return mag(sub(b, a)) < mag(sub(c, a)) ? b : c;
}

static int tile(const Point a, char** const tiles)
{
    const int x = a.x;
    const int y = a.y;
    return tiles[y][x] - ' ';
}

typedef struct
{
    int tile;
    double offset;
    Point where;
}
Hit;

static Hit collision(const Point hook, const Point direction, char** const walling)
{
    const double epsilon = 1e-3;
    return (Hit) { tile(add(hook, mul(direction, epsilon)), walling), dec(hook.x) + dec(hook.y), hook };
}

static Hit cast(const Point ray, const Point direction, char** const walling)
{
    const Point hook = cmp(ray, sh(ray, direction), sv(ray, direction));
    const Hit hit = collision(hook, direction, walling);
    return hit.tile ? hit : cast(hook, direction, walling);
}

typedef struct
{
    Point a;
    Point b;
}
Line;

static Line rotate(const Line l, const double t)
{
    return (Line) { turn(l.a, t), turn(l.b, t) };
}

static Point lerp(const Line l, const double n)
{
    return add(l.a, mul(sub(l.b, l.a), n));
}

typedef struct
{
    SDL_Surface** surface;
    int count;
}
Surfaces;

typedef struct
{
    Surfaces surfaces;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
}
Gpu;

static SDL_Surface* load(const char* const path, const uint32_t format)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const converted = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    SDL_FreeSurface(bmp);
    return converted;
}

static int newlines(FILE* const fp)
{
    char* line = NULL;
    unsigned reads = 0;
    int lines = 0;
    while(getline(&line, &reads, fp) != -1) lines++;
    free(line);
    rewind(fp);
    return lines;
}

static Surfaces pull(const char* const path, const uint32_t format)
{
    FILE* const fp = fopen(path, "r");
    char* line = NULL;
    unsigned reads = 0;
    const int count = newlines(fp);
    SDL_Surface** surface = calloc(count, sizeof(*surface));
    for(int i = 0; i < count; i++)
    {
        getline(&line, &reads, fp);
        line = strtok(line, "# \n");
        surface[i] = load(line, format);
    }
    free(line);
    fclose(fp);
    return (Surfaces) { surface, count };
}

static Gpu setup(const int res)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    return (Gpu) { pull("surfaces.cfg", format), window, renderer, texture };
}

static void release(const Gpu gpu)
{
    SDL_DestroyTexture(gpu.texture);
    SDL_Quit();
    SDL_DestroyWindow(gpu.window);
    SDL_DestroyRenderer(gpu.renderer);
    for(int i = 0; i < gpu.surfaces.count; i++) SDL_FreeSurface(gpu.surfaces.surface[i]);
    free(gpu.surfaces.surface);
}

static void present(const Gpu gpu)
{
    SDL_RenderCopyEx(gpu.renderer, gpu.texture, NULL, NULL, -90.0, NULL, 0);
    SDL_RenderPresent(gpu.renderer);
}

typedef struct
{
    uint32_t* pixels;
    int width;
}
Display;

static Display lock(const Gpu gpu)
{
    void* screen;
    int pitch;
    SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
    return (Display) { screen, pitch / sizeof(uint32_t) };
}

static void unlock(const Gpu gpu)
{
    SDL_UnlockTexture(gpu.texture);
}

typedef struct
{
    int bot;
    int top;
}
Clamped;

typedef struct
{
    int bot;
    int top;
    int height;
    Clamped clamped;
}
Wall;

static double focal(const Line l)
{
    return l.a.x / (l.b.y - l.a.y);
}

static Wall project(const int res, const Line fov, const Point corrected)
{
    const int height = focal(fov) * res / corrected.x;
    const int bot = (res - height) / 2;
    const int top = (res - bot);
    return (Wall) { bot, top, height, (Clamped) { bot < 0 ? 0 : bot, top > res ? res : top } };
}

typedef struct
{
    int inside;
    Line fov;
    Point where;
    Point velocity;
    double speed;
    double acceleration;
    double theta;
}
Hero;

static Hero spin(const Hero hero)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_H]) step.theta -= 0.1;
    if(key[SDL_SCANCODE_L]) step.theta += 0.1;
    return step;
}

static Hero move(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    Hero step = hero;
    if(key[SDL_SCANCODE_W] || key[SDL_SCANCODE_S] || key[SDL_SCANCODE_D] || key[SDL_SCANCODE_A])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = turn(reference, step.theta);
        const Point acceleration = mul(direction, step.acceleration);
        if(key[SDL_SCANCODE_W]) step.velocity = add(step.velocity, acceleration);
        if(key[SDL_SCANCODE_S]) step.velocity = sub(step.velocity, acceleration);
        if(key[SDL_SCANCODE_D]) step.velocity = add(step.velocity, rag(acceleration));
        if(key[SDL_SCANCODE_A]) step.velocity = sub(step.velocity, rag(acceleration));
    }
    else step.velocity = mul(step.velocity, 1.0 - step.acceleration / step.speed);
    if(mag(step.velocity) > step.speed) step.velocity = mul(unt(step.velocity), step.speed);
    step.where = add(step.where, step.velocity);
    if(tile(step.where, walling)) step.velocity = (Point) { 0, 0 }, step.where = hero.where;
    return step;
}

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
}
Blocks;

typedef struct
{
    int rows;
    int inside;
}
Meta;

typedef struct
{
    Meta meta;
    Blocks blocks;
}
Map;

static char** get(FILE* const fp, const int rows)
{
    char* line = NULL;
    unsigned reads = 0;
    char** block = calloc(rows, sizeof(char*));
    for(int row = 0; row < rows; row++)
    {
        getline(&line, &reads, fp);
        block[row] = strdup(line);
    }
    free(line);
    return block;
}

static Meta retrieve(FILE* const fp)
{
    char* line = NULL;
    unsigned reads = 0;
    int rows = 0;
    int inside = 0;
    getline(&line, &reads, fp);
    sscanf(line, "%d %d", &rows, &inside);
    free(line);
    return (Meta) { rows, inside };
}

static Blocks build(FILE* const fp, const int rows)
{
    return (Blocks) { get(fp, rows), get(fp, rows), get(fp, rows) };
}

static Map open(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    const Meta meta = retrieve(fp);
    const Blocks blocks = build(fp, meta.rows);
    fclose(fp);
    return (Map) { meta, blocks };
}

static void close(const Map map)
{
    for(int row = 0; row < map.meta.rows; row++) free(map.blocks.ceiling[row]);
    for(int row = 0; row < map.meta.rows; row++) free(map.blocks.walling[row]);
    for(int row = 0; row < map.meta.rows; row++) free(map.blocks.floring[row]);
    free(map.blocks.ceiling);
    free(map.blocks.walling);
    free(map.blocks.floring);
}

static Map reopen(const Map map, const char* const path)
{
    close(map);
    return open(path);
}

typedef struct
{
    Display display;
    Gpu gpu;
    Wall wall;
    int yy;
    int res;
}
Scanline;

static Scanline raise(const Scanline sl, const int res)
{
    Scanline raised = sl;
    raised.wall.top += raised.wall.height;
    raised.wall.bot += raised.wall.height;
    raised.wall.clamped.bot = raised.wall.bot < 0 ? 0 : raised.wall.bot;
    raised.wall.clamped.top = raised.wall.top > res ? res : raised.wall.top;
    return raised;
}

typedef struct
{
    Line trace;
    Point corrected;
    Line fov;
}
Traceline;

static double ccast(const Line fov, const int res, const int xx)
{
    return focal(fov) * res / (2 * xx - (res - 1));
}

static void wrend(const Scanline sl, const Hit hit)
{
    const SDL_Surface* const surface = sl.gpu.surfaces.surface[hit.tile];
    const int col = surface->w * hit.offset;
    const uint32_t* const pixels = surface->pixels;
    for(int xx = sl.wall.clamped.bot; xx < sl.wall.clamped.top; xx++)
    {
        const int row = surface->h * (xx - sl.wall.bot) / (sl.wall.top - sl.wall.bot);
        sl.display.pixels[xx + sl.yy * sl.display.width] = pixels[row + col * surface->w];
    }
}

static double fcast(const Line fov, const int res, const int xx)
{
    return -ccast(fov, res, xx);
}

static void frend(const Scanline sl, const Traceline tl, char** const floring)
{
    for(int xx = 0; xx < sl.wall.clamped.bot; xx++)
    {
        const Point where = lerp(tl.trace, fcast(tl.fov, sl.res, xx) / tl.corrected.x);
        const SDL_Surface* const surface = sl.gpu.surfaces.surface[tile(where, floring)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        sl.display.pixels[xx + sl.yy * sl.display.width] = pixels[row + col * surface->w];
    }
}

static void crend(const Scanline sl, const Traceline tl, char** const ceiling)
{
    for(int xx = sl.wall.clamped.top; xx < sl.res; xx++)
    {
        const Point where = lerp(tl.trace, ccast(tl.fov, sl.res, xx) / tl.corrected.x);
        const SDL_Surface* const surface = sl.gpu.surfaces.surface[tile(where, ceiling)];
        const int col = surface->w * dec(where.x);
        const int row = surface->h * dec(where.y);
        const uint32_t* const pixels = surface->pixels;
        sl.display.pixels[xx + sl.yy * sl.display.width] = pixels[row + col * surface->w];
    }
}

static void srend(const Scanline sl)
{
    for(int xx = sl.wall.clamped.top; xx < sl.res; xx++)
        sl.display.pixels[xx + sl.yy * sl.display.width] = 0x0;
}

typedef struct
{
    Traceline tl;
    Scanline sl;
    Hit hit;
}
Impact;

static Impact march(const Hero hero, char** const block, const Point column, const int res, const Display display, const Gpu gpu, const int yy)
{
    const Hit hit = cast(hero.where, column, block);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = turn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline tl = { trace, corrected, hero.fov };
    const Scanline sl = { display, gpu, wall, yy, res };
    return (Impact) { tl, sl, hit };
}

static void render(const Hero hero, const Blocks blocks, const int res, const Gpu gpu)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.theta);
    const Display display = lock(gpu);
    for(int yy = 0; yy < res; yy++)
    {
        const Point column = lerp(camera, yy / (double) res);
        const Impact lower = march(hero, blocks.walling, column, res, display, gpu, yy);
        frend(lower.sl, lower.tl, blocks.floring);
        if(hero.inside)
            crend(lower.sl, lower.tl, blocks.ceiling);
        else
        {
            const Impact upper = march(hero, blocks.ceiling, column, res, display, gpu, yy);
            wrend(raise(upper.sl, res), upper.hit);
            srend(raise(upper.sl, res));
        }
        wrend(lower.sl, lower.hit);
    }
    unlock(gpu);
    present(gpu);
    const int t1 = SDL_GetTicks();
    const int ms = 15 - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}

static int done()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT
    || event.key.keysym.sym == SDLK_F1
    || event.key.keysym.sym == SDLK_ESCAPE)
        return 1;
    return 0;
}

static Hit shoot(const Hero hero, char** const walling)
{
    const uint8_t* key = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();
    if(key[SDL_SCANCODE_E])
    {
        const Point reference = { 1.0, 0.0 };
        const Point direction = turn(reference, hero.theta);
        return cast(hero.where, direction, walling);
    }
    return (Hit) { 0, 0.0, (Point) { 0.0, 0.0 } };
}

static int handle(const Hero hero, char** const walling)
{
    const Hit hit = shoot(hero, walling);
    const int ch = hit.tile + ' ';
    const int near = mag(sub(hero.where, hit.where)) < 1.0;
    const int stepable = ch >= 'a' && ch <= 'z';
    return near && stepable ? ch : 0;
}

typedef struct
{
    Point where;
    char* blocks;
}
Portal;

typedef struct
{
    Portal* portal;
    int count;
}
Portals;

static Portals populate(const char* const path)
{
    FILE* const fp = fopen(path, "r");
    const int count = newlines(fp);
    Portal* portal = calloc(count, sizeof(*portal));
    for(int i = 0; i < count; i++)
    {
        char* line = NULL;
        unsigned reads = 0;
        Point where = { 0.0, 0.0 };
        getline(&line, &reads, fp);
        line = strtok(line, " ");
        sscanf(line, "%lf,%lf", &where.x, &where.y);
        portal[i].where = where;
        line = strtok(NULL, " #");
        portal[i].blocks = strdup(line);
    }
    fclose(fp);
    return (Portals) { portal, count };
}

static void destroy(const Portals portals)
{
    for(int i = 0; i < portals.count; i++) free(portals.portal[i].blocks);
    free(portals.portal);
}

int main(const int argc, const char* const* const argv)
{
    Map map = open("maps/start.map");
    Hero hero = {
        .inside = 1,
        .where = { 1.5, 5.5 },
        .velocity  = { 0.0, 0.0 },
        .acceleration = 0.015,
        .speed = 0.12,
        .theta = 0.0,
        .fov = {
            { +1.0, -1.0 },
            { +1.0, +1.0 }
        }
    };
    Portals portals = populate("maps.cfg");
    if(argc != 2) goto end;
    const int res = atoi(argv[1]);
    const Gpu gpu = setup(res);
    while(!done())
    {
        hero = move(hero, map.blocks.walling);
        hero = spin(hero);
        const int ch = handle(hero, map.blocks.walling);
        if(ch)
        {
            const Portal portal = portals.portal[ch - 'a'];
            map = reopen(map, portal.blocks);
            hero.where = portal.where;
            hero.inside = map.meta.inside;
        }
        render(hero, map.blocks, res, gpu);
    }
    release(gpu);
end:close(map);
    destroy(portals);
}
