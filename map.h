#pragma once

#include <stdint.h>

int map_ymax;
int map_xmax;
uint8_t** map_ceilings;
uint8_t** map_wallings;
uint8_t** map_floorings;

void
map_unload(void);

void
map_load(const char* path);
