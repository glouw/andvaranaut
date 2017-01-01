#pragma once

#include <stdint.h>

int rows;
int cols;
uint8_t** ceilings;
uint8_t** wallings;
uint8_t** floorings;

void unload_map(void);
void load_map(const char* path);
