#pragma once

typedef enum
{
    NO_THEME,
    HALL_OF_THE_DEAD,
    DEAD_KNIGHT_PARTY,
    NICE_GARDEN,
    TIMS_HOUSE,
    WATER_WELL,
    THEMES
}
Theme;

Theme* xthrand(const int count);

const char* xthname(const Theme);
