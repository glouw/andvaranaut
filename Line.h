#pragma once

#include "Types.h"

Line rotate(const Line line, const float t);
Point lerp(const Line line, const float n);
float focal(const Line line);
float ratio(const Line line);
float ccast(const Line fov, const int res, const int x);
float fcast(const Line fov, const int res, const int x);
