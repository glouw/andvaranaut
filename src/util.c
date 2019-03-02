#include "util.h"

#include <dirent.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

char* u_str_dup(const char* s)
{
    char* const d = u_toss(char, strlen(s) + 1);
    strcpy(d, s);
    return d;
}

int u_char_count(const char* const str, const char delim)
{
    const int len = strlen(str);

    int count = 0;
    for(int i = 0; i < len; i++)
        if(str[i] == delim)
            count++;

    return count;
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

void u_check_art_directory(void)
{
#define BASE "art/"
    const char* dirs[] = {
        BASE,
        BASE"gui",
        BASE"items",
        BASE"screenshots",
        BASE"sprites",
        BASE"tiles",
    };
    for(int i = 0; i < u_len(dirs); i++)
        if(!opendir(dirs[i]))
            u_bomb("error: missing directory %s\n", dirs[i]);
#undef BASE
}
