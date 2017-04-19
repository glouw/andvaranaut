#include "Sdl.h"
#include "Display.h"
#include "Boundary.h"
#include "Surfaces.h"
#include "Line.h"
#include "Hero.h"
#include "Wall.h"
#include "Util.h"

Sdl setup(const int res, const int fps, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    if(window == NULL) puts("Could not open window");
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = concat("config/", name);
    const Surfaces surfaces = pull(path, format);
    const unsigned long long renders = 0ull;
    const unsigned long long ticks = 0ull;
    const Sdl sdl = { res, fps, surfaces, window, renderer, texture, renders, ticks };
    free(path);
    return sdl;
}

void release(const Sdl sdl)
{
    SDL_DestroyTexture(sdl.texture);
    SDL_Quit();
    SDL_DestroyWindow(sdl.window);
    SDL_DestroyRenderer(sdl.renderer);
    for(int i = 0; i < sdl.surfaces.count; i++)
        SDL_FreeSurface(sdl.surfaces.surface[i]);
    free(sdl.surfaces.surface);
}

Sdl tick(const Sdl sdl, const unsigned long long renders)
{
    Sdl temp = sdl;
    temp.renders = renders;
    temp.ticks = renders / (sdl.fps / 4);
    return temp;
}

void churn(const Sdl sdl)
{
    SDL_RenderCopyEx(sdl.renderer, sdl.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

void present(const Sdl sdl)
{
    SDL_RenderPresent(sdl.renderer);
}

static void paste(const Sdl sdl, const Sprites sprites, Point* const lowers, Point* const uppers, const Hero hero)
{
    for(int which = 0; which < sprites.count; which++)
    {
        const Sprite sprite = sprites.sprite[which];
        // Move onto next sprite if this sprite is behind player
        if(sprite.where.x < 0) continue;
        // Calculate sprite size
        const float size = focal(hero.fov) * sdl.res / sprite.where.x;
        const float corner = (sdl.res - size) / 2.0;
        const int slide = (sdl.res / 2) * hero.fov.a.x * sprite.where.y / sprite.where.x;
        const SDL_Rect frame = { fl(corner) + slide, corner, size, size };
        // Trim from the left
        SDL_Rect scope = frame;
        for(; scope.x < scope.x + scope.w; scope.x++, scope.w--)
        {
            const int index = scope.x;
            if(index < 0 || index >= sdl.res) continue;
            // Stop trimming if the sprite is seen
            if(sprite.where.x < lowers[index].x) break;
        }
        // Trim from the right
        for(; scope.x > scope.x - scope.w; scope.w--)
        {
            const int index = scope.x + scope.w;
            if(index < 0 || index >= sdl.res) continue;
            // Stop trimming if the sprite is seen - Increment scope to avoid occasional clippings
            if(sprite.where.x < lowers[index].x) { scope.w++; break; }
        }
        // Move onto next sprite if this sprite is totally behind a wall
        if(scope.w <= 0) continue;
        // Move onto the next sprite if this sprite is off screen
        if(scope.x > sdl.res || scope.x + scope.w < 0) continue;
        // Trim source sprite
        const float dx = (scope.x - frame.x) / (float) frame.w;
        const float dw = (frame.w - scope.w) / (float) frame.w;
        SDL_Surface* const surface = sdl.surfaces.surface[sprite.ascii - ' '];
        // Select state
        const int states = 1;
        const int height = surface->h / states;
        const int state = height * sprite.state;
        // Select framing
        const int frames = 2;
        const int width = surface->w / frames;
        const int framing = width * (sdl.ticks % frames);
        // Get sprite on screen
        const float x = dx == 0.0 ? 0.0 : dx * width;
        const float w = width * (1.0 - dw);
        const SDL_Rect image = { fl(x) + framing, state, w, height };
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
        SDL_RenderCopy(sdl.renderer, texture, &image, &scope);
        SDL_DestroyTexture(texture);
    }
}

void render(const Sdl sdl, const Hero hero, const Sprites sprites, const Map map)
{
    const int t0 = SDL_GetTicks();
    const Line camera = rotate(hero.fov, hero.angle.theta);
    const Display display = lock(sdl);
    // Precomputes floor and ceiling casts
    float* const party = (float*) malloc(sdl.res * sizeof(*party));
    const int m = sdl.res / 2, l = sdl.res;
    for(int x = 0; x < m; x++) party[x] = fcast(hero.fov, sdl.res, x);
    for(int x = m; x < l; x++) party[x] = ccast(hero.fov, sdl.res, x);
    // Saves lower ray casts for sprite renderer
    Point* const lowers = (Point*) malloc(sdl.res * sizeof(*lowers));
    Point* const uppers = (Point*) malloc(sdl.res * sizeof(*uppers));
    Point* const wheres = (Point*) malloc(sdl.res * sizeof(*wheres));
    for(int y = 0; y < sdl.res; y++)
    {
        const Point column = lerp(camera, y / (float) sdl.res);
        const Scanline scanline = { sdl, display, y };
        // Several upper walls are rendered for seamless indoor/outdoor transitions.
        // Maps are required to have the outer most wall thickness _as many_ chars wide
        for(int max = 5, min = 1, hits = max; hits >= min; hits--)
        {
            const Impact upper = march(hero, map.ceiling, column, sdl.res, hits);
            const Boundary boundary = { scanline, raise(upper.wall, sdl.res) };
            if(hits == max) srend(boundary, hero.angle.percent);
            if(hits == min) uppers[y] = upper.traceline.corrected;
            wrend(boundary, upper.hit);
        }
        const Impact lower = march(hero, map.walling, column, sdl.res, 1);
        const Boundary boundary = { scanline, lower.wall };
        const Tracery tracery = { lower.traceline, party };
        wrend(boundary, lower.hit);
        frend(boundary, wheres, map.floring, tracery);
        crend(boundary, wheres, map.ceiling);
        // Save lower and upper wall hits for the sprite renderer
        lowers[y] = lower.traceline.corrected;
    }
    // Update screen
    unlock(sdl);
    churn(sdl);
    paste(sdl, sprites, lowers, uppers, hero);
    present(sdl);
    // Memory deallocation
    free(wheres);
    free(lowers);
    free(uppers);
    free(party);
    // 60 frames per second lock
    const int t1 = SDL_GetTicks();
    const int ms = 1000.0 / sdl.fps - (t1 - t0);
    SDL_Delay(ms < 0 ? 0 : ms);
}
