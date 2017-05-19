#pragma once

#include "Types.h"

void sort(const Sprites copied);

void rest(const Sprites sprites, const State state);

void walk(const Sprites sprites, const Map map); 

int count(const Sprites sprites, const State state);

Sprites wake(const char* const name);

void kill(const Sprites sprites);

Sprites swap(const Sprites sprites, const char* const name);

Sprites arrange(const Sprites sprites, const Hero hero);

Sprite registrar(const int ascii, const Point where);

void entomb(const Sprites sprites, const char* const name);
