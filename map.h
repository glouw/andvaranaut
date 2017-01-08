#pragma once

#include <stdint.h>
#include <stdbool.h>

int map_y;
int map_x;
bool map_inside;
bool map_outside;
// Roofing tiles
uint8_t** map_r;
// Ceiling tiles
uint8_t** map_c;
// Walling tiles
uint8_t** map_w;
// Flooring tiles
uint8_t** map_f;

void
map_unload(void);

void
map_load(const char* path);
