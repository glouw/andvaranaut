#include "Theme.h"

#include "util.h"

Theme* xthrand(const int count)
{
    Theme* const th = xwipe(Theme, count);
    for(int i = 0; i < count; i++)
    {
        const Theme theme = (Theme) (rand() % (int) THEMES);
        th[i] = (theme == NO_THEME) ? TIMS_HOUSE : theme;
    }
    return th;
}

const char* xthname(const Theme th)
{
    switch(th)
    {
    case NO_THEME: default:
        return "An Empty Room";

    case HALL_OF_THE_DEAD:
        return "Hall of the Dead";

    case DEAD_KNIGHT_PARTY:
        return "A Party of Dead Knights";

    case NICE_GARDEN:
        return "A Nice Garden";

    case TIMS_HOUSE:
        return "Timothy's House";

    case WATER_WELL:
        return "A Well of Water";
    }
}
