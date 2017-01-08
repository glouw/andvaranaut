#pragma once

#include <stdint.h>
#include <stdbool.h>

int map_ymax;
int map_xmax;
bool map_inside;
bool map_outside;
uint8_t** map_roofings;
uint8_t** map_ceilings;
uint8_t** map_wallings;
uint8_t** map_floorings;

void
map_unload(void);

void
map_load(const char* path);
