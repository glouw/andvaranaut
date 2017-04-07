#pragma once

#include "Types.h"

// The zero vector
Point zro();
// Turn
Point trn(const Point a, const float t);
// Right angle rotation
Point rag(const Point a);
Point sub(const Point a, const Point b);
Point add(const Point a, const Point b);
Point mul(const Point a, const float n);
float mag(const Point a);
Point dvd(const Point a, const float n);
// Unit vector
Point unt(const Point a);
// Slope
float slp(const Point a);
// Step horizontal
Point shr(const Point a, const Point b);
// Step vertical
Point svr(const Point a, const Point b);
// Compare
Point cmp(const Point a, const Point b, const Point c);
int tile(const Point a, char** const tiles);
