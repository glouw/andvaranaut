#pragma once

typedef struct
{
    float x;
    float y;
}
Point;

Point p_turn(const Point, const float t);

Point p_rot90(const Point);

Point p_sub(const Point a, const Point b);

Point p_add(const Point a, const Point b);

Point p_mul(const Point a, const float n);

float p_mag(const Point);

Point p_div(const Point a, const float n);

Point p_unit(const Point);

float p_slope(const Point);

Point p_jump_hor(const Point a, const Point b);

Point p_jump_ver(const Point a, const Point b);

Point p_mid(const Point);

int p_inf(const Point);

Point p_cmp(const Point a, const Point b, const Point c);

int p_eql(const Point a, const Point b, const float e);

int p_same(const Point a, const Point b);

int p_block(const Point, char** const blocks);

int p_tile(const Point, char** const blocks);

Point p_abs(const Point);

Point p_snap(const Point, const int grid);

void p_print(const Point);
