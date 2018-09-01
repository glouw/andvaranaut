#include "Attack.h"

Attack xzattack(void)
{
    static Attack attack;
    return attack;
}
