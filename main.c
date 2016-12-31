#include <SDL2/SDL.h>
#include <stdbool.h>

static const uint8_t map[][13] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
};

struct point
{
    double x, y;
};

static double mag(const struct point point)
{
    const double x = point.x * point.x;
    const double y = point.y * point.y;
    return sqrt(x + y);
}

static struct point sub(const struct point i, const struct point j)
{
    return (struct point){ i.x - j.x, i.y - j.y };
}

static struct point add(const struct point i, const struct point j)
{
    return (struct point){ i.x + j.x, i.y + j.y };
}

static struct point mul(const struct point i, const double n)
{
    return (struct point){ i.x * n, i.y * n };
}

static struct point sn(const struct point hero, const double m, const double b) // Step north
{
    const double y = ceil(hero.y - 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static struct point se(const struct point hero, const double m, const double b) // Step east
{
    const double x = floor(hero.x + 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static struct point ss(const struct point hero, const double m, const double b) // Step south
{
    const double y = floor(hero.y + 1.0);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static struct point sw(const struct point hero, const double m, const double b) // Step west
{
    const double x = ceil(hero.x - 1.0);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static struct point closest(const struct point hero, const struct point i, const struct point j)
{
    return mag(sub(i, hero)) < mag(sub(j, hero)) ? i : j;
}

static double fn(const struct point point) // Facing north
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && map[y][x] && map[y - 1][x] == 0;
}

static double fe(const struct point point) // Facing east
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && map[y][x] == 0 && map[y][x - 1];
}

static double fs(const struct point point) // Facing south
{
    const int x = point.x;
    const int y = point.y;
    return point.y - y == 0.0 && map[y][x] == 0 && map[y - 1][x];
}

static double fw(const struct point point) // Facing west
{
    const int x = point.x;
    const int y = point.y;
    return point.x - x == 0.0 && map[y][x] && map[y][x - 1] == 0;
}

static bool hor(const struct point point)
{
    return fn(point) || fs(point);
}

static bool ver(const struct point point)
{
    return fe(point) || fw(point);
}

static double percentage(const struct point point)
{
    if(fn(point)) return 0.0 + (point.x - (int)point.x);
    if(fe(point)) return 1.0 - (point.y - (int)point.y);
    if(fs(point)) return 1.0 - (point.x - (int)point.x);
    if(fw(point)) return 0.0 + (point.y - (int)point.y);
    return 0.0;
}

static int quadrant(const double radians)
{
    const double x = cos(radians);
    const double y = sin(radians);
    if(x >= 0.0 && y >= 0.0) return 0;
    if(x <= 0.0 && y >= 0.0) return 1;
    if(x <= 0.0 && y <= 0.0) return 2;
    if(x >= 0.0 && y <= 0.0) return 3;
    return -1;
}

static struct point step(const struct point hero, const double m, const double b, const int q)
{
    struct point point;
    switch(q)
    {
        case 0: point = closest(hero, se(hero, m, b), ss(hero, m, b)); break;
        case 1: point = closest(hero, sw(hero, m, b), ss(hero, m, b)); break;
        case 2: point = closest(hero, sw(hero, m, b), sn(hero, m, b)); break;
        case 3: point = closest(hero, se(hero, m, b), sn(hero, m, b)); break;
    }
    return hor(point) || ver(point) ? point : step(point, m, b, q);
}

static bool collision(const struct point point)
{
    const int x = point.x;
    const int y = point.y;
    return map[y][x];
}

static SDL_Surface* load(const char* path, const uint32_t format)
{
    SDL_Surface* const surface = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const convert = SDL_ConvertSurface(surface, allocation, 0);
    return convert;
}

int main(void)
{
    const int xres = 640;
    const int yres = 360;
    SDL_Init(SDL_INIT_VIDEO);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Window* const window = SDL_CreateWindow("water", 25, 120, xres, yres, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* const surface = load("textures/wall.bmp", format);
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, xres, yres);
    // Hero
    struct point hero = { 2.5, 2.5 }; double theta = 0.0;
    const double d0 = 0.060;
    const double dy = 0.080;
    const double dx = 0.080;
    // Game loop
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(;;)
    {
        const int t0 = SDL_GetTicks();
        // Keyboard update
        SDL_PumpEvents();
        // Keyboard exit
        if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) break;
        if(key[SDL_SCANCODE_RCTRL] && key[SDL_SCANCODE_D]) break;
        // Keyboard rotation
        if(key[SDL_SCANCODE_H]) theta -= d0;
        if(key[SDL_SCANCODE_L]) theta += d0;
        // Keyboard movement
        struct point temp = hero;
        const struct point direction = { dx * cos(theta), dy * sin(theta) };
        if(key[SDL_SCANCODE_W]) temp.x += direction.x, temp.y += direction.y;
        if(key[SDL_SCANCODE_S]) temp.x -= direction.x, temp.y -= direction.y;
        if(key[SDL_SCANCODE_A]) temp.y -= direction.x, temp.x += direction.y;
        if(key[SDL_SCANCODE_D]) temp.y += direction.x, temp.x -= direction.y;
        // Collision detection
        hero = collision(temp) ? hero : temp;
        // Cast a ray for each column of the screen
        void* bytes; int null; SDL_LockTexture(texture, NULL, &bytes, &null);
        uint32_t* const screen = (uint32_t*)bytes;
        for(int col = 0; col < xres; col++)
        {
            const double pan = 2.0 * (double)col / xres - 1.0;
            const double focal = 1.0;
            const double sigma = atan2(pan, focal);
            const double radians = sigma + theta;
            const double m = tan(radians);
            const double b = hero.y - m * hero.x;
            const double q = quadrant(radians);
            const struct point wall = step(hero, m, b, q);
            const struct point ray = sub(wall, hero);
            // Fish eye correction
            const double normal = mag(ray) * cos(sigma);
            // Wall height
            const double size = yres;
            const double height = round(size * focal / normal);
            const double top = (yres / 2.0) - (height / 2.0);
            const double bot = top + height;
            // Prepare column buffer
            const int ct = 0;
            const int cb = top < 0 ? 0 : top;
            const int ft = bot > yres ? yres : bot;
            const int fb = yres;
            const int w = surface->w;
            const int h = surface->h;
            // Surface for wall, floor, and ceiling
            const uint32_t* const piece = surface->pixels;
            // Buffer wall
            const int x = w * percentage(wall);
            for(int row = cb; row < ft; row++)
            {
                const int y = h * (row - top) / height;
                screen[row * xres + col] = piece[y * w + x];
            }
            // Cache floor and ceiling tiles
            struct cache { int x, y; } caches[yres / 2];
            for(int i = 0, row = ft; row < fb; i++, row++)
            {
                const double dis = yres / (2.0 * row - yres);
                const double t = dis / normal;
                const struct point tile = add(hero, mul(ray, t));
                const struct cache cache = {
                    w * (tile.x - (int)tile.x),
                    h * (tile.y - (int)tile.y),
                };
                caches[i] = cache;
            }
            // Buffer floor
            for(int i = 0, row = ft; row < fb; i++, row++)
            {
                const struct cache cache = caches[i];
                screen[row * xres + col] = piece[cache.y * w + cache.x];
            }
            // Buffer ceiling (Mirrors the floor)
            for(int i = 0, row = ct; row < cb; i++, row++)
            {
                const struct cache cache = caches[cb - 1 - i];
                screen[row * xres + col] = piece[cache.y * w + cache.x];
            }
        }
        SDL_UnlockTexture(texture);
        // Render columns
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int fps = 60;
        const int delay = 1000 / fps - dt;
        SDL_Delay(delay < 0 ? 0 : delay);
    }
    // Cleanup
    SDL_FreeSurface(surface);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
    return 0;
}
