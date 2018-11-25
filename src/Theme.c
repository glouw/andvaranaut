#include "Theme.h"

#include "util.h"

Theme* t_rand(const int count)
{
    Theme* const th = u_wipe(Theme, count);
    for(int i = 0; i < count; i++)
    {
        const Theme theme = (Theme) (rand() % (int) THEMES);
        th[i] = (theme == NO_THEME) ? AN_EMPTY_ROOM : theme;
    }
    return th;
}

const char* t_get_name(const Theme th)
{
    static const char* const strings[] = {
        #define X(a) #a,
        LIST_OF_THEMES
        #undef X
    };
    return strings[th];
}
