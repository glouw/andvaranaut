#pragma once

#include "Scanline.h"
#include "Impact.h"
#include "Torch.h"
#include "Point.h"

typedef struct
{
    Scanline scanline;
    Impact impact;
}
Sliver;

void wrend(const Sliver sliver, const Torch torch, int* const moddings);

void frend(const Sliver sliver, char** const floring, const Torch torch, Point* const wheres, int* const moddings, const float* party);

void crend(const Sliver sliver, char** const ceiling, Point* wheres);

void light(const Sliver sliver, int* const moddings);
