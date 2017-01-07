#include "map.h"
#include "geom.h"
#include "precalc.h"

#include <SDL2/SDL.h>

static SDL_Surface*
loadbmp(const uint32_t format, const char* path)
{
    SDL_Surface* const surface = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const convert = SDL_ConvertSurface(surface, allocation, 0);
    return convert;
}

int
main(void)
{
    const int xres = 800;
    const int yres = 400;
    // Resolution based calculation optimizations
    precalc_optimize(xres, yres);
    // Loads the map
    map_load("maps/test");
    // Boots up SDL
    SDL_Init(SDL_INIT_VIDEO);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    const uint32_t mode = SDL_RENDERER_ACCELERATED;
    const uint32_t estate = SDL_WINDOW_SHOWN;
    SDL_Window* const window = SDL_CreateWindow("water", 25, 120, xres, yres, estate);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, mode);
    // Loads all textures and sprites
    SDL_Surface* tiles[10] = { NULL };
    SDL_Surface* sprts[10] = { NULL };
    #define T(n, tile) tiles[n] = loadbmp(format, "tiles/"tile);
    #define LD_TILES                                           \
        T(0, "error.bmp")                                      \
        T(1, "stone.bmp")                                      \
        T(2, "wood.bmp")
    #define S(n, sprt) sprts[n] = loadbmp(format, "sprts/"sprt);
    #define LD_SPRTS                                           \
        S(0, "soldier.bmp")
    LD_TILES
    LD_SPRTS
    #undef T
    #undef S
    #undef LD_TILES
    #undef LD_SPRTS
    // Readys the GPU
    const uint32_t access = SDL_TEXTUREACCESS_STREAMING;
    SDL_Texture* const gpu = SDL_CreateTexture(renderer, format, access, xres, yres);
    // Creates the hero
    struct point hero = { 2.5, 4.5 }; double theta = 0.0;
    const double d0 = 0.080;
    const double dy = 0.100;
    const double dx = 0.100;
    // Loops the game until <CTRL+D> is pressed
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(;;)
    {
        const int t0 = SDL_GetTicks();
        // Keyboard: update
        SDL_PumpEvents();
        // Keyboard: exit
        if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) break;
        if(key[SDL_SCANCODE_RCTRL] && key[SDL_SCANCODE_D]) break;
        // Keyboard: rotation
        if(key[SDL_SCANCODE_H]) theta -= d0;
        if(key[SDL_SCANCODE_L]) theta += d0;
        // Keyboard: movement
        struct point temp = hero;
        const struct point direction = { dx * cos(theta), dy * sin(theta) };
        if(key[SDL_SCANCODE_W]) temp.x += direction.x, temp.y += direction.y;
        if(key[SDL_SCANCODE_S]) temp.x -= direction.x, temp.y -= direction.y;
        if(key[SDL_SCANCODE_A]) temp.y -= direction.x, temp.x += direction.y;
        if(key[SDL_SCANCODE_D]) temp.y += direction.x, temp.x -= direction.y;
        // Detects hero to wall collisions
        hero = geom_wallcollision(temp) ? hero : temp;
        // Acquires the GPU
        void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
        uint32_t* const screen = (uint32_t*)bytes;
        // Casts a ray for each column of the screen
        for(int col = 0; col < xres; col++)
        {
            const double radians = precalc_sigmas[col] + theta;
            const struct point wall = geom_cast(hero, radians);
            if(geom_outofbounds(wall)) continue;
            const struct point rayw = geom_sub(wall, hero);
            // Corrects the fish eye
            const double normal = geom_mag(rayw) * cos(precalc_sigmas[col]);
            // Calculates the wall height
            const double size = yres;
            const double height = round(precalc_focal * size / normal);
            const double top = round((yres / 2.0) - (height / 2.0));
            const double bot = top + height;
            // Clamps the top and the bottom of the ceilling and floor to the screen
            const int ctc = 0;
            const int cbc = top < 0 ? 0 : top;
            const int ftc = bot > yres ? yres : bot;
            const int fbc = yres;
            // Buffers walling column in GPU memory
            const int t = geom_getwallingtile(wall);
            const SDL_Surface* const walling = tiles[t];
            const int w = walling->w;
            const int h = walling->h;
            const int x = w * geom_wallpercentage(wall);
            for(int row = cbc; row < ftc; row++)
            {
                const uint32_t* const pixels = walling->pixels;
                const int y = h * (row - top) / height;
                screen[row * xres + col] = pixels[y * w + x];
            }
            // Buffers flooring column in GPU memory
            struct point caches[yres / 2];
            const int sz = fbc - ftc;
            for(int i = 0, row = ftc; row < fbc; i++, row++)
            {
                const double dis = precalc_distances[row];
                const double percent = dis / normal;
                const struct point rayf = geom_mul(rayw, percent);
                const struct point floor = geom_add(hero, rayf);
                caches[i] = floor;
                const int tt = geom_getflooringtile(floor);
                const SDL_Surface* const flooring = tiles[tt];
                const int ww = flooring->w;
                const int hh = flooring->h;
                const int xx = ww * geom_mod(floor.x);
                const int yy = hh * geom_mod(floor.y);
                const uint32_t* const pixels = flooring->pixels;
                screen[row * xres + col] = pixels[yy * ww + xx];
            }
            // Buffers ceiling column in GPU memory
            for(int i = 0, row = ctc; row < cbc; i++, row++)
            {
                const struct point ceil = caches[sz - i - 1];
                const int tt = geom_getceilingtile(ceil);
                const SDL_Surface* const ceiling = tiles[tt];
                const int ww = ceiling->w;
                const int hh = ceiling->h;
                const int xx = ww * geom_mod(ceil.x);
                const int yy = hh * geom_mod(ceil.y);
                const uint32_t* const pixels = ceiling->pixels;
                screen[row * xres + col] = tt ? pixels[yy * ww + xx] : 0x0;
            }
        }
        // Releases the GPU
        SDL_UnlockTexture(gpu);
        // Renders all buffered columns in GPU to screen
        SDL_RenderCopy(renderer, gpu, NULL, NULL);
        SDL_RenderPresent(renderer);
        // FPS correcting delay
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int fps = 60;
        const int delay = 1000 / fps - dt;
        SDL_Delay(delay < 0 ? 0 : delay);
    }
    // Frees the heap
    #define len(array) (int)(sizeof(array) / sizeof(*array))
    for(int i = 0; i < len(tiles); i++) SDL_FreeSurface(tiles[i]);
    for(int i = 0; i < len(sprts); i++) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
    map_unload();
    precalc_unload();
    return 0;
}
