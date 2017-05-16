#pragma once

#include "Types.h"

Map open(const char* const name);

void close(const Map map);

void dump(const Map map, const char* const name);

Map reopen(const Map map, const char* const name);
