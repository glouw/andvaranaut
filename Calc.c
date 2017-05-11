#include "Calc.h"
#include "Util.h"

Calc prealloc(const int res)
{
    Point* const wheres = toss(Point, res);
    int* const moddings = toss(int, res);
    const Calc calc = { wheres, moddings };
    return calc;
}

void abandon(const Calc calc)
{
    free(calc.wheres);
    free(calc.moddings);
}
