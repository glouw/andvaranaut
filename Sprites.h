#pragma once

#include "Types.h"

extern Sprite* find(const Sprites sprites, const State state);

extern int count(const Sprites sprites, const State state);

extern void prints(const Sprites sprites);

extern Sprites wake(const char* const name);

extern void entomb(const Sprites sprites, const char* const name);

extern void kill(const Sprites sprites);

extern Sprites swap(const Sprites sprites, const char* const name);

extern Sprites arrange(const Sprites sprites, const Hero hero);

extern void rest(const Sprites sprites, const State state);

extern void flourish(const Sprites sprites, const Hero hero, const Map map, const uint8_t* const key);

extern Sprite registrar(const int ascii, const Point where);

