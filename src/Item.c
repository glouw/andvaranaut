#include "Item.h"

#include "util.h"

#include <assert.h>
#include <stdlib.h>

static Item ammo(const int index)
{
    static const Item ammos[] = {
        {  0, AMMO, "Ammo", "Silver Bullets"   , "", 25.0f, 1, 360.0f, 0.05f },
        {  1, AMMO, "Ammo", "Regular Bullets"  , "", 25.0f, 1, 360.0f, 0.05f },
        {  2, AMMO, "Ammo", "Poorman's Bullets", "", 25.0f, 1, 360.0f, 0.05f },
        {  3, AMMO, "Ammo", "Popcap Bullets"   , "", 25.0f, 1, 360.0f, 0.05f },
        {  4, AMMO, "Ammo", "Pearls"           , "", 25.0f, 1, 360.0f, 0.05f },
        {  5, AMMO, "Ammo", "River Pebbels"    , "", 25.0f, 1, 360.0f, 0.05f },
        {  6, AMMO, "Ammo", "Stone Bits"       , "", 25.0f, 1, 360.0f, 0.05f },
        {  7, AMMO, "Ammo", "Hunter's Bow"     , "", 25.0f, 1, 360.0f, 0.05f },
        {  8, AMMO, "Ammo", "Orcish Bow"       , "", 25.0f, 1, 360.0f, 0.05f },
        {  9, AMMO, "Ammo", "Peasant Bow"      , "", 25.0f, 1, 360.0f, 0.05f },
        { 10, AMMO, "Ammo", "Elvish Bow"       , "", 25.0f, 1, 360.0f, 0.05f },
        { 11, AMMO, "Ammo", "Sling"            , "", 25.0f, 1, 360.0f, 0.05f },
        { 12, AMMO, "Ammo", "Crossbow"         , "", 25.0f, 1, 360.0f, 0.05f },
        { 13, AMMO, "Ammo", "Mithril Arrows"   , "", 25.0f, 1, 360.0f, 0.05f },
        { 14, AMMO, "Ammo", "Iron Arrows"      , "", 25.0f, 1, 360.0f, 0.05f },
        { 15, AMMO, "Ammo", "Gold Tip Arrows"  , "", 25.0f, 1, 360.0f, 0.05f },
        { 16, AMMO, "Ammo", "Bone Arrows"      , "", 25.0f, 1, 360.0f, 0.05f },
        { 17, AMMO, "Ammo", "Adament Arrows"   , "", 25.0f, 1, 360.0f, 0.05f },
        { 18, AMMO, "Ammo", "Barbed Arrows"    , "", 25.0f, 1, 360.0f, 0.05f },
        { 19, AMMO, "Ammo", "Dart"             , "", 25.0f, 1, 360.0f, 0.05f },
        { 20, AMMO, "Ammo", "Shurikens"        , "", 25.0f, 1, 360.0f, 0.05f },
        { 21, AMMO, "Ammo", "Boomerang"        , "", 25.0f, 1, 360.0f, 0.05f },
        { 22, AMMO, "Ammo", "One-Handed Cannon", "", 25.0f, 1, 360.0f, 0.05f },
        { 23, AMMO, "Ammo", "Two-Handed Cannon", "", 25.0f, 1, 360.0f, 0.05f },
    };
    assert(ammos[index].index == index);
    return ammos[index];
}

