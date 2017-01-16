#include "Display.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <assert.h>

// Precalculations
static double* distances;
static double* sigmas;
// Constant definitions and such
static const double focal = 1.0;
static const int xres = 600;
static const int yres = 300;
static const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
static const uint32_t mode = SDL_RENDERER_ACCELERATED;
static const uint32_t estate = SDL_WINDOW_SHOWN;
static const uint32_t access = SDL_TEXTUREACCESS_STREAMING;
// SDL related stuff
static SDL_Window* window;
static SDL_Texture* gpu;
static SDL_Renderer* renderer;
#define SURFACES 10
static SDL_Surface* tiles[SURFACES];
static SDL_Surface* sprts[SURFACES];

// Loads a BMP based on the pixel format and returns a surface tile
static SDL_Surface*
LoadBMP(const char* const path)
{
    SDL_Surface* const bmp = SDL_LoadBMP(path); assert(bmp);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format); assert(allocation);
    return SDL_ConvertSurface(bmp, allocation, 0);
}

// Optimizes row and column calculations
static void
Optimize()
{
    // Rows
    distances = malloc(sizeof(double) * yres); assert(distances);
    for(int row = 0; row < yres; row++)
        distances[row] = focal * yres / (2 * (row + 1) - yres);
    // Columns
    sigmas = malloc(sizeof(double) * xres); assert(sigmas);
    for(int col = 0; col < xres; col++)
    {
        const double pan = 2.0 * (double)col / xres - 1.0;
        sigmas[col] = atan2(pan, focal);
    }
}

// Darkens a pixel by some amount and clamps
static inline uint32_t
Darken(const uint32_t pixel, const unsigned amount)
{
    const uint8_t r = pixel >> 16;
    const uint8_t g = pixel >> 8;
    const uint8_t b = pixel;
    // Modified
    const int rm = r - amount;
    const int gm = g - amount;
    const int bm = b - amount;
    // Clamped
    const uint8_t rc = rm < 0 ? 0 : rm;
    const uint8_t gc = gm < 0 ? 0 : gm;
    const uint8_t bc = bm < 0 ? 0 : bm;
    return (pixel & 0xFF000000) | rc << 16 | gc << 8 | bc;
}

// Renders one screen column based on hero position
static void
RenderColumn(const Hero hero, const Map map, const int col, uint32_t* const screen)
{
    /* Wall Ray Casting */
    const double radians = sigmas[col] + hero.theta;
    const Point wall = Point_Cast(hero.where, radians, map.walling);
    // Renders an artifact column if the ray left the map
    const Point wray = Point_Sub(wall, hero.where);
    // Ray fish eye correction
    const double wmag = Point_Magnitude(wray);
    const double wnormal = wmag * cos(sigmas[col]);
    // Wall height calculation
    const double wheight = round(focal * (double)yres / wnormal);
    const double wt = (double)yres / 2.0 - wheight / 2.0;
    const double wb = wt + wheight;
    // Wall top clamped
    const int wtc = wt < 0.0 ? 0 : (int)wt;
    // Wall bottom clamped
    const int wbc = wb > (double)yres ? yres : (int)wb;
    // Wall tile inspection
    const int wtile = Point_Tile(wall, map.walling, true);
    // Wall tile BMP texture
    const SDL_Surface* const walling = tiles[wtile]; assert(walling);
    const int ww = walling->w;
    const int wh = walling->h;
    const int wx = ww * Point_Percent(wall, map.walling);
    // Wall darkening
    const unsigned wd = wmag * wmag * hero.torch;
    // Wall darkening clamped
    const unsigned wdc = wd > 0xFF ? 0xFF: wd;
    // GPU buffering
    for(int row = wtc; row < wbc; row++)
    {
        const uint32_t* const pixels = walling->pixels;
        const int wy = wh * (row - wt) / wheight;
        const uint32_t pixel = pixels[wy * ww + wx];
        screen[row * xres + col] = Darken(pixel, wdc);
    }
    // Party bottom clamped
    const int pbc = yres;
    // Party height
    const int pheight = yres - wbc;
    // Party cache
    Point parts[yres];
    // Darkening clamps
    unsigned dcs[yres];
    /* Party Ray Casting */
    for(int i = 0, row = wbc; row < pbc; i++, row++)
    {
        const double dis = distances[row];
        const double percent = dis / wnormal;
        const Point pray = Point_Mul(wray, percent);
        const Point part = Point_Add(hero.where, pray);
        parts[i] = part;
        // Party darkening
        const double pmag = Point_Magnitude(pray);
        const unsigned pd = pmag * pmag * hero.torch;
        // Party darkening clamped
        const unsigned pdc = pd > 0xFF ? 0xFF: pd;
        dcs[i] = pdc;
    }
    // Floor bottom clamped
    const int fbc = pbc;
    /* Floor tile mapping */
    for(int i = 0, row = wbc; row < fbc; i++, row++)
    {
        const int index = i;
        const Point flor = parts[index];
        // Floor tile inspection
        const int ftile = Point_Tile(flor, map.floring, false);
        // Floor tile BMP texture
        const SDL_Surface* const floring = tiles[ftile]; assert(floring);
        const int fw = floring->w;
        const int fh = floring->h;
        const int fx = fw * Point_Decimal(flor.x);
        const int fy = fh * Point_Decimal(flor.y);
        // GPU buffering
        const unsigned fdc = dcs[index];
        const uint32_t* const pixels = floring->pixels;
        const uint32_t pixel = pixels[fy * fw + fx];
        screen[row * xres + col] = Darken(pixel, fdc);
    }
    // Ceiling top clamped
    const int ctc = 0;
    /* Ceiling tile mapping */
    for(int i = 0, row = ctc; row < wtc; i++, row++)
    {
        const int index = pheight - i - 1;
        const Point ceil = parts[index];
        // Ceiling tile inspection
        const int ctile = Point_Tile(ceil, map.ceiling, false);
        // Ceiling tile BMP texture
        const SDL_Surface* const ceiling = tiles[ctile]; assert(ceiling);
        const int cw = ceiling->w;
        const int ch = ceiling->h;
        const int cx = cw * Point_Decimal(ceil.x);
        const int cy = ch * Point_Decimal(ceil.y);
        // GPU buffering
        const unsigned cdc = dcs[index];
        const uint32_t* const pixels = ceiling->pixels;
        const uint32_t pixel = pixels[cy * cw + cx];
        screen[row * xres + col] = Darken(pixel, cdc);
    }
}

