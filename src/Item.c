#include "Item.h"

#include "util.h"

#include <stdlib.h>

// All these names were created with Fantasy Name Generators:
// http://www.fantasynamegenerators.com

Item xzitem()
{
    static Item item;
    return item;
}

static Item ammo(Item it)
{
    it.cstr = "Ammo";

    switch(it.index)
    {
    case 0:
        it.name = "Silver Bullets";
        break;

    case 1:
        it.name = "Regular Bullets";
        break;

    case 2:
        it.name = "Poorman's Bullets";
        break;

    case 3:
        it.name = "Popcap Bullets";
        break;

    case 4:
        it.name = "Pearls";
        break;

    case 5:
        it.name = "River Pebbels";
        break;

    case 6:
        it.name = "Stone Bits";
        break;

    case 7:
        it.name = "Hunter's Bow";
        break;

    case 8:
        it.name = "Orcish Bow";
        break;

    case 9:
        it.name = "Peasant Bow";
        break;

    case 10:
        it.name = "Elvish Bow";
        break;

    case 11:
        it.name = "Sling";
        break;

    case 12:
        it.name = "Crossbow";
        break;

    case 13:
        it.name = "Mithril Arrows";
        break;

    case 14:
        it.name = "Iron Arrows";
        break;

    case 15:
        it.name = "Gold Tip Arrows";
        break;

    case 16:
        it.name = "Bone Arrows";
        break;

    case 17:
        it.name = "Adament Arrows";
        break;

    case 18:
        it.name = "Barbed Arrows";
        break;

    case 19:
        it.name = "Dart";
        break;

    case 20:
        it.name = "Shurikens";
        break;

    case 21:
        it.name = "Boomerang";
        break;

    case 22:
        it.name = "Dwarven One-Handed Cannon";
        break;

    case 23:
        it.name = "Dwarven Two-Handed Cannon";
        break;
    }

    return it;
}

static Item amulet(Item it)
{
    it.cstr = "Amulet";

    switch(it.index)
    {
    case 0:
        it.name = "The Emblem of Reincarnation";
        break;

    case 1:
        it.name = "The Aura of Endurance";
        break;

    case 2:
        it.name = "The Whisper of Panic";
        break;

    case 3:
        it.name = "The Fortified Secrets of Power";
        break;

    case 4:
        it.name = "The Enhanced Aspect of Auras";
        break;

    case 5:
        it.name = "The Mighty Mark of Hunger";
        break;

    case 6:
        it.name = "The Vampiric Hymn of Magical Defenses";
        break;

    case 7:
        it.name = "The Gift of Prime Loss";
        break;

    case 8:
        it.name = "The Emblem of Strengthened Storms";
        break;

    case 9:
        it.name = "The Hymn of Eternal Potency";
        break;

    case 10:
        it.name = "The Spark of Sight";
        break;

    case 11:
        it.name = "The Enchantment of Success";
        break;

    case 12:
        it.name = "The Grace of Haste";
        break;

    case 13:
        it.name = "The Fortified Charge of Cover";
        break;

    case 14:
        it.name = "The Lesser Bond of Cover";
        break;

    case 15:
        it.name = "The Brilliant Infusion of Danger";
        break;

    case 16:
        it.name = "The Light Seal of Disbelief";
        break;
    }

    return it;
}

