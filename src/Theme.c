#include "Theme.h"

#include "Sorter.h"

#include "util.h"

static Theme themes[] = {
#define X(a) a,
    LIST_OF_THEMES
#undef X
};

static int size = THEMES; /* warning: mutates */

static void sort(void)
{
    qsort(themes, THEMES, sizeof(themes[0]), s_largest_int_first);
}

int t_themes_left(void)
{
    return size > 0;
}

static Theme reuse(void)
{
    const Theme reusables[] = {
        AN_EMPTY_ROOM,
        A_WELL_OF_WATER,
        A_NICE_GARDEN,
    };
    const int index = rand() % u_len(reusables);
    return reusables[index];
}

static Theme pick()
{
    sort();
    const int index = rand() % size;
    const Theme theme = themes[index];
    themes[index] = (Theme) -1;
    size--;
    return theme == NO_THEME ? reuse() : theme;
}

static Theme get(void)
{
    return (u_d2() == 0 && size > 0) ? pick() : reuse();
}

Theme* t_rand(const int count)
{
    Theme* const th = u_wipe(Theme, count);
    for(int i = 0; i < count; i++)
        th[i] = get();
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
