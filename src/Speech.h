#pragma once

#include "Timer.h"

typedef struct
{
    char** sentences;
    int index;
    int count;
    int ticks;
}
Speech;

Speech s_swap(const Speech, const char* const str, const Timer);
