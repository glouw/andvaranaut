#include "Fire.h"

#include "util.h"

Fire f_kindle(const Map map)
{
    static Fire zero;
    Fire fire = zero;
    fire.rows = map.rows;
    fire.cols = map.cols;
    fire.embers = u_wipe(Embers*, fire.rows);
    for(int j = 0; j < fire.rows; j++)
        // Notice the wipe; ember append needs ember max and count to be zero.
        fire.embers[j] = u_wipe(Embers, fire.cols);
    return fire;
}

void f_clear(const Fire fire)
{
    for(int y = 0; y < fire.rows; y++)
    for(int x = 0; x < fire.cols; x++)
        fire.embers[y][x] = e_reset(fire.embers[y][x]);
}

static void gofree(const Fire fire)
{
    for(int j = 0; j < fire.rows; j++)
        free(fire.embers[j]);
    free(fire.embers);
}

void f_extinguish(const Fire fire)
{
    f_clear(fire);
    gofree(fire);
}
