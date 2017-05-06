#include "Calc.h"

Calc prealloc(const int res)
{
    Point* const wheres = (Point*) malloc(res * sizeof(*wheres));
    int* const moddings = (int*) malloc(res * sizeof(*moddings));
    const Calc calc = { wheres, moddings };
    return calc;
}

void abandon(const Calc calc)
{
    free(calc.wheres);
    free(calc.moddings);
}
