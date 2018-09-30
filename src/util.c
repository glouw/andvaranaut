#include "util.h"

#include <stdarg.h>
#include <math.h>
#include <ctype.h>

char* dups(const char* s)
{
    char* const d = xtoss(char, strlen(s) + 1);
    strcpy(d, s);
    return d;
}

int xfl(const float x)
{
    return (int) x - (x < (int) x);
}

int xcl(const float x)
{
    return (int) x + (x > (int) x);
}

float xdec(const float x)
{
    return x - (int) x;
}

int xlns(FILE* const file)
{
    int ch = EOF;
    int lines = 0;
    int pc = '\n';
    while((ch = getc(file)) != EOF)
    {
        if(ch == '\n')
            lines++;
        pc = ch;
    }
    if(pc != '\n')
        lines++;
    rewind(file);
    return lines;
}

char* xreadln(FILE* const file)
{
    int ch = EOF;
    int reads = 0;
    int size = 128;
    char* line = xtoss(char, size);
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            xretoss(line, char, size *= 2);
    }
    line[reads] = '\0';
    return line;
}

void xbomb(const char* const message, ...)
{
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    exit(1);
}

float xsinc(const float x, const float p)
{
    return sinf(FPI * x * p) / (FPI * x * p);
}

int xodd(const int a)
{
    return a % 2;
}

char* fmts(const char* const fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    // Must rewind from first call of vsnprintf.
    va_start(args, fmt);
    char* str = xtoss(char, len + 1);
    vsprintf(str, fmt, args);
    va_end(args);

    return str;
}
