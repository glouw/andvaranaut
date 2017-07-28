#pragma once

#include "Types.h"

extern Sprites wake(const int level);

extern void entomb(const Sprites sprites, const int level);

extern void kill(const Sprites sprites);

extern Sprites rewake(const Sprites sprites, const int level);

extern Sprites arrange(const Sprites sprites, const Hero hero);

extern void rest(const Sprites sprites, const State state);

extern bool issprite(const int ascii);

extern Sprites caretake(const Sprites sprites, const Hero hero, const Input input);

