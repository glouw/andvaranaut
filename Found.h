#pragma once

#include "Sprites.h"

typedef bool (*const Found)(const Sprite* const);

bool xgrabbed(const Sprite* const sprite);

Sprite* find(const Found, const Sprites);
