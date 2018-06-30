#include "Rooms.h"

#include "util.h"

static Rooms spy(Rooms rms)
{
    for(int i = 0; i < rms.count; i++)
        rms.agents[i] = 4;
    return rms;
}

Rooms xrsinit(const Points interests)
{
    // The number of rooms.
    const int count = interests.count;

    Rooms rooms = {
        interests.point,

        // Room themes are randomized.
        xthrand(count),

        // Agent (sprite) count to be calculated.
        xwipe(int, count),

        count,
    };

    // Calculate number of agents per room.
    rooms = spy(rooms);

    return rooms;
}
