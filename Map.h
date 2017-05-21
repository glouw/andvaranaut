#pragma once

#include "Types.h"

extern Map open(const char* const name);

extern void dump(const Map map, const char* const name);

extern void close(const Map map);

extern Map reopen(const Map map, const char* const name);

