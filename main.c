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
    const double x = point.x * point.x;
    const double y = point.y * point.y;
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
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const int format = SDL_GetWindowPixelFormat(window);
    SDL_PixelFormat* info = SDL_AllocFormat(format); (void)info; // Good to have but hide for now
    SDL_Texture* const gpu = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, xres, yres);
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
        const int x = temp.x;
        const int y = temp.y;
        hero = map[y][x] ? hero : temp;
        // Buffer columns
        void* bytes = NULL;
        int pitch = 0;
        SDL_LockTexture(gpu, NULL, &bytes, &pitch);
        for(int col = 0; col < xres; col++)
        {
            const double pan = 2.0 * (double)col / xres - 1.0;
            const double focal = 3.0;
            const double sigma = atan2(pan, focal);
            const double radians = sigma + theta;
            const struct point wall = step(hero, tan(radians), quadrant(radians));
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
            const double torch = 300.0;
            const double brightness = torch / (magnitude * magnitude);
            const unsigned lumi = brightness > 0xFF ? 0xFF : brightness;
            // Drawing
            #define max(a, b) (a > b ? b : a)
            #define min(a, b) (a < b ? b : a)
            const int a = 0;
            const int b = min(top, 0);
            const int c = max(bot, yres);
            const int d = yres;
            uint32_t* const pixel = (uint32_t*)bytes;
            for(int j = a; j < b; j++) pixel[j * pitch / sizeof(*pixel) + col] = 0x00 << 16 | 0x00 << 8 | 0x00; // Ceiling
            for(int j = b; j < c; j++) pixel[j * pitch / sizeof(*pixel) + col] = 0x00 << 16 | 0x00 << 8 | lumi; // Wall
            for(int j = c; j < d; j++) pixel[j * pitch / sizeof(*pixel) + col] = 0x00 << 16 | 0x00 << 8 | 0x00; // Floor
        }
        SDL_UnlockTexture(gpu);
        // Render columns
        SDL_RenderCopy(renderer, gpu, NULL, NULL);
        SDL_RenderPresent(renderer);
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int ms = 16 - dt;
        SDL_Delay(ms < 0 ? 0 : ms);
    }
    // Cleanup
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
    return 0;
}
