#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t* colors;
    int count;

    //
    // Palette color name aliases.
    //

    uint32_t blk;
    uint32_t brn;
    uint32_t red;
    uint32_t yel;
    uint32_t wht;
}
Palette;

Palette p_make(void);

void p_save(const Palette);
