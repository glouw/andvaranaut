#include "Rooms.h"

#include "util.h"

Rooms r_init(const Points interests)
{
    const int count = interests.count;
    const Rooms rooms = { interests.point, t_rand(count), u_wipe(int, count), count };
    return rooms;
}
