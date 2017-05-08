#pragma once

#include "Types.h"

Textures cache(const Surfaces surfaces, SDL_Renderer* const renderer);
void purge(const Textures textures);
