#pragma once

#include <stdint.h>

int ymax;
int xmax;
uint8_t** ceilings;
uint8_t** wallings;
uint8_t** floorings;

void
map_unload(void);

void
map_load(const char* path);
