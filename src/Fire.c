#include "Fire.h"

#include "util.h"

Fire f_kindle(const Map map)
{
    static Fire zero;
    Fire f = zero;
    f.rows = map.rows;
    f.cols = map.cols;
    f.embers = u_wipe(Embers*, f.rows);

    //
    // Notice the wipe; ember append needs ember max and count to be zero.
    //

    for(int j = 0; j < f.rows; j++)
        f.embers[j] = u_wipe(Embers, f.cols);

    return f;
}

void f_clear(const Fire f)
{
    for(int y = 0; y < f.rows; y++)
    for(int x = 0; x < f.cols; x++)
        f.embers[y][x] = e_reset(f.embers[y][x]);
}

static void go_free(const Fire f)
{
    for(int j = 0; j < f.rows; j++)
        free(f.embers[j]);
    free(f.embers);
}

void f_extinguish(const Fire f)
{
    f_clear(f);
    go_free(f);
}
