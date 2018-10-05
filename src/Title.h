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

void xttadvance(const int now);

void xttset(const int start, const int end, const int linger, const char* const text, ...);

void xttinit(void);

void xttshow(const Text, const Sdl);

void xttclear(void);

void xttstuckclear(void);
