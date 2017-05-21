#pragma once

#include "Types.h"

extern int illuminate(const Torch torch, const float distance);

extern Torch fade(const Torch torch);

extern Torch flicker(const Torch torch);

extern Torch reset();

