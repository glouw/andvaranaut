#pragma once

#include "Font.h"

typedef struct
{
    Font fill;
    Font line;
}
Text;

Text t_build(const char* const path, const int size, const uint32_t inner, const uint32_t outer);
