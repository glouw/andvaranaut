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
Fresh2D()
{
    uint8_t** const array = malloc(Map_ysz * sizeof(uint8_t*));
    for(int i = 0; i < Map_ysz; i++)
        array[i] = NULL;
    for(int i = 0; i < Map_ysz; i++)
        array[i] = malloc(Map_xsz * sizeof(uint8_t));
    for(int i = 0; i < Map_ysz; i++)
    for(int j = 0; j < Map_xsz; j++)
        array[i][j] = 0;
    return array;
}

static uint8_t**
GetParty(FILE* const fp)
{
    uint8_t** const array = Fresh2D();
    for(int i = 0; i < Map_ysz; i++)
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
Kill2D(uint8_t** const array)
{
    for(int i = 0; i < Map_ysz; i++)
        free(array[i]);
    free(array);
}

static void
Print2D(uint8_t** const array)
{
    for(int i = 0; i < Map_ysz; i++) {
    for(int j = 0; j < Map_xsz; j++)
        printf("%d ", array[i][j]);
    putchar('\n');
    }
}

static void
Dump()
{
    printf("Map size {\n\tx = %d\n\ty = %d\n}\n", Map_xsz, Map_ysz);
    Print2D(Map_ceiling);
    Print2D(Map_walling);
    Print2D(Map_floring);
}

void
Map_Shutdown()
{
    Kill2D(Map_ceiling);
    Kill2D(Map_walling);
    Kill2D(Map_floring);
}

void
Map_Load(const char* const path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // Map size
    line = GetLine(fp);
    sscanf(line, "%d %d", &Map_ysz, &Map_xsz);
    free(line);
    // Map
    Map_ceiling = GetParty(fp);
    Map_walling = GetParty(fp);
    Map_floring = GetParty(fp);
    fclose(fp);
    // Done
    Dump();
}
