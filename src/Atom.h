#pragma once

#include "Field.h"

//
// Notice y and x are in reverse here, contrary to the Point type.
//

typedef struct
{
    int y;
    int x;
    float val;
}
Atom;

Atom a_materialize(const Field, const int y, const int x);
