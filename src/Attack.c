#include "Attack.h"

Attack xzattack()
{
    static Attack attack;
    return attack;
}
