#include "Theme.h"

#include "util.h"

Theme* xthrand(const int count)
{
    Theme* const th = xwipe(Theme, count);
    for(int i = 0; i < count; i++)
    {
        // Random theme for a room (this includes NOTHEME which is not desirable).
        th[i] = (Theme) (rand() % (int) THEMES);
        if(th[i] == NO_THEME)
            th[i] = NICE_GARDEN;
    }
    return th;
}

char* xthname(const Theme th)
{
    switch(th)
    {
    default:
    case NO_THEME:
        return (char*) "An Empty Room";
    case HALL_OF_THE_DEAD:
        return (char*) "Hall of the Dead";
    case DEAD_KNIGHT_PARTY:
        return (char*) "Dead Knight Party";
    case NICE_GARDEN:
        return (char*) "A Nice Garden";
    case TIMS_HOUSE:
        return (char*) "Timothy's House";
    case WATER_WELL:
        return (char*) "A Well of Water";
    }
}
