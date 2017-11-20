#pragma once

#include "Sprites.h"

typedef bool (*const Excluding)(const Sprite* const);

bool width(const Sprite* const sprite);

Sprite* find(const Excluding excluding, const Sprites sprites, const State state);
