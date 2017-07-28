#include "Attack.h"

#include "Util.h"
#include "Hero.h"
#include "Point.h"

extern Attack nothing()
{
    Attack attack;
    zero(attack);
    return attack;
}

extern Attack swing(const Hero hero, const Point vect)
{
    Attack attack = nothing();
    attack.vect = vect;
    attack.power = mag(vect);
    attack.type.swing = true;
    attack.reach = hero.arm + 0.75;
    attack.tip = touch(hero, attack.reach);
    attack.area = 2 * attack.reach;
    return attack;
}

extern Attack melee(const Hero hero, const Input input)
{
    Attack attack = nothing();
    if(hero.consoling)
        return attack;
    if(hero.inserting)
        return attack;
    // Attack vector (persistent across function calls)
    static Point vect;
    // Held down mouse button, grow attack vector
    if(input.l)
    {
        vect.x += input.dx;
        vect.y += input.dy;
    }
    // Mouse button let go, calculate attack power from attack vector
    else
    {
        // Attack was a swing if there was weapon movement
        if(vect.x != 0 && vect.y != 0) attack = swing(hero, vect);
        // Reset attack vector as the reset vector
        // is persistent across function calls
        zero(vect);
    }
    return attack;
}
