#include "Display.h"

#include "Map.h"
#include "Precalc.h"

#include "SDL2/SDL.h"

const double Display_focal = 1.0;
const int Display_xres = 800;
const int Display_yres = 400;

static const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
static const uint32_t mode = SDL_RENDERER_ACCELERATED;
static const uint32_t estate = SDL_WINDOW_SHOWN;
static const uint32_t access = SDL_TEXTUREACCESS_STREAMING;
static SDL_Window* window;
static SDL_Texture* gpu;
static SDL_Surface* tiles[10];
static SDL_Surface* sprts[10];
static SDL_Renderer* renderer;

static SDL_Surface*
LoadBMP(const char* const path)
{
    SDL_Surface* const surface = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const convert = SDL_ConvertSurface(surface, allocation, 0);
    return convert;
}

static void
RenderColumn(const Hero hero, const int col, uint32_t* const screen)
{
    /* Wall Ray Casting */
    const double radians = Precalc_sigmas[col] + hero.theta;
    const struct point wall = Geom_cast(hero.where, radians, Map_walling);
    if(Geom_out(wall)) return;
    const struct point wray = Geom_sub(wall, hero.where);
    // Ray fish eye correction
    const double wnormal = Geom_mag(wray) * cos(Precalc_sigmas[col]);
    // Wall height calculation
    const double wheight = round(Display_focal * (double)Display_yres / wnormal);
    const double wt = (double)Display_yres / 2.0 - wheight / 2.0;
    const double wb = wt + wheight;
    // Wall top clamping
    const int wtc = wt < 0.0 ? 0 : (int)wt;
    // Wall bottom clamping
    const int wbc = wb > (double)Display_yres ? Display_yres : (int)wb;
    // Wall tile inspection
    const int wtile = Geom_etile(wall, Map_walling);
    // Wall tile BMP texture
    const SDL_Surface* const walling = tiles[wtile];
    const int ww = walling->w;
    const int wh = walling->h;
    const int wx = ww * Geom_epercent(wall, Map_walling);
    // GPU buffering
    for(int row = wtc; row < wbc; row++)
    {
        const uint32_t* const pixels = walling->pixels;
        const int wy = wh * (row - wt) / wheight;
        screen[row * Display_xres + col] = pixels[wy * ww + wx];
    }
    /* Floor Ray Casting */
    struct point caches[Display_yres / 2];
    // Floor bottom clamping
    const int fbc = Display_yres;
    // Floor height calculation
    const int fheight = fbc - wbc;
    for(int i = 0, row = wbc; row < fbc; i++, row++)
    {
        const double dis = Precalc_distances[row];
        const double percent = dis / wnormal;
        const struct point fray = Geom_mul(wray, percent);
        const struct point floor = Geom_add(hero.where, fray);
        // Caches floor calculations for ceiling use
        caches[i] = floor;
        // Floor tile inspection
        const int ftile = Geom_ftile(floor);
        // Floor tile BMP texture
        const SDL_Surface* const flooring = tiles[ftile];
        const int fw = flooring->w;
        const int fh = flooring->h;
        const int fx = fw * Geom_mod(floor.x);
        const int fy = fh * Geom_mod(floor.y);
        // GPU buffering
        const uint32_t* const pixels = flooring->pixels;
        screen[row * Display_xres + col] = pixels[fy * fw + fx];
    }
    // Ceiling top clamp
    const int ctc = 0;
    /* Ceiling Ray Casting */
    for(int i = 0, row = ctc; row < wtc; i++, row++)
    {
        // Gets ceiling cache
        const struct point ceil = caches[fheight - i - 1];
        // Ceiling tile inspection
        const int ctile = Geom_ctile(ceil);
        // Ceiling tile BMP texture
        const SDL_Surface* const ceiling = tiles[ctile];
        const int cw = ceiling->w;
        const int ch = ceiling->h;
        const int cx = cw * Geom_mod(ceil.x);
        const int cy = ch * Geom_mod(ceil.y);
        // GPU buffering
        const uint32_t* const pixels = ceiling->pixels;
        screen[row * Display_xres + col] = pixels[cy * cw + cx];
    }
}

void
Display_Shutdown()
{
    #define len(array) (int)(sizeof(array) / sizeof(*array))
    for(int i = 0; i < len(tiles); i++) SDL_FreeSurface(tiles[i]);
    for(int i = 0; i < len(sprts); i++) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
}

void
Display_RenderFrame(const Hero hero)
{
    // GPU readying
    void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
    uint32_t* const screen = (uint32_t*)bytes;
    // For all columns of the screen
    for(int col = 0; col < Display_xres; col++) RenderColumn(hero, col, screen);
    // GPU release
    SDL_UnlockTexture(gpu);
    // GPU screen update
    SDL_RenderCopy(renderer, gpu, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void
Display_Boot()
{
    // Display initialization
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("water", 25, 120, Display_xres, Display_yres, estate);
    renderer = SDL_CreateRenderer(window, -1, mode);
    // Texture and sprite loading
    #define T(n, tile) tiles[n] = LoadBMP("tiles/"tile);
    #define LD_TILES                                   \
        T(0, "error.bmp")                              \
        T(1, "stone.bmp")                              \
        T(2, "wood.bmp")
    #define S(n, sprt) sprts[n] = LoadBMP("sprts/"sprt);
    #define LD_SPRTS                                   \
        S(0, "soldier.bmp")
    LD_TILES
    LD_SPRTS
    #undef T
    #undef S
    #undef LD_TILES
    #undef LD_SPRTS
    // GPU acquisition
    gpu = SDL_CreateTexture(renderer, format, access, Display_xres, Display_yres);
}

