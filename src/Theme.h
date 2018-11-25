#pragma once

// X-Macro helps with creating a list of strings from an enum.
#define LIST_OF_THEMES \
    X(NO_THEME)        \
    X(AN_EMPTY_ROOM)   \
    X(A_NICE_GARDEN)   \
    X(A_WELL_OF_WATER)

typedef enum
{
    #define X(a) a,
    LIST_OF_THEMES
    #undef X
    THEMES
}
Theme;

Theme* t_rand(const int count);

const char* t_get_name(const Theme);
