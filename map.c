#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*
getline(FILE* const fp)
{
    if(ungetc(getc(fp), fp) == EOF)
        return NULL;
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
fresh2d(void)
{
    uint8_t** const array = malloc(map_ymax * sizeof(uint8_t*));
    for(int i = 0; i < map_ymax; i++)
        array[i] = NULL;
    for(int i = 0; i < map_ymax; i++)
        array[i] = malloc(map_xmax * sizeof(uint8_t));
    for(int i = 0; i < map_ymax; i++)
    for(int j = 0; j < map_xmax; j++)
        array[i][j] = 0;
    return array;
}

static uint8_t**
gettile(FILE* const fp)
{
    uint8_t** const array = fresh2d();
    for(int i = 0; i < map_ymax; i++)
    {
        char* const line = getline(fp);
        const char* tile;
        int j = 0;
        for(char* temp = line; (tile = strtok(temp, " ")); temp = NULL)
            array[i][j++] = atoi(tile);
        free(line);
    }
    return array;
}

static void
kill2d(uint8_t** const array)
{
    for(int i = 0; i < map_ymax; i++)
        free(array[i]);
    free(array);
}

static void
print2d(uint8_t** const array)
{
    for(int i = 0; i < map_ymax; i++)
    {
        for(int j = 0; j < map_xmax; j++)
        {
            printf("%d", array[i][j]);
            putchar(' ');
        }
        putchar('\n');
    }
}

void
map_unload(void)
{
    kill2d(map_ceilings);
    kill2d(map_wallings);
    kill2d(map_floorings);
}

void
map_load(const char* path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // ymax
    line = getline(fp);
    sscanf(line, "%d", &map_ymax);
    free(line);
    // xmax
    line = getline(fp);
    sscanf(line, "%d", &map_xmax);
    free(line);
    // Map
    map_ceilings = gettile(fp);
    map_wallings = gettile(fp);
    map_floorings = gettile(fp);
    // Done
    print2d(map_ceilings);
    print2d(map_wallings);
    print2d(map_floorings);
    fclose(fp);
}
