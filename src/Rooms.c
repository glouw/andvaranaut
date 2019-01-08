#include "Rooms.h"

#include "Sorter.h"

#include "util.h"

// Warning: Mutates. Themes set to -1 when used.
static Theme themes[] = {
#define X(a) a,
    LIST_OF_THEMES
#undef X
};

static int avail_themes(void)
{
    int avail = 0;
    for(int i = 0; i < u_len(themes); i++)
        if(themes[i] != (Theme) -1)
            avail++;
    return avail;
}

static void sort(void)
{
    qsort(themes, THEMES, sizeof(*themes), s_largest_theme_first);
}

static Theme reuse(void)
{
    const Theme reusables[] = { AN_EMPTY_ROOM, A_WELL_OF_WATER, A_NICE_GARDEN };
    const int index = rand() % u_len(reusables);
    return reusables[index];
}

static Theme del(const int index)
{
    sort();
    const Theme theme = themes[index];
    themes[index] = (Theme) -1;
    return theme;
}

static Theme pick(void)
{
    const int index = rand() % avail_themes();
    const Theme theme = del(index);
    return theme == NO_THEME ? reuse() : theme;
}

static Theme get(void)
{
    return (u_d2() == 0 && avail_themes() > 0) ? pick() : reuse();
}

static void get_themes(const Rooms rooms)
{
    for(int i = 0; i < rooms.count; i++)
        rooms.room[i].theme = get();
}

static void get_wheres(const Rooms rooms, const Points interests)
{
    for(int i = 0; i < rooms.count; i++)
        rooms.room[i].where = interests.point[i];
}

static void set_floors(const Rooms rooms, const int floor)
{
    for(int i = 0; i < rooms.count; i++)
        rooms.room[i].floor = floor;
}

int r_themes_left(void)
{
    return avail_themes() > 0;
}

Rooms r_init(const Points interests, const int floor)
{
    const Rooms rooms = {
        u_wipe(Room, interests.count), interests.count
    };

    get_themes(rooms);
    get_wheres(rooms, interests);
    set_floors(rooms, floor);

    return rooms;
}
