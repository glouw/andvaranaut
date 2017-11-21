#pragma once

#include "Sprites.h"

typedef bool (*const Finder)(const Sprite* const);

Sprite* find(const Finder finder, const Sprites sprites);
