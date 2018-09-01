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

char* xthname(const Theme th)
{
    switch(th)
    {
    case NO_THEME: default:
        return (char*) "An Empty Room";

    case HALL_OF_THE_DEAD:
        return (char*) "Hall of the Dead";

    case DEAD_KNIGHT_PARTY:
        return (char*) "A Party of Dead Knights";

    case NICE_GARDEN:
        return (char*) "A Nice Garden";

    case TIMS_HOUSE:
        return (char*) "Timothy's House";

    case WATER_WELL:
        return (char*) "A Well of Water";
    }
}
