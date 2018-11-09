#include "util.h"

#include <stdarg.h>
#include <math.h>
#include <ctype.h>

char* u_str_dup(const char* s)
{
    char* const d = u_toss(char, strlen(s) + 1);
    strcpy(d, s);
    return d;
}

int u_fl(const float x)
{
    return (int) x - (x < (int) x);
}

int u_cl(const float x)
{
    return (int) x + (x > (int) x);
}

float u_dec(const float x)
{
    return x - (int) x;
}

void u_bomb(const char* const message, ...)
{
    va_list args;

    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    exit(1);
}

float u_sinc(const float x, const float p)
{
    return sinf(U_PI * x * p) / (U_PI * x * p);
}

int u_odd(const int a)
{
    return a % 2;
}
