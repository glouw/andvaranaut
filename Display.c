#include "Display.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <assert.h>

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
    SDL_Surface* const cvt = SDL_ConvertSurface(bmp, allocation, 0);
    SDL_FreeFormat(allocation);
    return cvt;
}

// Darkens a pixel by some amount and clamps
// Discards the alpha
static inline uint32_t
Darken(const uint32_t pixel, const int amount)
{
    const int r = 0xFF & (pixel >> 16);
    const int g = 0xFF & (pixel >> 8);
    const int b = 0xFF & pixel;
    // Modified
    const int rm = r - amount;
    const int gm = g - amount;
    const int bm = b - amount;
    // Clamped
    const int rc = rm < 0 ? 0 : rm;
    const int gc = gm < 0 ? 0 : gm;
    const int bc = bm < 0 ? 0 : bm;
    // Pixel reconstruction
    return rc << 16 | gc << 8 | bc;
}

// Preoptimizs a bunch of static row and column calculations
static double* diss; // Distances
static double* sigs; // Sigmas
static void
PreOptimize(void)
{
    // Rows
    diss = malloc(yres * sizeof(double));
    for(int row = 0; row < yres; row++)
    {
        diss[row] = focal * yres / (2 * (row + 1) - yres);
    }
    // Cols
    sigs = malloc(xres * sizeof(double));
    for(int col = 0; col < xres; col++)
    {
        const double pan = 2.0 * (double)col / xres - 1.0;
        sigs[col] = atan2(pan, focal);
    }
}

// Renders one screen column based on hero position
static void
RenderColumn(const Hero hero, const Map map, const int col, uint32_t* const screen)
{
    /* Wall Ray Casting */
    const double sigma = sigs[col];
    const double radians = sigma + hero.theta;
    const Point wall = Point_Cast(hero.where, radians, map.walling);
    // Renders an artifact column if the ray left the map
    const Point wray = Point_Sub(wall, hero.where);
    // Ray fish eye correction
    const double wmag = Point_Magnitude(wray);
    const double wnormal = wmag * cos(sigma);
    // Wall height calculation
    const double wheight = round(focal * (double)yres / wnormal);
    const double wt = (double)yres / 2.0 - wheight / 2.0;
    const double wb = wt + wheight;
    // Wall top clamped
    const int wtc = wt < 0.0 ? 0 : (int)wt;
    // Wall bottom clamped
    const int wbc = wb > (double)yres ? yres : (int)wb;
    // Wall tile inspection
    const int wtile = Point_TileEnclosure(wall, map.walling);
    // Wall tile BMP texture
    const SDL_Surface* const walling = tiles[wtile]; assert(walling);
    const int ww = walling->w;
    const int wh = walling->h;
    const int wx = ww * Point_Percent(wall, map.walling);
    // Wall darkening
    const int wd = wmag * wmag / hero.draw;
    // GPU buffering
    const uint32_t* const wpixels = walling->pixels;
    for(int row = wtc; row < wbc; row++)
    {
        const int wy = wh * (row - wt) / wheight;
        const uint32_t pixel = wpixels[wy * ww + wx];
        screen[row * xres + col] = Darken(pixel, wd);
    }
    // Party bottom clamped
    const int pbc = yres;
    // Party height
    const int pheight = yres - wbc;
    // Party cache
    Point parts[yres];
    // Party darkenings
    int pds[yres];
    /* Party Ray Casting */
    for(int i = 0, row = wbc; row < pbc; i++, row++)
    {
        const double percent = diss[row] / wnormal;
        const Point pray = Point_Mul(wray, percent);
        const Point part = Point_Add(hero.where, pray);
        parts[i] = part;
        // Party darkening
        const double pmag = Point_Magnitude(pray);
        // Party darkening clamp
        pds[i] = pmag * pmag / hero.draw;
    }
    // Floor bottom clamped
    const int fbc = pbc;
    /* Floor tile mapping */
    for(int i = 0, row = wbc; row < fbc; i++, row++)
    {
        const int index = i;
        const Point flor = parts[index];
        // Floor tile inspection
        const int ftile = Point_TileParty(flor, map.floring);
        // Floor tile BMP texture
        const SDL_Surface* const floring = tiles[ftile]; assert(floring);
        const int fw = floring->w;
        const int fh = floring->h;
        const int fx = fw * Point_Decimal(flor.x);
        const int fy = fh * Point_Decimal(flor.y);
        // GPU buffering
        const int fd = pds[index];
        const uint32_t* const pixels = floring->pixels;
        const uint32_t pixel = pixels[fy * fw + fx];
        screen[row * xres + col] = Darken(pixel, fd);
    }
    // Ceiling top clamped
    const int ctc = 0;
    /* Ceiling tile mapping */
    for(int i = 0, row = ctc; row < wtc; i++, row++)
    {
        const int index = pheight - i - 1;
        const Point ceil = parts[index];
        // Ceiling tile inspection
        const int ctile = Point_TileParty(ceil, map.ceiling);
        // Ceiling tile BMP texture
        const SDL_Surface* const ceiling = tiles[ctile]; assert(ceiling);
        const int cw = ceiling->w;
        const int ch = ceiling->h;
        const int cx = cw * Point_Decimal(ceil.x);
        const int cy = ch * Point_Decimal(ceil.y);
        // GPU buffering
        const int cd = pds[index];
        const uint32_t* const pixels = ceiling->pixels;
        const uint32_t pixel = pixels[cy * cw + cx];
        screen[row * xres + col] = Darken(pixel, cd);
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
    PreOptimize();
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
    // Acquires GPU
    gpu = SDL_CreateTexture(renderer, format, access, xres, yres); assert(gpu);
}

// Cleans up SDL
void
Display_Shutdown()
{
    // SDL
    for(int i = 0; i < SURFACES; i++) SDL_FreeSurface(tiles[i]);
    for(int i = 0; i < SURFACES; i++) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
}
