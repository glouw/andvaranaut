// Copyright (C) 2016-2017 Gustav Louw
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct point
{
    float x, y;
}
Point;

Point
Point_Cast(const Point where, const float radians, uint8_t** const party);

Point
Point_Sub(const Point i, const Point j);

Point
Point_Add(const Point i, const Point j);

Point
Point_Mul(const Point i, const float n);

uint8_t
Point_TileEnclosure(const Point point, uint8_t** party);

uint8_t
Point_TileParty(const Point point, uint8_t** party);

bool
Point_Collision(const Point point, uint8_t** party);

float
Point_Magnitude(const Point point);

float
Point_Percent(const Point point, uint8_t** party);

float
Point_Decimal(const float d);