static Item armor(Item it)
{
    it.cstr = "Armor";

    switch(it.index)
    {
    case 0:
        it.name = "Bronze Chainmail of Immortal Souls";
        break;

    case 1:
        it.name = "Bronze Chainmail of Divine Bloodlust";
        break;

    case 2:
        it.name = "Mithril Chainmail of Chestguard";
        break;

    case 3:
        it.name = "Adamant Chainmail of Pride's Fall";
        break;

    case 4:
        it.name = "Bronze Chainmail of the Lone Wolf";
        break;

    case 5:
        it.name = "Mithril Chainmail of Delusions";
        break;

    case 6:
        it.name = "Gold Chainmail of Relentless Voices";
        break;

    case 7:
        it.name = "Mithril Chainmail of Recoil";
        break;

    case 8:
        it.name = "Charcoal Chainmail of Broken Bones";
        break;

    case 9:
        it.name = "Steel Chainmail of Riddles";
        break;

    case 10:
        it.name = "Adamant Chainmail of Unholy Blight";
        break;

    case 11:
        it.name = "Poly Chainmail of Dark Magic";
        break;

    case 12:
        it.name = "Copper Chain Rings";
        break;

    case 13:
        it.name = "Bronze Chain Rings";
        break;

    case 14:
        it.name = "Mithril Chain Rings";
        break;

    case 15:
        it.name = "Adamant Chain Rings";
        break;

    case 16:
        it.name = "Copper Chain Rings";
        break;

    case 17:
        it.name = "Mithril Chain Rings";
        break;

    case 18:
        it.name = "Gold Chain Rings";
        break;

    case 19:
        it.name = "Mithril Chain Rings";
        break;

    case 20:
        it.name = "Charcoal Chain Rings";
        break;

    case 21:
        it.name = "Steel Chain Rings";
        break;

    case 22:
        it.name = "Adamant Chain Rings";
        break;

    case 23:
        it.name = "Poly Chain Rings";
        break;

    case 24:
        it.name = "Greatcloak of Blind Whispers";
        break;

    case 25:
        it.name = "Drape of Blessed Glory";
        break;

    case 26:
        it.name = "Wool Mantle of Distant Powers";
        break;

    case 27:
        it.name = "Possessed Scaled Shroud";
        break;

    case 28:
        it.name = "Cloth Greatcloak of Haunted Justice";
        break;

    case 29:
        it.name = "Exile Embroided Wrap";
        break;

    case 30:
        it.name = "Warlord's Padded Greatcloak";
        break;

    case 31:
        it.name = "Lightning Mantle of Secrets";
        break;

    case 32:
        it.name = "Dragon Shroud of the Talon";
        break;

    case 33:
        it.name = "Doom's Wool Drape";
        break;

    case 34:
        it.name = "Piece of Cloth";
        break;

    case 35:
        it.name = "Dirty Rags";
        break;

    case 36:
        it.name = "Copper Chestguard of Fleeting Kings";
        break;

    case 37:
        it.name = "Bronze Chestguard of Hallowed Wars";
        break;

    case 38:
        it.name = "Steel Breastplate";
        break;

    case 39:
        it.name = "Studded Steel Breastplate";
        break;

    case 40:
        it.name = "Mithril Chestgaurd of Conquered Worlds";
        break;

    case 41:
        it.name = "Steel Plated Cuirass";
        break;

    case 42:
        it.name = "Heavy Steel Plated Cuirass";
        break;

    case 43:
        it.name = "Storm-Forged Steel Cuirass of the Gods";
        break;

    case 44:
        it.name = "Common Clothes";
        break;

    case 45:
        it.name = "Kimono of the Apprentice";
        break;

    case 46:
        it.name = "Kimono of Festivities";
        break;

    case 47:
        it.name = "Kimono of the Western Sea";
        break;

    case 48:
        it.name = "Kimono of the Fisherman";
        break;

    case 49:
        it.name = "Common Headscarf";
        break;

    case 50:
        it.name = "Desert Headscarf";
        break;

    case 51:
        it.name = "Red Cape of Riding";
        break;

    case 52:
        it.name = "Iron Chainmail";
        break;

    case 53:
        it.name = "Gold Chainmail";
        break;

    case 54:
        it.name = "Steel Chaimail";
        break;

    case 55:
        it.name = "Bronze Chainmail";
        break;

    case 56:
        it.name = "Charcoal Chainmail";
        break;

    case 57:
        it.name = "Wooden Chest Armor";
        break;

    case 58:
        it.name = "Hide Skin";
        break;
    }

    return it;
}

