#include "Sprites.h"

#include "util.h"

#include <stdlib.h>

Sprites wake(const char* const name)
{
    char* const path = strcon(name, ".sprites");
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    const Sprites sprites = {
        count
    };
    fclose(file);
    free(path);
    return sprites;
}
