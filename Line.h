#pragma once

#include "Types.h"

extern Line rotate(const Line line, const float t);

extern Point lerp(const Line line, const float n);

extern float focal(const Line line);

extern float ratio(const Line line);

extern float ccast(const Line fov, const int res, const int x);

extern float fcast(const Line fov, const int res, const int x);
