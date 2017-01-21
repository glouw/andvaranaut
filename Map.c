// Copyright (C) 2016-2017 Gustav Louw
// 
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

static inline char*
GetLine(FILE* const fp)
{
    if(ungetc(getc(fp), fp) == EOF) return NULL;
    int sz = 8;
    char* buffer = malloc(sz); assert(buffer);
    int i = 0;
    for(char c; (c = getc(fp)) != '\n'; buffer[i++] = c)
        if(i == sz - 1)
            buffer = realloc(buffer, sz *= 2);
    buffer[i] = '\0';
    return buffer;
}

static inline uint8_t**
FreshParty(const int ysz, const int xsz)
{
    uint8_t** const array = malloc(ysz * sizeof(uint8_t*)); assert(array);
    for(int i = 0; i < ysz; i++)
        array[i] = NULL;
    for(int i = 0; i < ysz; i++)
    {
        array[i] = malloc(xsz * sizeof(uint8_t));
        assert(array[i]);
    }
    for(int i = 0; i < ysz; i++)
    for(int j = 0; j < xsz; j++)
        array[i][j] = 0;
    return array;
}

static inline uint8_t**
GetParty(FILE* const fp, const int ysz, const int xsz)
{
    uint8_t** const array = FreshParty(ysz, xsz);
    for(int i = 0; i < ysz; i++)
    {
        char* const line = GetLine(fp);
        const char* tile;
        int j = 0;
        for(char* temp = line; (tile = strtok(temp, " ")); temp = NULL)
            array[i][j++] = atoi(tile);
        free(line);
    }
    return array;
}

Map
Map_Load(const char* const path)
{
    FILE* fp = fopen(path, "r"); assert(fp);
    char* line;
    // Map size
    int ysz;
    int xsz;
    line = GetLine(fp);
    sscanf(line, "%d %d", &ysz, &xsz);
    free(line);
    // Hero location
    Point where;
    line = GetLine(fp);
    sscanf(line, "%lf %lf", &where.x, &where.y);
    free(line);
    // Hero theta
    double theta;
    line = GetLine(fp);
    sscanf(line, "%lf", &theta);
    free(line);
    // Map
    uint8_t** ceiling = GetParty(fp, ysz, xsz);
    uint8_t** walling = GetParty(fp, ysz, xsz);
    uint8_t** floring = GetParty(fp, ysz, xsz);
    fclose(fp);
    // Done
    const Map map = {
        .ysz = ysz,
        .xsz = xsz,
        .ceiling = ceiling,
        .walling = walling,
        .floring = floring,
        .where = where,
        .theta = theta
    };
    return map;
}

void
Map_Unload(const Map map)
{
    for(int i = 0; i < map.ysz; i++)
    {
        free(map.ceiling[i]);
        free(map.walling[i]);
        free(map.floring[i]);
    }
    free(map.ceiling);
    free(map.walling);
    free(map.floring);
}
