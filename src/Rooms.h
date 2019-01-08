#pragma once

#include "Room.h"

typedef struct
{
    Room* room;
    int count;
}
Rooms;

Rooms r_init(const Points, const int floor);

int r_themes_left(void);
