#pragma once

#include <stdint.h>

int rows;
int cols;
uint8_t** ceilings;
uint8_t** wallings;
uint8_t** floorings;

void unload(void);
void load(const char* path);
