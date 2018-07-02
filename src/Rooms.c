#include "Rooms.h"

#include "util.h"

static Rooms spy(Rooms rms)
{
    for(int i = 0; i < rms.count; i++)
        switch(rms.themes[i])
        {
        case NICE_GARDEN:
            rms.agents[i] = 512;
            break;

        case NO_THEME:
            rms.agents[i] = 0;
            break;

        // TODO: TEMP
        default:
            rms.agents[i] = 1;
            break;
        }
    return rms;
}

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

    // Calculate number of agents per room.
    return spy(rooms);
}
