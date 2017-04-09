#include "Gpu.h"
#include "Surfaces.h"
#include "Line.h"
#include "Util.h"

Gpu setup(const int res, const char* const name)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* const window = SDL_CreateWindow("water", 0, 0, res, res, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Texture* const texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, res, res);
    char* const path = strcon("config/", name);
    const Gpu gpu = { pull(path, format), window, renderer, texture };
    free(path);
    return gpu;
}

void release(const Gpu gpu)
{
    SDL_DestroyTexture(gpu.texture);
    SDL_Quit();
    SDL_DestroyWindow(gpu.window);
    SDL_DestroyRenderer(gpu.renderer);
    for(int i = 0; i < gpu.surfaces.count; i++)
        SDL_FreeSurface(gpu.surfaces.surface[i]);
    free(gpu.surfaces.surface);
}

void churn(const Gpu gpu)
{
    SDL_RenderCopyEx(gpu.renderer, gpu.texture, NULL, NULL, -90.0, NULL, (SDL_RendererFlip) 0);
}

void present(const Gpu gpu)
{
    SDL_RenderPresent(gpu.renderer);
}

void paste(const Gpu gpu, const Sprites sprites, const Impact* const impacts, const Hero hero, const int res)
{
    for(int i = 0; i < sprites.count; i++)
    {
        const Sprite sprite = sprites.sprite[i];
        // Move onto the next sprite if this sprite is behind the player
        if(sprite.where.x < 0)
            continue;
        // Move onto the next sprite if this sprite is behind the player
        const int offset = (res / 2) * hero.fov.a.x * sprite.where.y / (float) sprite.where.x;
        const int size = focal(hero.fov) * res / sprite.where.x;
        const int locate = offset + res / 2;
        const struct { int min, max; } boundry = { -size / 2, res + size / 2 };
        if(locate < boundry.min || locate > boundry.max)
            continue;
        /* Destination sprite trimming */
        SDL_Rect dst;
        dst.y = (res - size) / 2;
        dst.h = size;
        // Trim destination sprite from the left
        int lb = 0;
        int rb = size;
        for(; lb < rb; lb++)
        {
            dst.x = (res - size) / 2 + lb + offset;
            dst.w = rb - lb;
            const int index = dst.x;
            if(index < 0 || index > res)
                break;
            // Stop trimming of the sprite is seen
            if(sprite.where.x < impacts[index].traceline.corrected.x)
                break;
        }
        // Trim destination sprite from the right
        for(; rb > lb; rb--)
        {
            dst.w = rb - lb;
            const int index = dst.x + dst.w;
            if(index < 0 || index > res)
                break;
            // Stop trimming of the sprite is seen
            if(sprite.where.x < impacts[index].traceline.corrected.x)
                break;
        }
        // Move onto the next sprite if this sprite is not seen
        if(rb <= lb)
            continue;
        /* Source sprite trimming */
        SDL_Surface* const surface = gpu.surfaces.surface[sprite.ascii - ' '];
        const float scale = (float) surface->w / size;
        const SDL_Rect src = { scale * lb, 0, scale * dst.w, surface->h };
        // Paste to renderer
        SDL_Texture* const texture = SDL_CreateTextureFromSurface(gpu.renderer, surface);
        SDL_RenderCopy(gpu.renderer, texture, &src, &dst);
        SDL_DestroyTexture(texture);
    }
}
