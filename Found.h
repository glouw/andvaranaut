#pragma once

#include "Sprites.h"

typedef int (*const Found)(const Sprite* const);

int xgrabbed(const Sprite* const sprite);

Sprite* find(const Found, const Sprites);
