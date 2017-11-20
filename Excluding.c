#include "Excluding.h"

#include <stdlib.h>

bool width(const Sprite* const sprite)
{
    return sprite->width == 0.0;
}

Sprite* find(const Excluding excluding, const Sprites sprites, const State state)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // Function call back exclusion.
        if(excluding(sprite))
            continue;
        if(sprite->state == state)
            return sprite;
    }
    return NULL;
}
