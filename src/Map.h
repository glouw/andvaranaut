#pragma once

#include "Points.h"
#include "Timer.h"
#include "Overview.h"
#include "Sheer.h"
#include "Theme.h"
#include "Rooms.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;

    int rows;
    int cols;

    Points trapdoors;
    Rooms rooms;

    // These sheers form an optical illusion with the middle and upper ceiling heights.
    // A larger upper sheer will extend the ceiling depth, and a larger middle sheer will
    // extend the height of the walls.
    Sheer mid;
    Sheer top;

    // Each room occupies at most (grid * grid) area of the map.
    int grid;
}
Map;

Map m_generate(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid);

Theme m_get_theme(const Theme last, const Map, const Point where, const Timer);

int m_is_portal(char** block, const Point where);

int m_out_of_bounds(const Map, const Point where);

void m_edit(const Map, const Overview);

void m_place_room(const Map, const Point where, const int w, const int h, const Party p);

void m_place_column(const Map, const Point where, const int ascii);

void m_place_platform(const Map, const int x, const int y, const Party);

void m_set_trapdoors(const Map, const Points, const Party);

void m_lay_corridor(const Map, const Point a, const Point b);

void m_place_barricades(const Map);

int m_min(const Map);

int m_max(const Map);

void m_themeate(const Map);