// Renders all screem columns based on hero position
void
Display_RenderFrame(const Hero hero, const Map map)
{
    // GPU readying
    void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
    uint32_t* const screen = (uint32_t*)bytes;
    // For all screen columns
    for(int col = 0; col < xres; col++) RenderColumn(hero, map, col, screen);
    // GPU release
    SDL_UnlockTexture(gpu);
    // GPU screen update
    SDL_RenderCopy(renderer, gpu, NULL, NULL);
}

// Displays a string
void
Display_String(const char* string, const int x, const int y)
{
    // TTF font loading and surface to texture conversion
    TTF_Font* const font = TTF_OpenFont("fonts/deja.ttf", 18); assert(font);
    const SDL_Color color = { 0xFF, 0xFF, 0x00, 0 };
    SDL_Surface* const blended = TTF_RenderText_Blended(font, string, color); assert(blended);
    SDL_Texture* const message = SDL_CreateTextureFromSurface(renderer, blended); assert(message);
    int width, height;
    TTF_SizeText(font, string, &width, &height);
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    SDL_Rect const rect = { x, y, width, height };
    SDL_RenderCopy(renderer, message, NULL, &rect);
    // Cleanup
    SDL_FreeSurface(blended);
    SDL_DestroyTexture(message);
    TTF_CloseFont(font);
}

// Shows FPS in the upper left corner of the screen
void
Display_ShowFPS(const int fps, const int x, const int y)
{
    #define MAX 10
    char string[MAX];
    snprintf(string, MAX, "%d", fps);
    Display_String(string, x, y);
    #undef MAX
}

// Updates display with render
void
Display_Update()
{
    SDL_RenderPresent(renderer);
}

// Boots up SDL
void
Display_Boot()
{
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("water", 0, 0, xres, yres, estate); assert(window);
    renderer = SDL_CreateRenderer(window, -1, mode); assert(renderer);
    // Loads textures and sprites
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
    // Acquires GPU and does some preliminary optimization calculations
    gpu = SDL_CreateTexture(renderer, format, access, xres, yres); assert(gpu);
    Optimize();
}

// Cleans up optimizations and SDL
void
Display_Shutdown()
{
    // Optimizations
    free(distances);
    free(sigmas);
    // SDL
    for(int i = 0; i < SURFACES; i++) SDL_FreeSurface(tiles[i]);
    for(int i = 0; i < SURFACES; i++) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
}