static Item book(Item it)
{
    it.cstr = "Book";

    switch(it.index)
    {
    case 0:
        it.name = "Duke Of The Nation";
        break;

    case 1:
        it.name = "Criminal Of The Night";
        break;

    case 2:
        it.name = "Humans Without Duty";
        break;

    case 3:
        it.name = "Descendants Of The Day";
        break;

    case 4:
        it.name = "Slaves And Friends";
        break;

    case 5:
        it.name = "Defenders And Swindlers";
        break;

    case 6:
        it.name = "Ruins With Money";
        break;

    case 7:
        it.name = "Sword Of History";
        break;

    case 8:
        it.name = "Dead At The Beginning";
        break;

    case 9:
        it.name = "Cleaning Up My Destiny";
        break;

    case 10:
        it.name = "Defender With Pride";
        break;

    case 11:
        it.name = "Human Without Courage";
        break;

    case 12:
        it.name = "Blacksmiths Of Hell";
        break;

    case 13:
        it.name = "Invaders Of The Great";
        break;

    case 14:
        it.name = "Slaves And Phantoms";
        break;

    case 15:
        it.name = "Heroes And Heirs";
        break;

    case 16:
        it.name = "Vengeance Of Heaven";
        break;

    case 17:
        it.name = "Culmination Of Fire";
        break;

    case 18:
        it.name = "Commanding The Depths";
        break;

    case 19:
        it.name = "Searching In Nightmares";
        break;

    case 20:
        it.name = "Butcher Of The Ancestors";
        break;

    case 21:
        it.name = "Priestess Of The Great";
        break;

    case 22:
        it.name = "Hunters Without A Goal";
        break;

    case 23:
        it.name = "Wolves Without Courage";
        break;

    case 24:
        it.name = "Bandits And Foreigners";
        break;

    case 25:
        it.name = "Strangers And Foes";
        break;

    case 26:
        it.name = "Crossbow Of The Ancestors";
        break;

    case 27:
        it.name = "Luck Of Dread";
        break;

    case 28:
        it.name = "Challenging The Immortals";
        break;

    case 29:
        it.name = "Dwelling In The Emperor";
        break;

    case 30:
        it.name = "Criminal Of Utopia";
        break;

    case 31:
        it.name = "Friend Without Time";
        break;

    case 32:
        it.name = "Enemies With Silver";
        break;

    case 33:
        it.name = "Priests Of The Plague";
        break;

    case 34:
        it.name = "Giants And Heirs";
        break;

    case 35:
        it.name = "Hunters And Mice";
        break;

    case 36:
        it.name = "Nation Without Flaws";
        break;

    case 37:
        it.name = "Revenge Of The Frontline";
        break;

    case 38:
        it.name = "Running In The Hunter";
        break;

    case 39:
        it.name = "Rescue At The Mist";
        break;

    case 40:
        it.name = "Gangster With Immortality";
        break;

    case 41:
        it.name = "Blacksmith Of Freedom";
        break;

    case 42:
        it.name = "Wives Without Direction";
        break;

    case 43:
        it.name = "Women With Silver";
        break;

    case 44:
        it.name = "Spies And Strangers";
        break;

    case 45:
        it.name = "Thieves And Kings";
        break;

    case 46:
        it.name = "World Of The Ancients";
        break;

    case 47:
        it.name = "Effect Of Hope";
        break;

    case 48:
        it.name = "Deceiving The Forest";
        break;

    case 49:
        it.name = "Remember The Forest";
        break;

    case 50:
        it.name = "Woman Of Glory";
        break;

    case 51:
        it.name = "Agent With Silver";
        break;

    case 52:
        it.name = "Foes Of The River";
        break;

    case 53:
        it.name = "Robots With Money";
        break;

    case 54:
        it.name = "Women And Criminals";
        break;

    case 55:
        it.name = "Officers And Rats";
        break;

    case 56:
        it.name = "Country Of Glory";
        break;

    case 57:
        it.name = "Will Of My Imagination";
        break;

    case 58:
        it.name = "Traces In The Jungle";
        break;

    case 59:
        it.name = "Dancing In The Animals";
        break;

    case 60:
        it.name = "Assassin Of Stone";
        break;

    case 61:
        it.name = "Criminal Of Earth";
        break;

    case 62:
        it.name = "Blacksmiths Of The Stockades";
        break;

    case 63:
        it.name = "Humans With Pride";
        break;

    case 64:
        it.name = "Owls And Dogs";
        break;
    }

    return it;
}

static Item boot(Item it)
{
    it.cstr = "Boot";

    switch(it.index)
    {
    case 0:
        it.name = "Footpads of Binding Lands";
        break;

    case 1:
        it.name = "Sabatons of Conquered Damnation";
        break;

    case 2:
        it.name = "Crazed Bronzed Greatboots";
        break;

    case 3:
        it.name = "Grieving Adamantite Sabatons";
        break;

    case 4:
        it.name = "Ebon Skeletal Greaves";
        break;

    case 5:
        it.name = "Iron Walkers of Hallowed Souls";
        break;

    case 6:
        it.name = "Thunderfury Skeletal Greaves";
        break;

    case 7:
        it.name = "Hollow Feet of Nature";
        break;

    case 8:
        it.name = "Challenger's Quilted Boots";
        break;

    case 9:
        it.name = "Sprinters of the Champion";
        break;
    }

    return it;
}

