#pragma once

#include "Types.h"

extern Sprites wake(const int level);

extern void entomb(const Sprites sprites, const int level);

extern void kill(const Sprites sprites);

extern Sprites arrange(const Sprites sprites, const Hero hero);

extern void rest(const Sprites sprites, const State state);

extern bool issprite(const int ascii);

extern Sprites flourish(const Sprites sprites, const Hero hero, const Map map, const Input input);

