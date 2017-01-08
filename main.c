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
main(const int argc, const char* const argv[])
{
    // Command line: <bin map>
    if(argc != 2) return 1;
    const int xres = 800;
    const int yres = 400;
    // Resolution based calculation optimizations
    precalc_optimize(xres, yres);
    // Loads map
    const char* map = argv[1];
    map_load(map);
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
    // Readys GPU
    const uint32_t access = SDL_TEXTUREACCESS_STREAMING;
    SDL_Texture* const gpu = SDL_CreateTexture(renderer, format, access, xres, yres);
    // Creates hero
    struct point hero = { 2.5, 2.5 }; double theta = 0.0;
    const double d0 = 0.080;
    const double dy = 0.100;
    const double dx = 0.100;
    // Loops game until <CTRL+D> is pressed
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
        // Detects hero collisions with wall
        hero = geom_collision(temp) ? hero : temp;
        // Acquires GPU
        void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
        uint32_t* const screen = (uint32_t*)bytes;
        // Casts a ray for each column of screen
        for(int col = 0; col < xres; col++)
        {
            const double radians = precalc_sigmas[col] + theta;
            const struct point wall = geom_cast(hero, radians, map_wallings);
            if(geom_out(wall)) continue;
            const struct point wray = geom_sub(wall, hero);
            // Corrects fish eye
            const double wnormal = geom_mag(wray) * cos(precalc_sigmas[col]);
            // Calculates wall height
            const double wheight = round(precalc_focal * yres / wnormal);
            const double wt = round((yres / 2.0) - (wheight / 2.0));
            const double wb = wt + wheight;
            // Clamps wall top to the screen
            const int wtc = wt < 0.0 ? 0 : (int)wt;
            // Clamps the wall bottom to the screen
            const int wbc = wb > (double)yres ? yres : (int)wb;
            // Buffers walling column in GPU memory
            const int wtile = geom_etile(wall, map_wallings);
            const SDL_Surface* const walling = tiles[wtile];
            const int ww = walling->w;
            const int wh = walling->h;
            const int wx = ww * geom_epercent(wall, map_wallings);
            for(int row = wtc; row < wbc; row++)
            {
                const uint32_t* const pixels = walling->pixels;
                const int wy = wh * (row - wt) / wheight;
                screen[row * xres + col] = pixels[wy * ww + wx];
            }
            // Clamps floor bottom to screen
            const int fbc = yres;
            // Buffers flooring column in GPU memory
            struct point caches[yres / 2];
            const int sz = fbc - wbc;
            for(int i = 0, row = wbc; row < fbc; i++, row++)
            {
                const double dis = precalc_distances[row];
                const double percent = dis / wnormal;
                const struct point fray = geom_mul(wray, percent);
                const struct point floor = geom_add(hero, fray);
                caches[i] = floor;
                const int ftile = geom_ftile(floor);
                const SDL_Surface* const flooring = tiles[ftile];
                const int fw = flooring->w;
                const int fh = flooring->h;
                const int fx = fw * geom_mod(floor.x);
                const int fy = fh * geom_mod(floor.y);
                const uint32_t* const pixels = flooring->pixels;
                screen[row * xres + col] = pixels[fy * fw + fx];
            }
            if(map_inside)
            {
                // Clamps ceiling top to screen
                const int ctc = 0;
                // Buffers ceiling column in GPU memory
                for(int i = 0, row = ctc; row < wtc; i++, row++)
                {
                    const struct point ceil = caches[sz - i - 1];
                    const int ctile = geom_ctile(ceil);
                    const SDL_Surface* const ceiling = tiles[ctile];
                    const int cw = ceiling->w;
                    const int ch = ceiling->h;
                    const int cx = cw * geom_mod(ceil.x);
                    const int cy = ch * geom_mod(ceil.y);
                    const uint32_t* const pixels = ceiling->pixels;
                    screen[row * xres + col] = pixels[cy * cw + cx];
                }
            }
            if(map_outside)
            {
                // Buffers roof column in GPU memory
                const struct point roof = geom_cast(hero, radians, map_roofings);
                if(geom_out(roof)) continue;
                const struct point rray = geom_sub(roof, hero);
                // Corrects fish eye
                const double rnormal = geom_mag(rray) * cos(precalc_sigmas[col]);
                // Calculates roof height
                const double rheight = round(precalc_focal * yres / rnormal);
                const double rt = round((yres / 2.0) - (rheight / 2.0));
                const double rb = rt + rheight;
                // Shifts the roof up
                const double rts = rt - rheight;
                const double rbs = rb - rheight;
                // Clamps the roof top to the top of the screen
                const int rtsc = rts < 0.0 ? 0 : (int)rts;
                // Clamps the roof bottom to the top of the screen
                const int rbsc = rbs < 0.0 ? 0 : (int)rbs;
                // Buffers roofing column in GPU memory
                const int rtile = geom_etile(roof, map_roofings);
                const SDL_Surface* const roofing = tiles[rtile];
                const int rw = roofing->w;
                const int rh = roofing->h;
                const int rx = rw * geom_epercent(roof, map_roofings);
                for(int row = rtsc; row < rbsc; row++)
                {
                    const uint32_t* const pixels = roofing->pixels;
                    const int ry = rh * (row - rts) / rheight;
                    screen[row * xres + col] = pixels[ry * rw + rx];
                }
                // Clamps sky top to screen
                const int stc = 0;
                // Clamps sky bottom to roof top shift
                const int sbc = rtsc;
                // Buffers sky column in GPU memory
                for(int row = stc; row < sbc; row++)
                {
                    screen[row * xres + col] = 0x0000000;
                }
            }
        }
        // Releases GPU
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
    // Frees heap
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
