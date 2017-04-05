#pragma once

typedef struct
{
    int rows;
    char** ceiling;
    char** walling;
    char** floring;
}
Map;

Map open(const char* const path);
void close(const Map map);
Map reopen(const Map map, const char* const path);
