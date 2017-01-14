#pragma once

#include <stdint.h>

typedef struct map
{
    int ysz;
    int xsz;
    uint8_t** ceiling;
    uint8_t** walling;
    uint8_t** floring;
}
Map;

Map
Map_Load(const char* const path);

void
Map_Unload(Map map);
