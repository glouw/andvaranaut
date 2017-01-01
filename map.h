#pragma once

#include <stdint.h>

int rows;
int cols;
uint8_t** ceilings;
uint8_t** wallings;
uint8_t** floorings;

void map_load(const char* path);
void map_kill(void);
