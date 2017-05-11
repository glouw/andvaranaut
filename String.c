#include "String.h"

#include "Util.h"

char* build(const char* const str)
{
    char* copy = toss(char, strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

char* concat(const char* const a, const char* const b)
{
    char* c = toss(char, strlen(a) + strlen(b) + 1);
    strcpy(c, a);
    strcat(c, b);
    return c;
}

bool match(const char* const a, const char* const b)
{
    return strcmp(a, b) == 0;
}

char* trim(char* const a)
{
    char* start = a;
    // Trim from the left
    while(isspace(*start)) start++;
    // Trim from the right
    char* end = start + strlen(start);
    while(isspace(*--end)) *end = '\0';
    // Want the mirror?
    return start;
}

float floating(const char* const a)
{
    return strtof(a, NULL);
}
