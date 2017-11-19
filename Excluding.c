#include "Excluding.h"

#include <stdlib.h>

bool width(const Sprite* const sprite)
{
    return sprite->width == 0.0;
}

Sprite* find(const Sprites sprites, const State state, Excluding excluding)
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
