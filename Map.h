#pragma once

typedef struct
{
    int rows;
    char** ceiling;
    char** walling;
    char** floring;
}
Map;

Map open(const char* const name);
void close(const Map map);
Map reopen(const Map map, const char* const name);
