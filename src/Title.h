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

void t_advance_title(const int now);

void t_set_title(const int start, const int length, const int linger, const char* const text, ...);

void t_show_title(const Text, const Sdl);

void t_clear_title(void);

void t_clear_title_when_linger(void);
