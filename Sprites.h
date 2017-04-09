#pragma once

#include "Types.h"

void prints(const Sprites sprites);
Sprites wake(const char* const name);
void sleep(const Sprites sprites);
Sprites swap(const Sprites sprites, const char* const name);
Sprites update(const Sprites sprites, const Hero hero);
