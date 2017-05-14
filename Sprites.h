#pragma once

#include "Types.h"

Sprites wake(const char* const name);

void kill(const Sprites sprites);

Sprites swap(const Sprites sprites, const char* const name);

Sprites arrange(const Sprites sprites, const Hero hero);

void rest(const Sprites sprites);

bool issprite(const int ascii);

Sprite oddish(const Point where);
