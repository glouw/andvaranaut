#include "util.h"

#include <stdarg.h>
#include <ctype.h>

int fl(const float x)
{
    return (int) x - (x < (int) x);
}

int cl(const float x)
{
    return (int) x + (x > (int) x);
}

float dec(const float x)
{
    return x - (int) x;
}

int lns(FILE* const file)
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

char* readln(FILE* const file)
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

void bomb(const char* const message, ...)
{
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    exit(1);
}

bool odd(const int a)
{
    return a % 2;
}

int balance(const int a)
{
    return odd(a) ? a + 1 : a;
}

char* concat(const char* const a, const char* const b)
{
    char* c = toss(char, strlen(a) + strlen(b) + 1);
    strcpy(c, a);
    strcat(c, b);
    return c;
}

void license()
{
    puts("This program comes with ABSOLUTELY NO WARRANTY.");
    puts("This is free software, and you are welcome to redistribute it");
    puts("under certain conditions.");
}
