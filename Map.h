#pragma once

#include "Types.h"

extern Map open(const int level);

extern void dump(const Map map, const int level);

extern void close(const Map map);

extern Map reopen(const Map map, const int level);

extern bool isportal(const Map map, const Point where);

