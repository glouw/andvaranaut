#pragma once

#include "Types.h"

int illuminate(const Torch torch, const float distance);

Torch fade(const Torch torch);

Torch flicker(const Torch torch);

Torch reset();
