#include "String.h"

#include "Util.h"

extern char* concat(const char* const a, const char* const b)
{
    char* c = toss(char, strlen(a) + strlen(b) + 1);
    strcpy(c, a);
    strcat(c, b);
    return c;
}

extern bool match(const char* const a, const char* const b)
{
    return strcmp(a, b) == 0;
}

extern char* trim(char* const a)
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
