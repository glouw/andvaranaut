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

Theme* t_rand(const int count);

const char* t_name(const Theme);
