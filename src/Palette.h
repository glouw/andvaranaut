#pragma once

#include <stdint.h>

typedef struct
{
    uint32_t* colors;
    int count;
}
Palette;

Palette p_make(void);

void p_save_color_pallete_as_pal(void);
