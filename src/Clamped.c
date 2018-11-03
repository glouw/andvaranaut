#include "Clamped.h"

#include "util.h"

Clamped c_clamp(const int yres, const float bot, const float top)
{
    const Clamped clamp = {
        (int) bot < 0 ? 0 : u_cl(bot), (int) top > yres ? yres : u_fl(top)
    };
    return clamp;
}
