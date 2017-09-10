#pragma once

#include "Scanline.h"

typedef struct
{
    Scanline scanline;
    Impact impact;
}
Sliver;

/* 
 * These functions operate on a single scanline.
 * The floor renderer saves calculations for the ceiling renderer.
 * The light function applies gamma darkening to the wall, floor, and ceiling
 */

// Wall renderer
void wrend(
    const Sliver sliver,
    const Torch torch,
    int* const moddings);

// Floor renderer
void frend(
    const Sliver sliver,
    char** const floring,
    const Torch torch,
    Point* const wheres,
    int* const moddings,
    const float* party);

// Ceiling renderer
void crend(
    const Sliver sliver,
    char** const ceiling,
    Point* wheres);

// Light renderer
void light(const Sliver sliver, int* const moddings);
