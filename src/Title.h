#pragma once

#include "Sdl.h"
#include "Text.h"

typedef struct
{
    int start;
    int now;
    int end;
    int linger;
    char* str;
}
Title;

void t_advance(const int now);

void t_set(const int start, const int end, const int linger, const char* const text, ...);

void t_init(void);

void t_show(const Text, const Sdl);

void t_clear(void);

void t_clear_when_linger(void);
