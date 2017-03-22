#pragma once

typedef struct
{
    float x;
    float y;
}
Point;

Point trn(const Point a, const float t); // Turn rotation
Point rag(const Point a); // Right angle rotation
Point sub(const Point a, const Point b);
Point add(const Point a, const Point b);
Point mul(const Point a, const float n);
float mag(const Point a);
Point dvd(const Point a, const float n);
Point unt(const Point a); // Unit vector
float slp(const Point a); // Slope
Point shr(const Point a, const Point b); // Step horizontal
Point svr(const Point a, const Point b); // Step vertical
Point cmp(const Point a, const Point b, const Point c);
int tile(const Point a, char** const tiles);
