#include "util.h"

#include <stdarg.h>
#include <math.h>
#include <ctype.h>

char* u_dups(const char* s)
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

char* u_readln(FILE* const file)
{
    int ch = EOF;
    int reads = 0;
    int size = 128;
    char* line = u_toss(char, size);
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            u_retoss(line, char, size *= 2);
    }
    line[reads] = '\0';
    return line;
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
