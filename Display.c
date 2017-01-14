#include "Display.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Precalculations
static double* distances;
static double* sigmas;
// Constant definitions and such
static const double focal = 1.0;
static const int xres = 800;
static const int yres = 400;
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
    SDL_Surface* const surface = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const convert = SDL_ConvertSurface(surface, allocation, 0);
    return convert;
}

// Optimizes row and column calculations
static void
Optimize()
{
    distances = malloc(sizeof(double) * yres);
    for(int row = 0; row < yres; row++)
        distances[row] = focal * yres / (2 * (row + 1) - yres);
    sigmas = malloc(sizeof(double) * xres);
    for(int col = 0; col < xres; col++)
    {
        const double pan = 2.0 * (double)col / xres - 1.0;
        sigmas[col] = atan2(pan, focal);
    }
}

// Renders one screen column based on hero position
static void
RenderColumn(const Hero hero, const int col, uint32_t* const screen, const Map map)
{
    /* Wall Ray Casting */
    const double radians = sigmas[col] + hero.theta;
    const Point wall = Map_Cast(hero.where, radians, map);
    // Renders an artifact column if the ray left the map
    if(Map_Out(wall, map)) return;
    const Point wray = Point_Sub(wall, hero.where);
    // Ray fish eye correction
    const double wnormal = Point_Magnitude(wray) * cos(sigmas[col]);
    // Wall height calculation
    const double wheight = round(focal * (double)yres / wnormal);
    const double wt = (double)yres / 2.0 - wheight / 2.0;
    const double wb = wt + wheight;
    // Wall top clamping
    const int wtc = wt < 0.0 ? 0 : (int)wt;
    // Wall bottom clamping
    const int wbc = wb > (double)yres ? yres : (int)wb;
    // Wall tile inspection
    const int wtile = Point_Tile(wall, map.walling, true);
    // Wall tile BMP texture
    const SDL_Surface* const walling = tiles[wtile];
    const int ww = walling->w;
    const int wh = walling->h;
    const int wx = ww * Point_Percent(wall, map.walling);
    // GPU buffering
    for(int row = wtc; row < wbc; row++)
    {
        const uint32_t* const pixels = walling->pixels;
        const int wy = wh * (row - wt) / wheight;
        screen[row * xres + col] = pixels[wy * ww + wx];
    }
    /* Floor Ray Casting */
    Point caches[yres / 2];
    // Floor bottom clamping
    const int fbc = yres;
    // Floor height calculation
    const int fheight = fbc - wbc;
    for(int i = 0, row = wbc; row < fbc; i++, row++)
    {
        const double dis = distances[row];
        const double percent = dis / wnormal;
        const Point fray = Point_Mul(wray, percent);
        const Point flor = Point_Add(hero.where, fray);
        // Caches floor calculations for ceiling use
        caches[i] = flor;
        // Floor tile inspection
        const int ftile = Point_Tile(flor, map.floring, false);
        // Floor tile BMP texture
        const SDL_Surface* const floring = tiles[ftile];
        const int fw = floring->w;
        const int fh = floring->h;
        const int fx = fw * Point_Decimal(flor.x);
        const int fy = fh * Point_Decimal(flor.y);
        // GPU buffering
        const uint32_t* const pixels = floring->pixels;
        screen[row * xres + col] = pixels[fy * fw + fx];
    }
    // Ceiling top clamp
    const int ctc = 0;
    /* Ceiling Ray Casting */
    for(int i = 0, row = ctc; row < wtc; i++, row++)
    {
        // Gets ceiling cache
        const Point ceil = caches[fheight - i - 1];
        // Ceiling tile inspection
        const int ctile = Point_Tile(ceil, map.ceiling, false);
        // Ceiling tile BMP texture
        const SDL_Surface* const ceiling = tiles[ctile];
        const int cw = ceiling->w;
        const int ch = ceiling->h;
        const int cx = cw * Point_Decimal(ceil.x);
        const int cy = ch * Point_Decimal(ceil.y);
        // GPU buffering
        const uint32_t* const pixels = ceiling->pixels;
        screen[row * xres + col] = pixels[cy * cw + cx];
    }
}

// Renders all screem columns based on hero position
void
Display_RenderFrame(const Hero hero, const Map map)
{
    // GPU readying
    void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
    uint32_t* const screen = (uint32_t*)bytes;
    // For all columns of the screen
    for(int col = 0; col < xres; col++) RenderColumn(hero, col, screen, map);
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
    TTF_Font* const font = TTF_OpenFont("fonts/deja.ttf", 18);
    const SDL_Color color = { 0xFF, 0xFF, 0x00, 0 };
    SDL_Surface* const surface = TTF_RenderText_Blended(font, string, color);
    SDL_Texture* const message = SDL_CreateTextureFromSurface(renderer, surface);
    int width, height;
    TTF_SizeText(font, string, &width, &height);
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    SDL_Rect const rect = { x, y, width, height };
    SDL_RenderCopy(renderer, message, NULL, &rect);
    // Cleanup
    SDL_FreeSurface(surface);
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
    window = SDL_CreateWindow("water", 0, 0, xres, yres, estate);
    renderer = SDL_CreateRenderer(window, -1, mode);
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
    gpu = SDL_CreateTexture(renderer, format, access, xres, yres);
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
