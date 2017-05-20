#include "World.h"

#include "Hero.h"
#include "Torch.h"
#include "Sprites.h"

static Hero themself(const Hero hero, const Sprites sprites, const Map map, const uint8_t* const key)
{
    Hero temp = console(hero, key);
    if(temp.consoling)
        return temp;
    temp = spin(temp, key);
    temp = move(temp, map.walling, key);
    temp = zoom(temp, key);
    temp = pick(temp, key);
    temp = save(temp, map, sprites, key);
    temp.torch = fade(temp.torch);
    if(scared(temp, sprites))
        temp.torch = flicker(temp.torch);
    return temp;
}

static void themselves(const Hero hero, const Sprites sprites, const Map map, const uint8_t* const key)
{
    rearrange(sprites, hero);
    grab(hero, sprites, key);
    shove(sprites);
    constrain(sprites, hero, map);
}

Hero sustain(const Hero hero, const Sprites sprites, const Map map, const uint8_t* const key)
{
    Hero temp = themself(hero, sprites, map, key);
    themselves(temp, sprites, map, key);
    return temp;
}
