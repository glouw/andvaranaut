// Copyright (C) 2016-2017 Gustav Louw
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Display.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <assert.h>

// Constant definitions and such
static const float focal = 1.f;
static const int xres = 600;
static const int yres = 300;
static const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
static const uint32_t mode = SDL_RENDERER_ACCELERATED;
static const uint32_t estate = SDL_WINDOW_SHOWN;
static const uint32_t access = SDL_TEXTUREACCESS_STREAMING;
// SDL related stuff
static SDL_Window* window;
static SDL_Texture* wcf;
static SDL_Renderer* renderer;
static SDL_Surface* tiles[10];
static SDL_Surface* sprts[10];

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

// Preoptimizs a bunch of static row and column calculations
static float* diss; // Distances
static float* sigs; // Sigmas
#define CLRS 256
#define MODS 256
static uint8_t clut[CLRS][MODS];
static void
PreOptimize()
{
    // Rows
    diss = malloc(yres * sizeof(float));
    for(int row = 0; row < yres; ++row)
        diss[row] = focal * yres / (2 * (row + 1) - yres);
    // Cols
    sigs = malloc(xres * sizeof(float));
    for(int col = 0; col < xres; ++col)
    {
        const float pan = 2.f * (float)col / xres - 1.f;
        sigs[col] = atan2f(pan, focal);
    }
    // Color Look up table
    for(int i = 0xFF; i >= 0x00; i--)
    for(int j = 0xFF; j >= 0x00; j--)
        clut[i][j] = (i * j) / 0xFF;
}

// Mods a pixel by some percentage
// Discards the alpha
static inline uint32_t
FlatMod(const uint32_t pixel, const int mod)
{
    const int r = 0xFF & (pixel >> 16);
    const int g = 0xFF & (pixel >>  8);
    const int b = 0xFF & (pixel >>  0);
    // Modified
    const int rm = clut[r][mod];
    const int gm = clut[g][mod];
    const int bm = clut[b][mod];
    // Pixel reconstruction
    return rm << 16 | gm << 8 | bm;
}

// Clamps a value between min and max
static inline float
clamp(const float value, const float min, const float max)
{
    const float t = value < min ? min : value;
    return t > max ? max : t;
}

// Renders one screen column based on hero position
static void
RenderColumn(const Hero hero, const Map map, const int col, uint32_t* const screen)
{
    /* Wall Ray Casting */
    const float sigma = sigs[col];
    const float radians = sigma + hero.theta;
    const Point wall = Point_Cast(hero.where, radians, map.walling);
    // Renders an artifact column if the ray left the map
    const Point wray = Point_Sub(wall, hero.where);
    // Ray fish eye correction
    const float wmag = Point_Magnitude(wray);
    const float wnormal = wmag * cosf(sigma);
    // Wall height calculation
    const float wheight = round(focal * (float)yres / wnormal);
    const float wt = (float)yres / 2.f - wheight / 2.f;
    const float wb = wt + wheight;
    // Wall top clamped
    const int wtc = wt < 0.f ? 0 : (int)wt;
    // Wall bottom clamped
    const int wbc = wb > (float)yres ? yres : (int)wb;
    // Wall tile inspection
    const int wtile = Point_TileEnclosure(wall, map.walling);
    // Wall tile BMP texture
    const SDL_Surface* const walling = tiles[wtile]; assert(walling);
    const int ww = walling->w;
    const int wh = walling->h;
    const int wx = ww * Point_Percent(wall, map.walling);
    // Wall mod
    const float wm = hero.torch / (wmag * wmag);
    // Wall mod clamped
    const int wmc = (float)0xFF * clamp(wm, 0.f, 1.f);
    // Wall buffering
    const uint32_t* const wpixels = walling->pixels;
    for(int row = wtc; row < wbc; ++row)
    {
        const int wy = wh * (row - wt) / wheight;
        const uint32_t pixel = wpixels[wy * ww + wx];
        screen[row * xres + col] = FlatMod(pixel, wmc);
    }
    // Party bottom clamped
    const int pbc = yres;
    // Party height
    const int pheight = yres - wbc;
    // Party cache
    Point parts[yres];
    // Party mod clamps
    int pmcs[yres];
    /* Party Ray Casting */
    for(int i = 0, row = wbc; row < pbc; ++i, ++row)
    {
        const float percent = diss[row] / wnormal;
        const Point pray = Point_Mul(wray, percent);
        const Point part = Point_Add(hero.where, pray);
        parts[i] = part;
        const float pmag = Point_Magnitude(pray);
        // Party mod
        const float pm = hero.torch / (pmag * pmag);
        // Party mod clamped
        const int pmc = (float)0xFF * clamp(pm, 0.f, 1.f); // Party mod clamps
        pmcs[i] = pmc;
    }
    // Floor bottom clamped
    const int fbc = pbc;
    /* Floor tile mapping */
    for(int i = 0, row = wbc; row < fbc; ++i, ++row)
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
        // Floor buffering
        const int fm = pmcs[index];
        const uint32_t* const pixels = floring->pixels;
        const uint32_t pixel = pixels[fy * fw + fx];
        screen[row * xres + col] = FlatMod(pixel, fm);
    }
    // Ceiling top clamped
    const int ctc = 0;
    /* Ceiling tile mapping */
    for(int i = 0, row = ctc; row < wtc; ++i, ++row)
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
        // Ceiling buffering
        const int cm = pmcs[index];
        const uint32_t* const pixels = ceiling->pixels;
        const uint32_t pixel = pixels[cy * cw + cx];
        screen[row * xres + col] = FlatMod(pixel, cm);
    }
}

