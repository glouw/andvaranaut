#include "String.h"

char* build(const char* const str)
{
    char* copy = (char*) malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

char* concat(const char* const a, const char* const b)
{
    char* c = (char*) malloc(strlen(a) + strlen(b) + 1);
    strcpy(c, a);
    strcat(c, b);
    return c;
}
