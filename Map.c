#include "Map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*
GetLine(FILE* const fp)
{
    if(ungetc(getc(fp), fp) == EOF) return NULL;
    int sz = 8;
    char* buffer = malloc(sz);
    int i = 0;
    for(char c; (c = getc(fp)) != '\n'; buffer[i++] = c)
        if(i == sz - 1)
            buffer = realloc(buffer, sz *= 2);
    buffer[i] = '\0';
    return buffer;
}

static uint8_t**
FreshParty(const int ysz, const int xsz)
{
    uint8_t** const array = malloc(ysz * sizeof(uint8_t*));
    for(int i = 0; i < ysz; i++)
        array[i] = NULL;
    for(int i = 0; i < ysz; i++)
        array[i] = malloc(xsz * sizeof(uint8_t));
    for(int i = 0; i < ysz; i++)
    for(int j = 0; j < xsz; j++)
        array[i][j] = 0;
    return array;
}

static uint8_t**
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

static void
KillParty(uint8_t** const array, const int ysz)
{
    for(int i = 0; i < ysz; i++)
        free(array[i]);
    free(array);
}

static void
PrintParty(uint8_t** const array, const int ysz, const int xsz)
{
    for(int i = 0; i < ysz; i++) {
    for(int j = 0; j < xsz; j++)
        printf("%d ", array[i][j]);
    putchar('\n');
    }
}

static void
Print(Map map)
{
    printf("Map size {\n\tx = %d\n\ty = %d\n}\n", map.xsz, map.ysz);
    PrintParty(map.ceiling, map.ysz, map.xsz);
    PrintParty(map.walling, map.ysz, map.xsz);
    PrintParty(map.floring, map.ysz, map.xsz);
}

void
Map_Unload(Map map)
{
    KillParty(map.ceiling, map.ysz);
    KillParty(map.walling, map.ysz);
    KillParty(map.floring, map.ysz);
}

Map
Map_Load(const char* const path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // Map size
    int ysz;
    int xsz;
    line = GetLine(fp);
    sscanf(line, "%d %d", &ysz, &xsz);
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
    };
    Print(map);
    return map;
}
