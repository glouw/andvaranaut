#pragma once

#include "Types.h"

Map open(const char* const name);

void close(const Map map);

Map reopen(const Map map, const char* const name);

void edit(const Map map, const Hero hero, const uint8_t* key);