static Item amulet(const int index)
{
    static const Item amulets[] = {
        {  0, AMULET, "Amulet", "Emblem of Reincarnation"          , "", 25.0f, 1, 360.f, 0.5f },
        {  1, AMULET, "Amulet", "Aura of Endurance"                , "", 25.0f, 1, 360.f, 0.5f },
        {  2, AMULET, "Amulet", "Whisper of Panic"                 , "", 25.0f, 1, 360.f, 0.5f },
        {  3, AMULET, "Amulet", "Fortified Secrets of Power"       , "", 25.0f, 1, 360.f, 0.5f },
        {  4, AMULET, "Amulet", "Enhanced Aspect of Auras"         , "", 25.0f, 1, 360.f, 0.5f },
        {  5, AMULET, "Amulet", "Mighty Mark of Hunger"            , "", 25.0f, 1, 360.f, 0.5f },
        {  6, AMULET, "Amulet", "Vampiric Hymn of Magical Defenses", "", 25.0f, 1, 360.f, 0.5f },
        {  7, AMULET, "Amulet", "Gift of Prime Loss"               , "", 25.0f, 1, 360.f, 0.5f },
        {  8, AMULET, "Amulet", "Emblem of Strengthened Storms"    , "", 25.0f, 1, 360.f, 0.5f },
        {  9, AMULET, "Amulet", "Hymn of Eternal Potency"          , "", 25.0f, 1, 360.f, 0.5f },
        { 10, AMULET, "Amulet", "Spark of Sight"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 11, AMULET, "Amulet", "Enchantment of Success"           , "", 25.0f, 1, 360.f, 0.5f },
        { 12, AMULET, "Amulet", "Grace of Haste"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 13, AMULET, "Amulet", "Fortified Charge of Cover"        , "", 25.0f, 1, 360.f, 0.5f },
        { 14, AMULET, "Amulet", "Lesser Bond of Cover"             , "", 25.0f, 1, 360.f, 0.5f },
        { 15, AMULET, "Amulet", "Brilliant Infusion of Danger"     , "", 25.0f, 1, 360.f, 0.5f },
        { 16, AMULET, "Amulet", "Light Seal of Disbelief"          , "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(amulets[index].index == index);
    return amulets[index];
}

static Item armor(const int index)
{
    static const Item armors[] = {
        {  0, ARMOR, "Armor", "Bronze Chainmail of Immortal Souls"     , "", 25.0f, 1, 360.f, 0.5f },
        {  1, ARMOR, "Armor", "Bronze Chainmail of Divine Bloodlust"   , "", 25.0f, 1, 360.f, 0.5f },
        {  2, ARMOR, "Armor", "Mithril Chainmail of Chestguard"        , "", 25.0f, 1, 360.f, 0.5f },
        {  3, ARMOR, "Armor", "Adamant Chainmail of Pride's Fall"      , "", 25.0f, 1, 360.f, 0.5f },
        {  4, ARMOR, "Armor", "Bronze Chainmail of the Lone Wolf"      , "", 25.0f, 1, 360.f, 0.5f },
        {  5, ARMOR, "Armor", "Mithril Chainmail of Delusions"         , "", 25.0f, 1, 360.f, 0.5f },
        {  6, ARMOR, "Armor", "Gold Chainmail of Relentless Voices"    , "", 25.0f, 1, 360.f, 0.5f },
        {  7, ARMOR, "Armor", "Mithril Chainmail of Recoil"            , "", 25.0f, 1, 360.f, 0.5f },
        {  8, ARMOR, "Armor", "Charcoal Chainmail of Broken Bones"     , "", 25.0f, 1, 360.f, 0.5f },
        {  9, ARMOR, "Armor", "Steel Chainmail of Riddles"             , "", 25.0f, 1, 360.f, 0.5f },
        { 10, ARMOR, "Armor", "Adamant Chainmail of Unholy Blight"     , "", 25.0f, 1, 360.f, 0.5f },
        { 11, ARMOR, "Armor", "Poly Chainmail of Dark Magic"           , "", 25.0f, 1, 360.f, 0.5f },
        { 12, ARMOR, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 13, ARMOR, "Armor", "Bronze Chain Rings"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 14, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 15, ARMOR, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 16, ARMOR, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 17, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 18, ARMOR, "Armor", "Gold Chain Rings"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 19, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 20, ARMOR, "Armor", "Charcoal Chain Rings"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 21, ARMOR, "Armor", "Steel Chain Rings"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 22, ARMOR, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 23, ARMOR, "Armor", "Poly Chain Rings"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 24, ARMOR, "Armor", "Greatcloak of Blind Whispers"           , "", 25.0f, 1, 360.f, 0.5f },
        { 25, ARMOR, "Armor", "Drape of Blessed Glory"                 , "", 25.0f, 1, 360.f, 0.5f },
        { 26, ARMOR, "Armor", "Wool Mantle of Distant Powers"          , "", 25.0f, 1, 360.f, 0.5f },
        { 27, ARMOR, "Armor", "Possessed Scaled Shroud"                , "", 25.0f, 1, 360.f, 0.5f },
        { 28, ARMOR, "Armor", "Cloth Greatcloak of Haunted Justice"    , "", 25.0f, 1, 360.f, 0.5f },
        { 29, ARMOR, "Armor", "Exile Embroided Wrap"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 30, ARMOR, "Armor", "Warlord's Padded Greatcloak"            , "", 25.0f, 1, 360.f, 0.5f },
        { 31, ARMOR, "Armor", "Lightning Mantle of Secrets"            , "", 25.0f, 1, 360.f, 0.5f },
        { 32, ARMOR, "Armor", "Dragon Shroud of the Talon"             , "", 25.0f, 1, 360.f, 0.5f },
        { 33, ARMOR, "Armor", "Doom's Wool Drape"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 34, ARMOR, "Armor", "Piece of Cloth"                         , "", 25.0f, 1, 360.f, 0.5f },
        { 35, ARMOR, "Armor", "Dirty Rags"                             , "", 25.0f, 1, 360.f, 0.5f },
        { 36, ARMOR, "Armor", "Copper Chestguard of Fleeting Kings"    , "", 25.0f, 1, 360.f, 0.5f },
        { 37, ARMOR, "Armor", "Bronze Chestguard of Hallowed Wars"     , "", 25.0f, 1, 360.f, 0.5f },
        { 38, ARMOR, "Armor", "Steel Breastplate"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 39, ARMOR, "Armor", "Studded Steel Breastplate"              , "", 25.0f, 1, 360.f, 0.5f },
        { 40, ARMOR, "Armor", "Mithril Chestgaurd of Conquered Worlds" , "", 25.0f, 1, 360.f, 0.5f },
        { 41, ARMOR, "Armor", "Steel Plated Cuirass"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 42, ARMOR, "Armor", "Heavy Steel Plated Cuirass"             , "", 25.0f, 1, 360.f, 0.5f },
        { 43, ARMOR, "Armor", "Storm-Forged Steel Cuirass of the Gods" , "", 25.0f, 1, 360.f, 0.5f },
        { 44, ARMOR, "Armor", "Common Clothes"                         , "", 25.0f, 1, 360.f, 0.5f },
        { 45, ARMOR, "Armor", "Kimono of the Apprentice"               , "", 25.0f, 1, 360.f, 0.5f },
        { 46, ARMOR, "Armor", "Kimono of Festivities"                  , "", 25.0f, 1, 360.f, 0.5f },
        { 47, ARMOR, "Armor", "Kimono of the Western Sea"              , "", 25.0f, 1, 360.f, 0.5f },
        { 48, ARMOR, "Armor", "Kimono of the Fisherman"                , "", 25.0f, 1, 360.f, 0.5f },
        { 49, ARMOR, "Armor", "Common Headscarf"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 50, ARMOR, "Armor", "Desert Headscarf"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 51, ARMOR, "Armor", "Red Cape of Riding"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 52, ARMOR, "Armor", "Iron Chainmail"                         , "", 25.0f, 1, 360.f, 0.5f },
        { 53, ARMOR, "Armor", "Gold Chainmail"                         , "", 25.0f, 1, 360.f, 0.5f },
        { 54, ARMOR, "Armor", "Steel Chaimail"                         , "", 25.0f, 1, 360.f, 0.5f },
        { 55, ARMOR, "Armor", "Bronze Chainmail"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 56, ARMOR, "Armor", "Charcoal Chainmail"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 57, ARMOR, "Armor", "Wooden Chest Armor"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 58, ARMOR, "Armor", "Hide Skin"                              , "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(armors[index].index == index);
    return armors[index];
}

static Item book(const int index)
{
    static const Item books[] = {
        {  0, BOOK, "Book", "Duke Of The Nation"          , "", 25.0f, 1, 360.f, 0.5f },
        {  1, BOOK, "Book", "Criminal Of The Night"       , "", 25.0f, 1, 360.f, 0.5f },
        {  2, BOOK, "Book", "Humans Without Duty"         , "", 25.0f, 1, 360.f, 0.5f },
        {  3, BOOK, "Book", "Descendants Of The Day"      , "", 25.0f, 1, 360.f, 0.5f },
        {  4, BOOK, "Book", "Slaves And Friends"          , "", 25.0f, 1, 360.f, 0.5f },
        {  5, BOOK, "Book", "Defenders And Swindlers"     , "", 25.0f, 1, 360.f, 0.5f },
        {  6, BOOK, "Book", "Ruins With Money"            , "", 25.0f, 1, 360.f, 0.5f },
        {  7, BOOK, "Book", "Sword Of History"            , "", 25.0f, 1, 360.f, 0.5f },
        {  8, BOOK, "Book", "Dead At The Beginning"       , "", 25.0f, 1, 360.f, 0.5f },
        {  9, BOOK, "Book", "Cleaning Up My Destiny"      , "", 25.0f, 1, 360.f, 0.5f },
        { 10, BOOK, "Book", "Defender With Pride"         , "", 25.0f, 1, 360.f, 0.5f },
        { 11, BOOK, "Book", "Human Without Courage"       , "", 25.0f, 1, 360.f, 0.5f },
        { 12, BOOK, "Book", "Blacksmiths Of Hell"         , "", 25.0f, 1, 360.f, 0.5f },
        { 13, BOOK, "Book", "Invaders Of The Great"       , "", 25.0f, 1, 360.f, 0.5f },
        { 14, BOOK, "Book", "Slaves And Phantoms"         , "", 25.0f, 1, 360.f, 0.5f },
        { 15, BOOK, "Book", "Heroes And Heirs"            , "", 25.0f, 1, 360.f, 0.5f },
        { 16, BOOK, "Book", "Vengeance Of Heaven"         , "", 25.0f, 1, 360.f, 0.5f },
        { 17, BOOK, "Book", "Culmination Of Fire"         , "", 25.0f, 1, 360.f, 0.5f },
        { 18, BOOK, "Book", "Commanding The Depths"       , "", 25.0f, 1, 360.f, 0.5f },
        { 19, BOOK, "Book", "Searching In Nightmares"     , "", 25.0f, 1, 360.f, 0.5f },
        { 20, BOOK, "Book", "Butcher Of The Ancestors"    , "", 25.0f, 1, 360.f, 0.5f },
        { 21, BOOK, "Book", "Priestess Of The Great"      , "", 25.0f, 1, 360.f, 0.5f },
        { 22, BOOK, "Book", "Hunters Without A Goal"      , "", 25.0f, 1, 360.f, 0.5f },
        { 23, BOOK, "Book", "Wolves Without Courage"      , "", 25.0f, 1, 360.f, 0.5f },
        { 24, BOOK, "Book", "Bandits And Foreigners"      , "", 25.0f, 1, 360.f, 0.5f },
        { 25, BOOK, "Book", "Strangers And Foes"          , "", 25.0f, 1, 360.f, 0.5f },
        { 26, BOOK, "Book", "Crossbow Of The Ancestors"   , "", 25.0f, 1, 360.f, 0.5f },
        { 27, BOOK, "Book", "Luck Of Dread"               , "", 25.0f, 1, 360.f, 0.5f },
        { 28, BOOK, "Book", "Challenging The Immortals"   , "", 25.0f, 1, 360.f, 0.5f },
        { 29, BOOK, "Book", "Dwelling In The Emperor"     , "", 25.0f, 1, 360.f, 0.5f },
        { 30, BOOK, "Book", "Criminal Of Utopia"          , "", 25.0f, 1, 360.f, 0.5f },
        { 31, BOOK, "Book", "Friend Without Time"         , "", 25.0f, 1, 360.f, 0.5f },
        { 32, BOOK, "Book", "Enemies With Silver"         , "", 25.0f, 1, 360.f, 0.5f },
        { 33, BOOK, "Book", "Priests Of The Plague"       , "", 25.0f, 1, 360.f, 0.5f },
        { 34, BOOK, "Book", "Giants And Heirs"            , "", 25.0f, 1, 360.f, 0.5f },
        { 35, BOOK, "Book", "Hunters And Mice"            , "", 25.0f, 1, 360.f, 0.5f },
        { 36, BOOK, "Book", "Nation Without Flaws"        , "", 25.0f, 1, 360.f, 0.5f },
        { 37, BOOK, "Book", "Revenge Of The Frontline"    , "", 25.0f, 1, 360.f, 0.5f },
        { 38, BOOK, "Book", "Running In The Hunter"       , "", 25.0f, 1, 360.f, 0.5f },
        { 39, BOOK, "Book", "Rescue At The Mist"          , "", 25.0f, 1, 360.f, 0.5f },
        { 40, BOOK, "Book", "Gangster With Immortality"   , "", 25.0f, 1, 360.f, 0.5f },
        { 41, BOOK, "Book", "Blacksmith Of Freedom"       , "", 25.0f, 1, 360.f, 0.5f },
        { 42, BOOK, "Book", "Wives Without Direction"     , "", 25.0f, 1, 360.f, 0.5f },
        { 43, BOOK, "Book", "Women With Silver"           , "", 25.0f, 1, 360.f, 0.5f },
        { 44, BOOK, "Book", "Spies And Strangers"         , "", 25.0f, 1, 360.f, 0.5f },
        { 45, BOOK, "Book", "Thieves And Kings"           , "", 25.0f, 1, 360.f, 0.5f },
        { 46, BOOK, "Book", "World Of The Ancients"       , "", 25.0f, 1, 360.f, 0.5f },
        { 47, BOOK, "Book", "Effect Of Hope"              , "", 25.0f, 1, 360.f, 0.5f },
        { 48, BOOK, "Book", "Deceiving The Forest"        , "", 25.0f, 1, 360.f, 0.5f },
        { 49, BOOK, "Book", "Remember The Forest"         , "", 25.0f, 1, 360.f, 0.5f },
        { 50, BOOK, "Book", "Woman Of Glory"              , "", 25.0f, 1, 360.f, 0.5f },
        { 51, BOOK, "Book", "Agent With Silver"           , "", 25.0f, 1, 360.f, 0.5f },
        { 52, BOOK, "Book", "Foes Of The River"           , "", 25.0f, 1, 360.f, 0.5f },
        { 53, BOOK, "Book", "Robots With Money"           , "", 25.0f, 1, 360.f, 0.5f },
        { 54, BOOK, "Book", "Women And Criminals"         , "", 25.0f, 1, 360.f, 0.5f },
        { 55, BOOK, "Book", "Officers And Rats"           , "", 25.0f, 1, 360.f, 0.5f },
        { 56, BOOK, "Book", "Country Of Glory"            , "", 25.0f, 1, 360.f, 0.5f },
        { 57, BOOK, "Book", "Will Of My Imagination"      , "", 25.0f, 1, 360.f, 0.5f },
        { 58, BOOK, "Book", "Traces In The Jungle"        , "", 25.0f, 1, 360.f, 0.5f },
        { 59, BOOK, "Book", "Dancing In The Animals"      , "", 25.0f, 1, 360.f, 0.5f },
        { 60, BOOK, "Book", "Assassin Of Stone"           , "", 25.0f, 1, 360.f, 0.5f },
        { 61, BOOK, "Book", "Criminal Of Earth"           , "", 25.0f, 1, 360.f, 0.5f },
        { 62, BOOK, "Book", "Blacksmiths Of The Stockades", "", 25.0f, 1, 360.f, 0.5f },
        { 63, BOOK, "Book", "Humans With Pride"           , "", 25.0f, 1, 360.f, 0.5f },
        { 64, BOOK, "Book", "Owls And Dogs"               , "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(books[index].index == index);
    return books[index];
}

static Item boot(const int index)
{
    static const Item boots[] = {
        { 0, BOOT, "Boots", "Footpads of Binding Lands"      , "", 25.0f, 1, 360.f, 0.5f },
        { 1, BOOT, "Boots", "Sabatons of Conquered Damnation", "", 25.0f, 1, 360.f, 0.5f },
        { 2, BOOT, "Boots", "Crazed Bronzed Greatboots"      , "", 25.0f, 1, 360.f, 0.5f },
        { 3, BOOT, "Boots", "Grieving Adamantite Sabatons"   , "", 25.0f, 1, 360.f, 0.5f },
        { 4, BOOT, "Boots", "Ebon Skeletal Greaves"          , "", 25.0f, 1, 360.f, 0.5f },
        { 5, BOOT, "Boots", "Iron Walkers of Hallowed Souls" , "", 25.0f, 1, 360.f, 0.5f },
        { 6, BOOT, "Boots", "Thunderfury Skeletal Greaves"   , "", 25.0f, 1, 360.f, 0.5f },
        { 7, BOOT, "Boots", "Hollow Feet of Nature"          , "", 25.0f, 1, 360.f, 0.5f },
        { 8, BOOT, "Boots", "Challenger's Quilted Boots"     , "", 25.0f, 1, 360.f, 0.5f },
        { 9, BOOT, "Boots", "Sprinters of the Champion"      , "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(boots[index].index == index);
    return boots[index];
}

static Item chest(const int index)
{
    static const Item chests[] = {
        {  0, CHEST, "Chests", "Small Locked Chest"              , "", 25.0f, 1, 360.f, 0.5f },
        {  1, CHEST, "Chests", "Large Locked Chest"              , "", 25.0f, 1, 360.f, 0.5f },
        {  2, CHEST, "Chests", "Broken Chest"                    , "", 25.0f, 1, 360.f, 0.5f },
        {  3, CHEST, "Chests", "Small Locked Safe"               , "", 25.0f, 1, 360.f, 0.5f },
        {  4, CHEST, "Chests", "Large Locked Safe"               , "", 25.0f, 1, 360.f, 0.5f },
        {  5, CHEST, "Chests", "Broken Safe"                     , "", 25.0f, 1, 360.f, 0.5f },
        {  6, CHEST, "Chests", "Broken Safe"                     , "", 25.0f, 1, 360.f, 0.5f },
        {  7, CHEST, "Chests", "Crystal Enclosure"               , "", 25.0f, 1, 360.f, 0.5f },
        {  8, CHEST, "Chests", "Broken Crystal Enclosure"        , "", 25.0f, 1, 360.f, 0.5f },
        {  9, CHEST, "Chests", "Gold Pouch"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 10, CHEST, "Chests", "Mystic Gold Pouch"               , "", 25.0f, 1, 360.f, 0.5f },
        { 11, CHEST, "Chests", "Expensive Gold Pouch"            , "", 25.0f, 1, 360.f, 0.5f },
        { 12, CHEST, "Chests", "Hungry Gold Pouch"               , "", 25.0f, 1, 360.f, 0.5f },
        { 13, CHEST, "Chests", "Barrel of Common Goods"          , "", 25.0f, 1, 360.f, 0.5f },
        { 14, CHEST, "Chests", "Barrel of Rare Goods"            , "", 25.0f, 1, 360.f, 0.5f },
        { 15, CHEST, "Chests", "Broken Barrel of Common Goods"   , "", 25.0f, 1, 360.f, 0.5f },
        { 16, CHEST, "Chests", "Broken Barrel of Expensive Goods", "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(chests[index].index == index);
    return chests[index];
}

static Item flesh(Item it)
{
}

static Item food(const int index)
{
    static const Item foods[] = {
        {  0, FOOD, "Food", "Cramp Lion"                       , "", 25.0f, 1, 360.f, 0.5f },
        {  1, FOOD, "Food", "Stag Ear"                         , "", 25.0f, 1, 360.f, 0.5f },
        {  2, FOOD, "Food", "Luminescent Lichen"               , "", 25.0f, 1, 360.f, 0.5f },
        {  3, FOOD, "Food", "Orange Jelly Drop"                , "", 25.0f, 1, 360.f, 0.5f },
        {  4, FOOD, "Food", "Mossy Hair Lichen"                , "", 25.0f, 1, 360.f, 0.5f },
        {  5, FOOD, "Food", "Scaly Swamp Russula"              , "", 25.0f, 1, 360.f, 0.5f },
        {  6, FOOD, "Food", "Hypoxylon Lichen"                 , "", 25.0f, 1, 360.f, 0.5f },
        {  7, FOOD, "Food", "Questionable Felt Lichen"         , "", 25.0f, 1, 360.f, 0.5f },
        {  8, FOOD, "Food", "Marasmiellus Spindles"            , "", 25.0f, 1, 360.f, 0.5f },
        {  9, FOOD, "Food", "Powdered Pored Bolete"            , "", 25.0f, 1, 360.f, 0.5f },
        { 10, FOOD, "Food", "Fly Blewit"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 11, FOOD, "Food", "Snow-white Egg Bird's Nest Fungus", "", 25.0f, 1, 360.f, 0.5f },
        { 12, FOOD, "Food", "Viscid Destroyer Fungi"           , "", 25.0f, 1, 360.f, 0.5f },
        { 13, FOOD, "Food", "Tumbling Waxcap"                  , "", 25.0f, 1, 360.f, 0.5f },
        { 14, FOOD, "Food", "Cloudy Widow"                     , "", 25.0f, 1, 360.f, 0.5f },
        { 15, FOOD, "Food", "Green-belted Mans Fingers"        , "", 25.0f, 1, 360.f, 0.5f },
        { 16, FOOD, "Food", "Hygroscopic Waxycap"              , "", 25.0f, 1, 360.f, 0.5f },
        { 17, FOOD, "Food", "Onger"                            , "", 25.0f, 1, 360.f, 0.5f },
        { 18, FOOD, "Food", "Nilery"                           , "", 25.0f, 1, 360.f, 0.5f },
        { 19, FOOD, "Food", "Kime"                             , "", 25.0f, 1, 360.f, 0.5f },
        { 20, FOOD, "Food", "Sun Guanabana"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 21, FOOD, "Food", "Iggeomquat"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 22, FOOD, "Food", "River Mango"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 23, FOOD, "Food", "Klorang"                          , "", 25.0f, 1, 360.f, 0.5f },
        { 24, FOOD, "Food", "Sun Cawesh"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 25, FOOD, "Food", "Summer Babaco"                    , "", 25.0f, 1, 360.f, 0.5f },
        { 26, FOOD, "Food", "Dragon Bean"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 27, FOOD, "Food", "Kastrialon"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 28, FOOD, "Food", "Teobaco"                          , "", 25.0f, 1, 360.f, 0.5f },
        { 29, FOOD, "Food", "Wrurreorang"                      , "", 25.0f, 1, 360.f, 0.5f },
        { 30, FOOD, "Food", "Eale"                             , "", 25.0f, 1, 360.f, 0.5f },
        { 31, FOOD, "Food", "Ogliaguaro"                       , "", 25.0f, 1, 360.f, 0.5f },
        { 32, FOOD, "Food", "Natine"                           , "", 25.0f, 1, 360.f, 0.5f },
        { 33, FOOD, "Food", "Lembas"                           , "", 25.0f, 1, 360.f, 0.5f },
        { 34, FOOD, "Food", "Lembas Delight"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 35, FOOD, "Food", "Klegriariac Bonbons"              , "", 25.0f, 1, 360.f, 0.5f },
        { 36, FOOD, "Food", "Crown Honey Snacks"               , "", 25.0f, 1, 360.f, 0.5f },
        { 37, FOOD, "Food", "Barrel of Mead"                   , "", 25.0f, 1, 360.f, 0.5f },
        { 38, FOOD, "Food", "Lump of Honey Sugar"              , "", 25.0f, 1, 360.f, 0.5f },
        { 39, FOOD, "Food", "River Mango Pie"                  , "", 25.0f, 1, 360.f, 0.5f },
        { 40, FOOD, "Food", "Natine Chocolate Bar"             , "", 25.0f, 1, 360.f, 0.5f },
        { 41, FOOD, "Food", "Bruggearind Fortune Cake"         , "", 25.0f, 1, 360.f, 0.5f },
        { 42, FOOD, "Food", "Slossimelo Sweet Bun"             , "", 25.0f, 1, 360.f, 0.5f },
    };
    assert(foods[index].index == index);
    return foods[index];
}

static Item glove(Item it)
{
    it.cstr = "Glove";

    switch(it.index)
    {
    case 0:
        it.name = "Honor's Embroided Handguards";
        break;

    case 1:
        it.name = "Quilted Gloves";
        break;

    case 2:
        it.name = "Vengeance Quilted Hands";
        break;

    case 3:
        it.name = "Gladiator's Gloves of Immortal";
        break;
    }

    return it;
}

static Item hat(Item it)
{
    it.cstr = "Hat";

    switch(it.index)
    {
    case 0:
        it.name = "Pot of Cooking";
        break;

    case 1:
        it.name = "Soldier's Titanium Headcover";
        break;

    case 2:
        it.name = "Plate Faceguard of Condemned Worlds";
        break;

    case 3:
        it.name = "Vindication Steel Headcover";
        break;

    case 4:
        it.name = "Ivory Gaze of Fleeting Sorrow";
        break;

    case 5:
        it.name = "Vindication Faceguard of Slaughter";
        break;

    case 6:
        it.name = "Hide Helm of Unholy Visions";
        break;

    case 7:
        it.name = "Steel Cap of Relentless Visions";
        break;

    case 8:
        it.name = "Steel Facemask of Relentless Visions";
        break;

    case 9:
        it.name = "Desolation Cloth Coif";
        break;

    case 10:
        it.name = "Wizard's Deep Blue Hat";
        break;

    case 11:
        it.name = "Dunce Cap";
        break;

    case 12:
        it.name = "Metal Head Ring";
        break;

    case 13:
        it.name = "Crown of the Blackthorn";
        break;

    case 14:
        it.name = "Crown of the Shattered Plain's King";
        break;
    }

    return it;
}

static Item key(Item it)
{
    it.cstr = "Key";

    switch(it.index)
    {
    case 0:
        it.name = "Silver Key";
        break;

    case 1:
        it.name = "Iron Key";
        break;

    case 2:
        it.name = "ID Card";
        break;
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
    case  0:
        it.name = "Odal";
        it.desc = "Home";
        break;

    case  1:
        it.name = "Daeg";
        it.desc = "Day";
        break;

    case  2:
        it.name = "Jera";
        it.desc = "Harvest";
        break;

    case  3:
        it.name = "Lagu";
        it.desc = "Water";
        break;

    case  4:
        it.name = "Man";
        it.desc = "Human";
        break;

    case  5:
        it.name = "Eh";
        it.desc = "Horse";
        break;

    case  6:
        it.name = "Beorc";
        it.desc = "Puriry";
        break;

    case  7:
        it.name = "Tyr";
        it.desc = "Arrow";
        break;

    case  8:
        it.name = "Sigel";
        it.desc = "Sun";
        break;

    case  9:
        it.name = "Elhaz";
        it.desc = "Elk";
        break;

    case 10:
        it.name = "Peorth";
        it.desc = "Dice";
        break;

    case 11:
        it.name = "Eoh";
        it.desc = "Yew";
        break;

    case 12:
        it.name = "Ng";
        it.desc = "Hero";
        break;

    case 13:
        it.name = "Is";
        it.desc = "Ice";
        break;

    case 14:
        it.name = "Nyd";
        it.desc = "Need";
        break;

    case 15:
        it.name = "Hagal";
        it.desc = "Hail";
        break;

    case 16:
        it.name = "Wynn";
        it.desc = "Wind";
        break;

    case 17:
        it.name = "Gifu";
        it.desc = "Gift";
        break;

    case 18:
        it.name = "Ken";
        it.desc = "Wood";
        break;

    case 19:
        it.name = "Rad";
        it.desc = "Wheel";
        break;

    case 20:
        it.name = "As";
        it.desc = "God";
        break;

    case 21:
        it.name = "Thorn";
        it.desc = "Giant";
        break;

    case 22:
        it.name = "Ur";
        it.desc = "Ox";
        break;

    case 23:
        it.name = "Feoh";
        it.desc = "Cattle";
        break;
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
    switch(c)
    {
    case     AMMO: return     ammo(index);
    case   AMULET: return   amulet(index);
    case    ARMOR: return    armor(index);
    case     BOOK: return     book(index);
    //case     BOOT: return     boot(it);
    //case    CHEST: return    chest(it);
    //case    FLESH: return    flesh(it);
    //case     FOOD: return     food(it);
    //case    GLOVE: return    glove(it);
    //case      HAT: return      hat(it);
    //case      KEY: return      key(it);
    //case    LIGHT: return    light(it);
    //case  LONGWEP: return  longwep(it);
    //case   MEDWEP: return   medwep(it);
    //case    MONEY: return    money(it);
    //case    MUSIC: return    music(it);
    //case   POTION: return   potion(it);
    //case     RING: return     ring(it);
    //case     ROCK: return     rock(it);
    //case   SCROLL: return   scroll(it);
    //case   SHIELD: return   shield(it);
    //case SHORTWEP: return shortwep(it);
    //case     TOOL: return     tool(it);
    //case     WAND: return     wand(it);
    //case     NONE: return     none(it);
    }
}

Item xitrand(void)
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(c);
    return xitnew(c, index);
}
