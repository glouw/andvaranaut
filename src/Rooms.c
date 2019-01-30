#include "Rooms.h"

#include "Sorter.h"

#include "util.h"

//
// Warning:
// Themes set to -1 when deleted and the themes array is then sorted by largest theme first.
//

static Theme themes[] = {
#define X(a) a,
    LIST_OF_THEMES
#undef X
};

static int avail_themes = THEMES;

static inline void dump(void)
{
    printf("\t%d\n", avail_themes);

    for(int i = 0; i < avail_themes; i++)
        printf("\t %s\n", t_get_name(themes[i]));

    putchar('\n');
}

static void sort(void)
{
    qsort(themes, THEMES, sizeof(*themes), s_largest_theme_first);
}

static Theme reuse(void)
{
    const Theme reusables[] = {
        AN_EMPTY_ROOM, A_WELL_OF_WATER, A_NICE_GARDEN
    };
    const int index = rand() % u_len(reusables);
    return reusables[index];
}

static Theme peek(const int index)
{
    return themes[index];
}

static Theme del(const int index)
{
    const Theme theme = peek(index);

    themes[index] = (Theme) -1;
    avail_themes--;

    sort();

    return theme;
}

static Theme pick(void)
{
    const int index = rand() % avail_themes;
    const Theme peeked = peek(index);

    //
    // Fafnir's hideout will be used for the last room.
    //

    return peeked == THE_HIDEOUT_OF_FAFNIR && avail_themes > 1 ? pick() : del(index);
}

int r_themes_left(void)
{
    return avail_themes > 0;
}

static int should_pick(void)
{
    return r_themes_left() && u_d2() == 0;
}

static Theme get(void)
{
    dump();

    //
    // Deleting the starting room as an index only works
    // the first time when array indices match enums.
    //

    if(avail_themes == THEMES)
        return del(STARTING_ROOM);

    return should_pick() ? pick() : reuse();
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
