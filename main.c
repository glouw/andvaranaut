#include <SDL2/SDL.h>
#include <stdbool.h>

static const int map[][13] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
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
    const double x = pow(point.x, 2.0);
    const double y = pow(point.y, 2.0);
    return sqrt(x + y);
}

static struct point sub(const struct point i, const struct point j)
{
    struct point temp = i;
    temp.x -= j.x;
    temp.y -= j.y;
    return temp;
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

static bool hor(const struct point point)
{
    return point.y == floor(point.y);
}

static bool ver(const struct point point)
{
    return point.x == floor(point.x);
}

static struct point step(const struct point hero, const double m, const int quadrant)
{
    const double b = hero.y - m * hero.x;
    // Step to the next line
    struct point point;
    switch(quadrant)
    {
        case 0: point = closest(hero, se(hero, m, b), ss(hero, m, b)); break;
        case 1: point = closest(hero, sw(hero, m, b), ss(hero, m, b)); break;
        case 2: point = closest(hero, sw(hero, m, b), sn(hero, m, b)); break;
        case 3: point = closest(hero, se(hero, m, b), sn(hero, m, b)); break;
    }
    // Was a wall found?
    const int x = point.x;
    const int y = point.y;
    if(hor(point)) { if(map[y][x] || map[y - 1][x]) return point; }
    if(ver(point)) { if(map[y][x] || map[y][x - 1]) return point; }
    // No wall? Find the next line
    return step(point, m, quadrant);
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

int main(void)
{
    const int xres = 800;
    const int yres = 600;
    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 120, 80, xres, yres, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    // Hero init
    struct point hero = { 2.5, 3.5 };
    double theta = 0.0;
    const double d0 = 0.025;
    const double dy = 0.025;
    const double dx = 0.025;
    // Game loop
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(;;)
    {
        const int t0 = SDL_GetTicks();
        // Keyboard update
        SDL_PumpEvents();
        // Keyboard exit
        if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) break;
        // Keyboard rotation
        if(key[SDL_SCANCODE_H]) theta -= d0;
        if(key[SDL_SCANCODE_L]) theta += d0;
        // Keyboard movement
        struct point temp = hero;
        if(key[SDL_SCANCODE_W]) temp.x += dx * cos(theta), temp.y += dy * sin(theta);
        if(key[SDL_SCANCODE_S]) temp.x -= dx * cos(theta), temp.y -= dy * sin(theta);
        if(key[SDL_SCANCODE_A]) temp.y -= dx * cos(theta), temp.x += dy * sin(theta);
        if(key[SDL_SCANCODE_D]) temp.y += dx * cos(theta), temp.x -= dy * sin(theta);
        // Collision detection
        const int x = temp.x;
        const int y = temp.y;
        hero = map[y][x] ? hero : temp;
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        // Buffer columns
        for(int col = 0; col < xres; col++)
        {
            const double pan = 2.0 * (double)col / xres - 1.0;
            const double focal = 3.0;
            const double sigma = atan2(pan, focal);
            const double radians = sigma + theta;
            const double m = tan(radians);
            const struct point wall = step(hero, m, quadrant(radians));
            const struct point ray = sub(wall, hero);
            // Fish eye correction
            const double magnitude = mag(ray);
            const double normal = magnitude * cos(sigma);
            // Column height
            const double zoom = 250.0;
            const double height = round(zoom * focal / normal);
            const double top = (yres / 2.0) - (height / 2.0);
            const double bot = top + height;
            // Column brightness
            const double brightness = 300.0;
            const double torch = brightness / pow(magnitude, 2.0);
            const double lumi = torch > 0xFF ? 0xFF : torch;
            SDL_SetRenderDrawColor(renderer, lumi, lumi, lumi, 0x00);
            SDL_RenderDrawLine(renderer, col, top < 0.0 ? 0.0 : top, col, bot > yres ? yres : bot);
        }
        // Render columns
        SDL_RenderPresent(renderer);
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int ms = 16 - dt < 0 ? 0 : dt;
        SDL_Delay(ms);
    }
    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
