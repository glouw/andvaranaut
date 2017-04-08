#pragma once

#include "Types.h"

Sprites wake(const char* const name);
void sleep(const Sprites sprites);
Sprites swap(const Sprites sprites, const char* const name);
void prints(const Sprites sprites);
Sprites update(const Sprites sprites, const Hero hero);
void paste(const Sprites sprites, const Gpu gpu, Impact* const impacts, const Hero hero, const int res);
