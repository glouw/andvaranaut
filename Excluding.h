#pragma once

#include "Sprites.h"

typedef bool (*Excluding)(const Sprite* const);

bool width(const Sprite* const sprite);

Sprite* find(const Sprites sprites, const State state, Excluding excluding);
