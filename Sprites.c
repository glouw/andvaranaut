#include "Sprites.h"

#include "util.h"

#include <stdlib.h>

Sprites wake(const char* const path)
{
    FILE* const file = fopen(path, "r");
    const int count = lns(file);
    const Sprites sprites = {
        count
    };
    fclose(file);
    return sprites;
}
