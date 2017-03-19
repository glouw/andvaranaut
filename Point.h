#pragma once

typedef struct
{
    double x;
    double y;
}
Point;

Point turn(const Point a, const double t);
Point rag(const Point a);
Point sub(const Point a, const Point b);
Point add(const Point a, const Point b);
Point mul(const Point a, const double n);
double mag(const Point a);
Point dvd(const Point a, const double n);
Point unt(const Point a);
double slope(const Point a);
Point sh(const Point a, const Point b);
Point sv(const Point a, const Point b);
Point cmp(const Point a, const Point b, const Point c);
int tile(const Point a, char** const tiles);
