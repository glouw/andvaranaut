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

    // Map width and height.
    int rows;
    int cols;

    // Trapdoor points for moving up and down map floors.
    Points trapdoors;

    // Room information, eg. room location, number of agents (sprites), theme, etc.
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

Map m_gen(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid);

int m_is_portal(char** block, const Point where);

void m_edit(const Map, const Overview);

int m_out(const Map, const Point where);

int m_carvable(const Map, const int x, const int y);

void m_room(const Map, const Point where, const int w, const int h, const Party);

void m_column(const Map, const Point where, const int ascii);

void m_trapdoors(const Map, const Points trapdoors, const Party);

void m_corridor(const Map, const Point a, const Point b);

Theme m_theme(const Theme, const Map, const Point where, const Timer);

void m_platform(const Map, const int x, const int y, const Party);

void m_barricade(const Map);

int m_min(const Map);

int m_max(const Map);

void m_themeate(const Map);
