#pragma once

#include "Font.h"

typedef struct
{
    Font fill;
    Font line;
}
Text;

Text t_build(const char* const path, const int size, const uint32_t inner, const uint32_t outer);

void t_put(
    const Text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void t_put_bottom_right(
    const Text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void t_put_top_left(
    const Text text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y);

void t_print(
    const Text, const char* const str, const int alpha, SDL_Renderer* const rend, const int x, const int y);
