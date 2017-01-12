#pragma once

#include <stdint.h>
#include <stdbool.h>

int Map_ysz;
int Map_xsz;

uint8_t** Map_ceiling;
uint8_t** Map_walling;
uint8_t** Map_floring;

void
Map_Shutdown();

void
Map_Load(const char* const path);
