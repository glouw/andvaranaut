#pragma once

#include "Font.h"
#include "Position.h"

typedef struct
{
    Font fill;
    Font line;
}
Text;

Text t_build(const char* const path, const int size, const uint32_t inner, const uint32_t outer);

void t_write(
    const Text, SDL_Renderer* const, const int x, const int y, const Position, const int alpha, const int line, const char* const str);

int t_puts(
    const Text, SDL_Renderer* const, const int x, const int y, const Position, const int alpha, const int line, const char* const str);

int t_printf(
    const Text, SDL_Renderer* const, const int x, const int y, const Position, const int alpha, const int line, const char* const fmt, ...);
