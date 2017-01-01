#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* get_line(FILE* const fp)
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

static uint8_t** build(void)
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

static uint8_t** get_tile(FILE* const fp)
{
    uint8_t** const array = build();
    for(int i = 0; i < rows; i++)
    {
        char* const line = get_line(fp);
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

void unload_map(void)
{
    kill(ceilings);
    kill(wallings);
    kill(floorings);
}

void load_map(const char* path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // Rows
    line = get_line(fp);
    sscanf(line, "%d", &rows);
    free(line);
    // Columns
    line = get_line(fp);
    sscanf(line, "%d", &cols);
    free(line);
    // Map
    ceilings  = get_tile(fp);
    wallings  = get_tile(fp);
    floorings = get_tile(fp);
    // Done
    fclose(fp);
}
