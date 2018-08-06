#pragma once

#include "Sdl.h"
#include "Ttf.h"

void xttadvance(const int now);

void xttset(const int start, const int end, const int linger, const char* const text, ...);

void xttinit();

void xttshow(const Ttf, const Sdl);

void xttclear();

void xttstuckclear();
