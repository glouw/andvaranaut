#include "Attack.h"

Attack a_zero(void)
{
    static Attack attack;
    return attack;
}
