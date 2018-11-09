#include "Vram.h"

Vram v_lock(SDL_Texture* const texture)
{
    void* raw;
    int pitch;
    SDL_LockTexture(texture, NULL, &raw, &pitch);
    const Vram vram = { (uint32_t*) raw, (int) (pitch / sizeof(uint32_t)) };
    return vram;
}

void v_unlock(SDL_Texture* const texture)
{
    SDL_UnlockTexture(texture);
}

void v_draw_rooms(const Vram vram, const Map map, const uint32_t in, const uint32_t out)
{
    for(int y = 1; y < map.rows - 1; y++)
    for(int x = 1; x < map.cols - 1; x++)
    {
        if(map.walling[y][x] != ' ' && map.walling[y][x + 1] == ' ') vram.pixels[x + y * vram.width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y][x - 1] == ' ') vram.pixels[x + y * vram.width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y + 1][x] == ' ') vram.pixels[x + y * vram.width] = out;
        if(map.walling[y][x] != ' ' && map.walling[y - 1][x] == ' ') vram.pixels[x + y * vram.width] = out;
        if(map.walling[y][x] == ' ') vram.pixels[x + y * vram.width] = in;
        if(map.walling[y][x] == '!') vram.pixels[x + y * vram.width] = in;
    }
}

void v_draw_dot(const Vram vram, const Point where, const int size, const uint32_t in, const uint32_t out)
{
    for(int y = -size; y <= size; y++)
    for(int x = -size; x <= size; x++)
    {
        const int xx = x + where.x;
        const int yy = y + where.y;
        vram.pixels[xx + yy * vram.width] = in;
        if(x == -size) vram.pixels[xx + yy * vram.width] = out;
        if(x == +size) vram.pixels[xx + yy * vram.width] = out;
        if(y == -size) vram.pixels[xx + yy * vram.width] = out;
        if(y == +size) vram.pixels[xx + yy * vram.width] = out;
    }
}
