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
fresh(void)
{
    uint8_t** const array = malloc(map_y * sizeof(uint8_t*));
    for(int i = 0; i < map_y; i++)
        array[i] = NULL;
    for(int i = 0; i < map_y; i++)
        array[i] = malloc(map_x * sizeof(uint8_t));
    for(int i = 0; i < map_y; i++)
    for(int j = 0; j < map_x; j++)
        array[i][j] = 0;
    return array;
}

static uint8_t**
getparty(FILE* const fp)
{
    uint8_t** const array = fresh();
    for(int i = 0; i < map_y; i++)
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
kill(uint8_t** const array)
{
    for(int i = 0; i < map_y; i++)
        free(array[i]);
    free(array);
}

static void
print(uint8_t** const array)
{
    for(int i = 0; i < map_y; i++) {
    for(int j = 0; j < map_x; j++)
        printf("%d ", array[i][j]);
    putchar('\n');
    }
}

void
map_unload(void)
{
    kill(map_r);
    kill(map_c);
    kill(map_w);
    kill(map_f);
}

void
map_load(const char* path)
{
    FILE* fp = fopen(path, "r");
    char* line;
    // Map size
    line = getline(fp);
    sscanf(line, "%d %d", &map_x, &map_y);
    free(line);
    // Inside or outside
    int temp;
    line = getline(fp);
    sscanf(line, "%d", &temp);
    map_inside = (bool)temp;
    map_outside = !map_inside;
    free(line);
    // Map
    map_r = getparty(fp);
    map_c = getparty(fp);
    map_w = getparty(fp);
    map_f = getparty(fp);
    // Done
    printf("map size: x %d: y %d\n", map_x, map_y);
    printf("%s\n", map_inside ? "inside" : "outside");
    print(map_r);
    print(map_c);
    print(map_w);
    print(map_f);
    fclose(fp);
}
