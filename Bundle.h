#pragma once

#include "Group.h"

typedef struct
{
    int a;
    int b;
    Group g;
}
Bundle;

// Bundle rasterer. Renders scanlines <a> to <b>.
int xbraster(void* const bundle);
