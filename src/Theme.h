#pragma once

#define LIST_OF_THEMES        \
    X(NO_THEME)               \
    X(STARTING_ROOM)          \
    X(AN_EMPTY_ROOM)          \
    X(THE_HIDEOUT_OF_FAFNIR)  \
    X(THE_HOLDING_OF_GUNNAR)  \
    X(A_DWARVEN_OUTPOST)      \
    X(THE_ENTRAPMENT_OF_OLAN) \
    X(A_NICE_GARDEN)          \
    X(A_WELL_OF_WATER)

typedef enum
{
#define X(a) a,
    LIST_OF_THEMES
#undef X
    THEMES
}
Theme;

const char* t_get_name(const Theme);
