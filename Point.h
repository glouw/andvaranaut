#pragma once

#include "Types.h"

Point zro();

Point trn(const Point a, const float t);

Point rag(const Point a);

Point sub(const Point a, const Point b);

Point add(const Point a, const Point b);

Point mul(const Point a, const float n);

float mag(const Point a);

Point dvd(const Point a, const float n);

Point unt(const Point a);

float slp(const Point a);

Point shr(const Point a, const Point b);

Point svr(const Point a, const Point b);

Point cmp(const Point a, const Point b, const Point c);

int block(const Point a, char** const blocks);

int tile(const Point a, char** const blocks);
