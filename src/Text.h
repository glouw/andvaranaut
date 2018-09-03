#pragma once

#include "Font.h"

typedef struct
{
    Font fill;
    Font line;
}
Text;

Text xtxnew(const char* const path, const int size, const uint32_t inner, const uint32_t outer);
