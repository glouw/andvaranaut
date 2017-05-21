#pragma once

#include "Types.h"

extern Point zro();

extern Point trn(const Point a, const float t);

extern Point rag(const Point a);

extern Point sub(const Point a, const Point b);

extern Point add(const Point a, const Point b);

extern Point mul(const Point a, const float n);

extern float mag(const Point a);

extern Point dvd(const Point a, const float n);

extern Point unt(const Point a);

extern float slp(const Point a);

extern Point shr(const Point a, const Point b);

extern Point svr(const Point a, const Point b);

extern Point cmp(const Point a, const Point b, const Point c);

extern bool eql(const Point a, const Point b, const float epsilon);

extern int block(const Point a, char** const blocks);

extern int tile(const Point a, char** const blocks);

