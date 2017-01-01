#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* getln(FILE* const fp)
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

static uint8_t** heap(void)
{
    uint8_t** const array = malloc(rows * sizeof(uint8_t*));
    for(int i = 0; i < rows; i++)
        array[i] = NULL;
    for(int i = 0; i < rows; i++)
        array[i] = malloc(cols * sizeof(uint8_t));
    for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
        array[i][j] = 0;
    return array;
}

static uint8_t** get(FILE* const fp)
{
    uint8_t** const array = heap();
    for(int i = 0; i < rows; i++)
    {
        char* const line = getln(fp);
        const char* tile;
        int j = 0;
        for(char* temp = line; (tile = strtok(temp, " ")); temp = NULL)
            array[i][j++] = atoi(tile);
        free(line);
    }
    return array;
}

static void kill(uint8_t** const array)
{
    for(int i = 0; i < rows; i++)
        free(array[i]);
    free(array);
}

void map_load(const char* path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // Rows
    line = getln(fp);
    sscanf(line, "%d", &rows);
    free(line);
    // Cols
    line = getln(fp);
    sscanf(line, "%d", &cols);
    free(line);
    // The good stuff
    ceilings = get(fp);
    wallings = get(fp);
    floorings = get(fp);
    // Done
    fclose(fp);
}

void map_kill(void)
{
    kill(ceilings);
    kill(wallings);
    kill(floorings);
}