static Item chest(Item it)
{
    it.cstr = "Chest";

    switch(it.index)
    {
    case 0:
        it.name = "Small Locked Chest";
        break;

    case 1:
        it.name = "Large Locked Chest";
        break;

    case 2:
        it.name = "Broken Chest";
        break;

    case 3:
        it.name = "Small Locked Safe";
        break;

    case 4:
        it.name = "Large Locked Safe";
        break;

    case 5:
        it.name = "Broken Safe";
        break;

    case 6:
        it.name = "Broken Safe";
        break;

    case 7:
        it.name = "Crystal Enclosure";
        break;

    case 8:
        it.name = "Broken Crystal Enclosure";
        break;

    case 9:
        it.name = "Gold Pouch";
        break;

    case 10:
        it.name = "Mystic Gold Pouch";
        break;

    case 11:
        it.name = "Expensive Gold Pouch";
        break;

    case 12:
        it.name = "Hungry Gold Pouch";
        break;

    case 13:
        it.name = "Barrel of Common Goods";
        break;

    case 14:
        it.name = "Barrel of Rare Goods";
        break;

    case 15:
        it.name = "Broken Barrel of Common Goods";
        break;

    case 16:
        it.name = "Broken Barrel of Expensive Goods";
        break;
    }

    return it;
}

static Item flesh(Item it)
{
    it.cstr = "Flesh";

    switch(it.index)
    {
    }

    return it;
}

static Item food(Item it)
{
    it.cstr = "Food";

    switch(it.index)
    {
    case 0:
        it.name = "Cramp Lion";
        break;

    case 1:
        it.name = "Stag’s Ear";
        break;

    case 2:
        it.name = "Luminescent Lichen";
        break;

    case 3:
        it.name = "Orange Jelly Drop";
        break;

    case 4:
        it.name = "Mossy Hair Lichen";
        break;

    case 5:
        it.name = "Scaly Swamp Russula";
        break;

    case 6:
        it.name = "Hypoxylon Lichen";
        break;

    case 7:
        it.name = "Questionable Felt Lichen";
        break;

    case 8:
        it.name = "Marasmiellus Spindles";
        break;

    case 9:
        it.name = "Powdered Pored Bolete";
        break;

    case 10:
        it.name = "Fly Blewit";
        break;

    case 11:
        it.name = "Snow-white Egg Bird's Nest Fungus";
        break;

    case 12:
        it.name = "Viscid Destroyer Fungi";
        break;

    case 13:
        it.name = "Tumbling Waxcap";
        break;

    case 14:
        it.name = "Cloudy Widow";
        break;

    case 15:
        it.name = "Green-belted Mans Fingers";
        break;

    case 16:
        it.name = "Hygroscopic Waxycap";
        break;

    case 17:
        it.name = "Onger";
        break;

    case 18:
        it.name = "Nilery";
        break;

    case 19:
        it.name = "Kime";
        break;

    case 20:
        it.name = "Sun Guanabana";
        break;

    case 21:
        it.name = "Iggeomquat";
        break;

    case 22:
        it.name = "River Mango";
        break;

    case 23:
        it.name = "Klorang";
        break;

    case 24:
        it.name = "Sun Cawesh";
        break;

    case 25:
        it.name = "Summer Babaco";
        break;

    case 26:
        it.name = "Dragon Bean";
        break;

    case 27:
        it.name = "Kastrialon";
        break;

    case 28:
        it.name = "Teobaco";
        break;

    case 29:
        it.name = "Wrurreorang";
        break;

    case 30:
        it.name = "Eale";
        break;

    case 31:
        it.name = "Ogliaguaro";
        break;

    case 32:
        it.name = "Natine";
        break;

    case 33:
        it.name = "Lembas";
        break;

    case 34:
        it.name = "Lembas Delight";
        break;

    case 35:
        it.name = "Klegriariac Bonbons";
        break;

    case 36:
        it.name = "Crown Honey Snacks";
        break;

    case 37:
        it.name = "Barrel of Mead";
        break;

    case 38:
        it.name = "Lump of Honey Sugar";
        break;

    case 39:
        it.name = "River Mango Pie";
        break;

    case 40:
        it.name = "Natine Chocolate Bar";
        break;

    case 41:
        it.name = "Bruggearind Fortune Cake";
        break;

    case 42:
        it.name = "Slossimelo Sweet Bun";
        break;
    }

    return it;
}

static Item glove(Item it)
{
    it.cstr = "Glove";

    switch(it.index)
    {
    }

    return it;
}

static Item hat(Item it)
{
    it.cstr = "Hat";

    switch(it.index)
    {
    }

    return it;
}

static Item key(Item it)
{
    it.cstr = "Key";

    switch(it.index)
    {
    }

    return it;
}

static Item light(Item it)
{
    it.cstr = "Light";

    switch(it.index)
    {
    }

    return it;
}

static Item longwep(Item it)
{
    it.cstr = "Longwep";

    switch(it.index)
    {
    }

    return it;
}

static Item medwep(Item it)
{
    it.cstr = "Medwep";

    switch(it.index)
    {
    }

    return it;
}

