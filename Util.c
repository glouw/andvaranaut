#include "Util.h"

extern int fl(const float x)
{
    return (int) x - (x < (int) x);
}

extern int cl(const float x)
{
    return (int) x + (x > (int) x);
}

extern float dec(const float x)
{
    return x - (int) x;
}

extern int lns(FILE* const file)
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

extern char* readln(FILE* const file)
{
    int ch = EOF;
    int reads = 0;
    int size = 128;
    char* line = toss(char, size);
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            retoss(line, char, size *= 2);
    }
    line[reads] = '\0';
    return line;
}

extern void bomb(const char* const message, ...)
{
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    exit(1);
}

extern bool odd(const int a)
{
    return a % 2;
}

extern int balance(const int a)
{
    return odd(a) ? a + 1 : a;
}
