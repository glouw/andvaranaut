#pragma once

#include "Types.h"

extern Sprites wake(const char* const name);

extern void entomb(const Sprites sprites, const char* const name);

extern void kill(const Sprites sprites);

extern Sprites arrange(const Sprites sprites, const Hero hero);

extern void rest(const Sprites sprites, const State state);

extern void flourish(const Sprites sprites, const Hero hero, const Map map);

extern Sprite registrar(const int ascii, const Point where);