// Renders sprites based on player location in map
static void
RenderS(const Hero hero, const Map map)
{
    // Currently not in use
    (void)map;
    // Hence the override
    const Point sprite = { 24, 7.5f };
    const Point sray = Point_Sub(sprite, hero.where);
    // Sprite magniude
    const float smag = Point_Magnitude(sray);
    // Which sprite surface?
    SDL_Surface* const sprt = sprts[0];
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sprt);
    // Sprite mod
    const float sm = hero.torch / (smag * smag);
    // Sprite mod clamped
    const int smc = (float)0xFF * clamp(sm, 0.f, 1.f);
    // Darkening mod
    SDL_SetTextureColorMod(texture, smc, smc, smc);
    // Percieved width and height
    const float psw = yres / smag, psh = psw;
    // Draw
    SDL_Rect dest = {
        // Sprite x
        xres / 2 - psw / 2,
        // Sprite y
        yres / 2 - psh / 2,
        // Percieved size
        psw, psh
    };
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

// Renders the wall, ceiling, and floor based on player location in map
static void
RenderWCF(const Hero hero, const Map map)
{
    void* bytes; int null; SDL_LockTexture(wcf, NULL, &bytes, &null);
    uint32_t* const screen = (uint32_t*)bytes;
    // Renders all columns and returns player to wall magnitudes for each column
    for(int col = 0; col < xres; ++col) RenderColumn(hero, map, col, screen);
    // Screen update
    SDL_UnlockTexture(wcf);
    SDL_RenderCopy(renderer, wcf, NULL, NULL);
}

// Renders entire frame based on hero position
void
Display_RenderFrame(const Hero hero, const Map map)
{
    RenderWCF(hero, map);
    //RenderS(hero, map);
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

// Shows FPS at screen coordinates
void
Display_FPS(const int fps, const int x, const int y)
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
    // Acquires wcf
    wcf = SDL_CreateTexture(renderer, format, access, xres, yres); assert(wcf);
}

// Cleans up SDL and friends
void
Display_Shutdown()
{
    // SDL
    for(int i = 0; tiles[i]; ++i) SDL_FreeSurface(tiles[i]);
    for(int i = 0; sprts[i]; ++i) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(wcf);
    SDL_Quit();
    // Optimizations
    free(sigs);
    free(diss);
}
