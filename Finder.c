#include "Finder.h"

#include <stdlib.h>

Sprite* find(const Finder finder, const Sprites sprites)
{
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(finder(sprite)) return sprite;
    }
    return NULL;
}
