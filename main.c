#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

static const int map[][13] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1 },
    { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1 },
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

static struct point sn(const struct point player, const double m, const double b)
{
    const double y = ceil(player.y - 1);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static struct point se(const struct point player, const double m, const double b)
{
    const double x = floor(player.x + 1);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static struct point ss(const struct point player, const double m, const double b)
{
    const double y = floor(player.y + 1);
    const double x = (y - b) / m;
    return (struct point){ x, y };
}

static struct point sw(const struct point player, const double m, const double b)
{
    const double x = ceil(player.x - 1);
    const double y = m * x + b;
    return (struct point){ x, y };
}

static struct point step(const struct point player, const double slope, const int sector)
{
    const double intercept = player.y - slope * player.x;
    const struct point n = sn(player, slope, intercept); // Step north
    const struct point e = se(player, slope, intercept); // Step east
    const struct point s = ss(player, slope, intercept); // Step south
    const struct point w = sw(player, slope, intercept); // Step west
    // Step to the next line
    struct point next;
    switch(sector)
    {
        case 0: next = mag(sub(e, player)) < mag(sub(s, player)) ? e : s; break;
        case 1: next = mag(sub(w, player)) < mag(sub(s, player)) ? w : s; break;
        case 2: next = mag(sub(w, player)) < mag(sub(n, player)) ? w : n; break;
        case 3: next = mag(sub(e, player)) < mag(sub(n, player)) ? e : n; break;
    }
    // Check the map
    const int x = next.x;
    const int y = next.y;
    // Horizontal line?
    if(next.y == floor(next.y))
    {
        if(map[y][x] || map[y - 1][x]) return next; // Is it a wall?
    }
    // Vertical line
    else
    {
        if(map[y][x] || map[y][x - 1]) return next; // Please be a wall...
    }
    // No Wall? Next line
    return step(next, slope, sector);
}

static int quadrant(const double theta)
{
    const double x = cos(theta);
    const double y = sin(theta);
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
    // Player init
    struct point player = { 2.5, 2.5 };
    double angle = 0.0;
    const double d0 = 0.025;
    const double dy = 0.025;
    const double dx = 0.025;
    // Game loop
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(;;)
    {
        const uint32_t t0 = SDL_GetTicks();
        // Keyboard
        SDL_PumpEvents();
        // Keyboard exit
        if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) break;
        // Keyboard rotation
        if(key[SDL_SCANCODE_H]) angle -= d0;
        if(key[SDL_SCANCODE_L]) angle += d0;
        // Player movement
        struct point temp = player;
        if(key[SDL_SCANCODE_W]) temp.x += dx * cos(angle), temp.y += dy * sin(angle);
        if(key[SDL_SCANCODE_S]) temp.x -= dx * cos(angle), temp.y -= dy * sin(angle);
        if(key[SDL_SCANCODE_A]) temp.y -= dx * cos(angle), temp.x += dy * sin(angle);
        if(key[SDL_SCANCODE_D]) temp.y += dx * cos(angle), temp.x -= dy * sin(angle);
        const int x = temp.x;
        const int y = temp.y;
        player = map[y][x] ? player : temp; // Collision detection
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        // Buffer columns
        for(int col = 0; col < xres; col++)
        {
            const double pan = 2.0 * (double)col / xres - 1.0;
            const double focal = 2.5;
            const double radians = atan2(pan, focal);
            const double theta = radians + angle;
            const double slope = tan(theta);
            const struct point wall = step(player, slope, quadrant(theta));
            const struct point ray = sub(wall, player);
            // Fish eye correction
            const double magnitude = mag(ray);
            const double normal = magnitude * cos(radians);
            // Wall height
            const double height = yres / normal;
            const double top = (yres / 2.0) - (height / 2.0);
            const double bot = (yres / 2.0) + (height / 2.0);
            // Torch brightness
            const double brightness = 500.0;
            const double torch = brightness / pow(magnitude, 2.0);
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, torch > 0xFF ? 0xFF : torch, 0x00);
            // Buffer wall
            SDL_RenderDrawLine(renderer, col, top, col, bot);
        }
        // Render walls
        SDL_RenderPresent(renderer);
        const unsigned t1 = SDL_GetTicks();
        fprintf(stderr, "fps: %u\n", 1000 / (t1 - t0));
    }
    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
