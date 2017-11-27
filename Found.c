#include "Found.h"

#include <stdlib.h>

int xgrabbed(const Sprite* const sprite)
{
    return sprite->state == GRABBED;
}

Sprite* find(const Found found, const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(found(sprite)) return sprite;
    }
    return NULL;
}