static Item money(Item it)
{
    it.cstr = "Money";

    switch(it.index)
    {
    }

    return it;
}

static Item music(Item it)
{
    it.cstr = "Music";

    switch(it.index)
    {
    }

    return it;
}

static Item potion(Item it)
{
    it.cstr = "Potion";

    switch(it.index)
    {
    }

    return it;
}

static Item ring(Item it)
{
    it.cstr = "Ring";

    switch(it.index)
    {
    }

    return it;
}

static Item rock(Item it)
{
    it.cstr = "Rock";

    switch(it.index)
    {
    }

    return it;
}

static Item scroll(Item it)
{
    it.cstr = "Scroll";

    switch(it.index)
    {
    case  0: it.name = "Odal"  , it.desc = "Home";    break;
    case  1: it.name = "Daeg"  , it.desc = "Day";     break;
    case  2: it.name = "Jera"  , it.desc = "Harvest"; break;
    case  3: it.name = "Lagu"  , it.desc = "Water";   break;
    case  4: it.name = "Man"   , it.desc = "Human";   break;
    case  5: it.name = "Eh"    , it.desc = "Horse";   break;
    case  6: it.name = "Beorc" , it.desc = "Puriry";  break;
    case  7: it.name = "Tyr"   , it.desc = "Arrow";   break;
    case  8: it.name = "Sigel" , it.desc = "Sun";     break;
    case  9: it.name = "Elhaz" , it.desc = "Elk";     break;
    case 10: it.name = "Peorth", it.desc = "Dice";    break;
    case 11: it.name = "Eoh"   , it.desc = "Yew";     break;
    case 12: it.name = "Ng"    , it.desc = "Hero";    break;
    case 13: it.name = "Is"    , it.desc = "Ice";     break;
    case 14: it.name = "Nyd"   , it.desc = "Need";    break;
    case 15: it.name = "Hagal" , it.desc = "Hail";    break;
    case 16: it.name = "Wynn"  , it.desc = "Wind";    break;
    case 17: it.name = "Gifu"  , it.desc = "Gift";    break;
    case 18: it.name = "Ken"   , it.desc = "Wood";    break;
    case 19: it.name = "Rad"   , it.desc = "Wheel";   break;
    case 20: it.name = "As"    , it.desc = "God";     break;
    case 21: it.name = "Thorn" , it.desc = "Giant";   break;
    case 22: it.name = "Ur"    , it.desc = "Ox";      break;
    case 23: it.name = "Feoh"  , it.desc = "Cattle";  break;
    }

    return it;
}

static Item shield(Item it)
{
    it.cstr = "Shield";

    switch(it.index)
    {
    }

    return it;
}

static Item shortwep(Item it)
{
    it.cstr = "Shortwep";

    switch(it.index)
    {
    }

    return it;
}

static Item tool(Item it)
{
    it.cstr = "Tool";

    switch(it.index)
    {
    }

    return it;
}

static Item wand(Item it)
{
    it.cstr = "Wand";

    switch(it.index)
    {
    }

    return it;
}

static Item none(Item it)
{
    it.cstr = "None";

    switch(it.index)
    {
    }

    return it;
}

Item xitnew(const Classification c, const int index)
{
    Item it = xzitem();
    it.index = index;
    it.c = c;
    it.period = 0.05f;
    it.amplitude = 360.0f;
    it.damage = 25.0f;
    it.hurts = 3;

    switch(c)
    {
    case     AMMO: return     ammo(it);
    case   AMULET: return   amulet(it);
    case    ARMOR: return    armor(it);
    case     BOOK: return     book(it);
    case     BOOT: return     boot(it);
    case    CHEST: return    chest(it);
    case    FLESH: return    flesh(it);
    case     FOOD: return     food(it);
    case    GLOVE: return    glove(it);
    case      HAT: return      hat(it);
    case      KEY: return      key(it);
    case    LIGHT: return    light(it);
    case  LONGWEP: return  longwep(it);
    case   MEDWEP: return   medwep(it);
    case    MONEY: return    money(it);
    case    MUSIC: return    music(it);
    case   POTION: return   potion(it);
    case     RING: return     ring(it);
    case     ROCK: return     rock(it);
    case   SCROLL: return   scroll(it);
    case   SHIELD: return   shield(it);
    case SHORTWEP: return shortwep(it);
    case     TOOL: return     tool(it);
    case     WAND: return     wand(it);
    case     NONE: return     none(it);

    default:
        return it;
    }
}

Item xitrand()
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(c);
    return xitnew(c, index);
}
