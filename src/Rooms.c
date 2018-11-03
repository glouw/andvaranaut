#include "Rooms.h"

#include "util.h"

Rooms r_init(const Points interests)
{
    const int count = interests.count;
    const Rooms rooms = { interests.point, xthrand(count), xwipe(int, count), count };
    return rooms;
}
