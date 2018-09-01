#pragma once

typedef struct
{
    float x;
    float y;
}
Point;

Point xzpoint(void);

Point xtrn(const Point, const float t);

Point xrag(const Point);

Point xsub(const Point a, const Point b);

Point xadd(const Point a, const Point b);

Point xmul(const Point a, const float n);

float xmag(const Point);

Point xdiv(const Point a, const float n);

Point xunt(const Point);

float xslp(const Point);

Point xshr(const Point a, const Point b);

Point xsvr(const Point a, const Point b);

Point xmid(const Point);

int xinf(const Point);

Point xcmp(const Point a, const Point b, const Point c);

int xeql(const Point a, const Point b, const float e);

int xpsame(const Point a, const Point b);

int xblok(const Point, char** const blocks);

int xtile(const Point, char** const blocks);

Point xabs(const Point);

Point xsnap(const Point, const int grid);

Point xrand(Point, const int grid);

void xpprint(const Point);
