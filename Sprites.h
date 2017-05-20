#pragma once

#include "Types.h"

void rest(const Sprites sprites, const State state);

int count(const Sprites sprites, const State state);

Sprites wake(const char* const name);

void kill(const Sprites sprites);

Sprites swap(const Sprites sprites, const char* const name);

Sprites arrange(const Sprites sprites, const Hero hero);

void rearrange(const Sprites sprites, const Hero hero);

Sprite registrar(const int ascii, const Point where);

void entomb(const Sprites sprites, const char* const name);

void prints(const Sprites sprites);
