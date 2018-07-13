#include "Rooms.h"

#include "util.h"

Rooms xrsinit(const Points interests)
{
    // The number of rooms.
    const int count = interests.count;

    const Rooms rooms = {
        interests.point,

        // Room themes are randomized.
        xthrand(count),

        // Agent (sprite) count to be calculated.
        xwipe(int, count),

        count,
    };

    return rooms;
}
