#include "Item.h"

#include "util.h"

#include <assert.h>
#include <stdlib.h>

#define CHECK(items, CLASS)                  \
{                                            \
    assert(xlen(items) == xcindices(CLASS)); \
    for(int i = 0; i < xlen(items); i++)     \
        assert(items[i].index == i);         \
}

static Item xzitem(void)
{
    static Item item;
    return item;
}

static Item ammo(const int index)
{
    const Item ammos[] = {
        {  0, AMMO, "Ammo", "Silver Bullets"    , "", 25.0f, 1, 360.0f, 0.05f },
        {  1, AMMO, "Ammo", "Regular Bullets"   , "", 25.0f, 1, 360.0f, 0.05f },
        {  2, AMMO, "Ammo", "Poorman's Bullets" , "", 25.0f, 1, 360.0f, 0.05f },
        {  3, AMMO, "Ammo", "Popcap Bullets"    , "", 25.0f, 1, 360.0f, 0.05f },
        {  4, AMMO, "Ammo", "Pearls"            , "", 25.0f, 1, 360.0f, 0.05f },
        {  5, AMMO, "Ammo", "River Pebbels"     , "", 25.0f, 1, 360.0f, 0.05f },
        {  6, AMMO, "Ammo", "Stone Bits"        , "", 25.0f, 1, 360.0f, 0.05f },
        {  7, AMMO, "Ammo", "Bits of Sand"      , "", 25.0f, 1, 360.0f, 0.05f },
        {  8, AMMO, "Ammo", "Hunter's Bow"      , "", 25.0f, 1, 360.0f, 0.05f },
        {  9, AMMO, "Ammo", "Orcish Bow"        , "", 25.0f, 1, 360.0f, 0.05f },
        { 10, AMMO, "Ammo", "Peasant Bow"       , "", 25.0f, 1, 360.0f, 0.05f },
        { 11, AMMO, "Ammo", "Elvish Bow"        , "", 25.0f, 1, 360.0f, 0.05f },
        { 12, AMMO, "Ammo", "Sling"             , "", 25.0f, 1, 360.0f, 0.05f },
        { 13, AMMO, "Ammo", "Crossbow"          , "", 25.0f, 1, 360.0f, 0.05f },
        { 14, AMMO, "Ammo", "Mithril Arrows"    , "", 25.0f, 1, 360.0f, 0.05f },
        { 15, AMMO, "Ammo", "Iron Arrows"       , "", 25.0f, 1, 360.0f, 0.05f },
        { 16, AMMO, "Ammo", "Gold Tip Arrows"   , "", 25.0f, 1, 360.0f, 0.05f },
        { 17, AMMO, "Ammo", "Bone Arrows"       , "", 25.0f, 1, 360.0f, 0.05f },
        { 18, AMMO, "Ammo", "Adament Arrows"    , "", 25.0f, 1, 360.0f, 0.05f },
        { 19, AMMO, "Ammo", "Barbed Arrows"     , "", 25.0f, 1, 360.0f, 0.05f },
        { 20, AMMO, "Ammo", "Dart"              , "", 25.0f, 1, 360.0f, 0.05f },
        { 21, AMMO, "Ammo", "Shurikens"         , "", 25.0f, 1, 360.0f, 0.05f },
        { 22, AMMO, "Ammo", "Boomerang"         , "", 25.0f, 1, 360.0f, 0.05f },
        { 23, AMMO, "Ammo", "One-Handed Cannon" , "", 25.0f, 1, 360.0f, 0.05f },
        { 24, AMMO, "Ammo", "Two-Handed Cannon" , "", 25.0f, 1, 360.0f, 0.05f },
    };
    CHECK(ammos, AMMO)
    return ammos[index];
}

static Item amulet(const int index)
{
    const Item amulets[] = {
        {  0, AMULET, "Amulet", "Emblem of Reincarnation"           , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, AMULET, "Amulet", "Aura of Endurance"                 , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, AMULET, "Amulet", "Whisper of Panic"                  , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, AMULET, "Amulet", "Fortified Secrets of Power"        , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, AMULET, "Amulet", "Enhanced Aspect of Auras"          , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, AMULET, "Amulet", "Mighty Mark of Hunger"             , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, AMULET, "Amulet", "Vampiric Hymn of Magical Defenses" , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, AMULET, "Amulet", "Gift of Prime Loss"                , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, AMULET, "Amulet", "Emblem of Strengthened Storms"     , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, AMULET, "Amulet", "Hymn of Eternal Potency"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, AMULET, "Amulet", "Spark of Sight"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, AMULET, "Amulet", "Enchantment of Success"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, AMULET, "Amulet", "Grace of Haste"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, AMULET, "Amulet", "Fortified Charge of Cover"         , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, AMULET, "Amulet", "Lesser Bond of Cover"              , "", 25.0f, 1, 360.0f, 0.5f },
        { 15, AMULET, "Amulet", "Brilliant Infusion of Danger"      , "", 25.0f, 1, 360.0f, 0.5f },
        { 16, AMULET, "Amulet", "Light Seal of Disbelief"           , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(amulets, AMULET)
    return amulets[index];
}

static Item armor(const int index)
{
    const Item armors[] = {
        {  0, ARMOR, "Armor", "Bronze Chainmail of Immortal Souls"     , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, ARMOR, "Armor", "Bronze Chainmail of Divine Bloodlust"   , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, ARMOR, "Armor", "Mithril Chainmail of Chestguard"        , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, ARMOR, "Armor", "Adamant Chainmail of Pride's Fall"      , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, ARMOR, "Armor", "Bronze Chainmail of the Lone Wolf"      , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, ARMOR, "Armor", "Mithril Chainmail of Delusions"         , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, ARMOR, "Armor", "Gold Chainmail of Relentless Voices"    , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, ARMOR, "Armor", "Mithril Chainmail of Recoil"            , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, ARMOR, "Armor", "Charcoal Chainmail of Broken Bones"     , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, ARMOR, "Armor", "Steel Chainmail of Riddles"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, ARMOR, "Armor", "Adamant Chainmail of Unholy Blight"     , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, ARMOR, "Armor", "Poly Chainmail of Dark Magic"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, ARMOR, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, ARMOR, "Armor", "Bronze Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 15, ARMOR, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 16, ARMOR, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 17, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 18, ARMOR, "Armor", "Gold Chain Rings"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 19, ARMOR, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 20, ARMOR, "Armor", "Charcoal Chain Rings"                   , "", 25.0f, 1, 360.0f, 0.5f },
        { 21, ARMOR, "Armor", "Steel Chain Rings"                      , "", 25.0f, 1, 360.0f, 0.5f },
        { 22, ARMOR, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 23, ARMOR, "Armor", "Poly Chain Rings"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 24, ARMOR, "Armor", "Greatcloak of Blind Whispers"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 25, ARMOR, "Armor", "Drape of Blessed Glory"                 , "", 25.0f, 1, 360.0f, 0.5f },
        { 26, ARMOR, "Armor", "Wool Mantle of Distant Powers"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 27, ARMOR, "Armor", "Possessed Scaled Shroud"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 28, ARMOR, "Armor", "Cloth Greatcloak of Haunted Justice"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 29, ARMOR, "Armor", "Exile Embroided Wrap"                   , "", 25.0f, 1, 360.0f, 0.5f },
        { 30, ARMOR, "Armor", "Warlord's Padded Greatcloak"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 31, ARMOR, "Armor", "Lightning Mantle of Secrets"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 32, ARMOR, "Armor", "Dragon Shroud of the Talon"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 33, ARMOR, "Armor", "Doom's Wool Drape"                      , "", 25.0f, 1, 360.0f, 0.5f },
        { 34, ARMOR, "Armor", "Piece of Cloth"                         , "", 25.0f, 1, 360.0f, 0.5f },
        { 35, ARMOR, "Armor", "Dirty Rags"                             , "", 25.0f, 1, 360.0f, 0.5f },
        { 36, ARMOR, "Armor", "Copper Chestguard of Fleeting Kings"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 37, ARMOR, "Armor", "Bronze Chestguard of Hallowed Wars"     , "", 25.0f, 1, 360.0f, 0.5f },
        { 38, ARMOR, "Armor", "Steel Breastplate"                      , "", 25.0f, 1, 360.0f, 0.5f },
        { 39, ARMOR, "Armor", "Studded Steel Breastplate"              , "", 25.0f, 1, 360.0f, 0.5f },
        { 40, ARMOR, "Armor", "Mithril Chestgaurd of Conquered Worlds" , "", 25.0f, 1, 360.0f, 0.5f },
        { 41, ARMOR, "Armor", "Steel Plated Cuirass"                   , "", 25.0f, 1, 360.0f, 0.5f },
        { 42, ARMOR, "Armor", "Heavy Steel Plated Cuirass"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 43, ARMOR, "Armor", "Storm-Forged Steel Cuirass of the Gods" , "", 25.0f, 1, 360.0f, 0.5f },
        { 44, ARMOR, "Armor", "Common Clothes"                         , "", 25.0f, 1, 360.0f, 0.5f },
        { 45, ARMOR, "Armor", "Kimono of the Apprentice"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 46, ARMOR, "Armor", "Kimono of Festivities"                  , "", 25.0f, 1, 360.0f, 0.5f },
        { 47, ARMOR, "Armor", "Kimono of the Western Sea"              , "", 25.0f, 1, 360.0f, 0.5f },
        { 48, ARMOR, "Armor", "Kimono of the Fisherman"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 49, ARMOR, "Armor", "Common Headscarf"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 50, ARMOR, "Armor", "Desert Headscarf"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 51, ARMOR, "Armor", "Red Cape of Riding"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 52, ARMOR, "Armor", "Iron Chainmail"                         , "", 25.0f, 1, 360.0f, 0.5f },
        { 53, ARMOR, "Armor", "Gold Chainmail"                         , "", 25.0f, 1, 360.0f, 0.5f },
        { 54, ARMOR, "Armor", "Steel Chaimail"                         , "", 25.0f, 1, 360.0f, 0.5f },
        { 55, ARMOR, "Armor", "Bronze Chainmail"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 56, ARMOR, "Armor", "Charcoal Chainmail"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 57, ARMOR, "Armor", "Wooden Chest Armor"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 58, ARMOR, "Armor", "Hide Skin"                              , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(armors, ARMOR)
    return armors[index];
}

static Item book(const int index)
{
    const Item books[] = {
        {  0, BOOK, "Book", "Duke Of The Nation"           , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, BOOK, "Book", "Criminal Of The Night"        , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, BOOK, "Book", "Humans Without Duty"          , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, BOOK, "Book", "Descendants Of The Day"       , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, BOOK, "Book", "Slaves And Friends"           , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, BOOK, "Book", "Defenders And Swindlers"      , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, BOOK, "Book", "Ruins With Money"             , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, BOOK, "Book", "Sword Of History"             , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, BOOK, "Book", "Dead At The Beginning"        , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, BOOK, "Book", "Cleaning Up My Destiny"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, BOOK, "Book", "Defender With Pride"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, BOOK, "Book", "Human Without Courage"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, BOOK, "Book", "Blacksmiths Of Hell"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, BOOK, "Book", "Invaders Of The Great"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, BOOK, "Book", "Slaves And Phantoms"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 15, BOOK, "Book", "Heroes And Heirs"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 16, BOOK, "Book", "Vengeance Of Heaven"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 17, BOOK, "Book", "Culmination Of Fire"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 18, BOOK, "Book", "Commanding The Depths"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 19, BOOK, "Book", "Searching In Nightmares"      , "", 25.0f, 1, 360.0f, 0.5f },
        { 20, BOOK, "Book", "Butcher Of The Ancestors"     , "", 25.0f, 1, 360.0f, 0.5f },
        { 21, BOOK, "Book", "Priestess Of The Great"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 22, BOOK, "Book", "Hunters Without A Goal"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 23, BOOK, "Book", "Wolves Without Courage"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 24, BOOK, "Book", "Bandits And Foreigners"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 25, BOOK, "Book", "Strangers And Foes"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 26, BOOK, "Book", "Crossbow Of The Ancestors"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 27, BOOK, "Book", "Luck Of Dread"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 28, BOOK, "Book", "Challenging The Immortals"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 29, BOOK, "Book", "Dwelling In The Emperor"      , "", 25.0f, 1, 360.0f, 0.5f },
        { 30, BOOK, "Book", "Criminal Of Utopia"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 31, BOOK, "Book", "Friend Without Time"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 32, BOOK, "Book", "Enemies With Silver"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 33, BOOK, "Book", "Priests Of The Plague"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 34, BOOK, "Book", "Giants And Heirs"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 35, BOOK, "Book", "Hunters And Mice"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 36, BOOK, "Book", "Nation Without Flaws"         , "", 25.0f, 1, 360.0f, 0.5f },
        { 37, BOOK, "Book", "Revenge Of The Frontline"     , "", 25.0f, 1, 360.0f, 0.5f },
        { 38, BOOK, "Book", "Running In The Hunter"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 39, BOOK, "Book", "Rescue At The Mist"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 40, BOOK, "Book", "Gangster With Immortality"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 41, BOOK, "Book", "Blacksmith Of Freedom"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 42, BOOK, "Book", "Wives Without Direction"      , "", 25.0f, 1, 360.0f, 0.5f },
        { 43, BOOK, "Book", "Women With Silver"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 44, BOOK, "Book", "Spies And Strangers"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 45, BOOK, "Book", "Thieves And Kings"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 46, BOOK, "Book", "World Of The Ancients"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 47, BOOK, "Book", "Effect Of Hope"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 48, BOOK, "Book", "Deceiving The Forest"         , "", 25.0f, 1, 360.0f, 0.5f },
        { 49, BOOK, "Book", "Remember The Forest"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 50, BOOK, "Book", "Woman Of Glory"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 51, BOOK, "Book", "Agent With Silver"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 52, BOOK, "Book", "Foes Of The River"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 53, BOOK, "Book", "Robots With Money"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 54, BOOK, "Book", "Women And Criminals"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 55, BOOK, "Book", "Officers And Rats"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 56, BOOK, "Book", "Country Of Glory"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 57, BOOK, "Book", "Will Of My Imagination"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 58, BOOK, "Book", "Traces In The Jungle"         , "", 25.0f, 1, 360.0f, 0.5f },
        { 59, BOOK, "Book", "Dancing In The Animals"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 60, BOOK, "Book", "Assassin Of Stone"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 61, BOOK, "Book", "Criminal Of Earth"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 62, BOOK, "Book", "Blacksmiths Of The Stockades" , "", 25.0f, 1, 360.0f, 0.5f },
        { 63, BOOK, "Book", "Humans With Pride"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 64, BOOK, "Book", "Owls And Dogs"                , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(books, BOOK)
    return books[index];
}

static Item boot(const int index)
{
    const Item boots[] = {
        { 0, BOOT, "Boots", "Footpads of Binding Lands"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 1, BOOT, "Boots", "Sabatons of Conquered Damnation" , "", 25.0f, 1, 360.0f, 0.5f },
        { 2, BOOT, "Boots", "Crazed Bronzed Greatboots"       , "", 25.0f, 1, 360.0f, 0.5f },
        { 3, BOOT, "Boots", "Grieving Adamantite Sabatons"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 4, BOOT, "Boots", "Ebon Skeletal Greaves"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 5, BOOT, "Boots", "Iron Walkers of Hallowed Souls"  , "", 25.0f, 1, 360.0f, 0.5f },
        { 6, BOOT, "Boots", "Thunderfury Skeletal Greaves"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 7, BOOT, "Boots", "Hollow Feet of Nature"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 8, BOOT, "Boots", "Challenger's Quilted Boots"      , "", 25.0f, 1, 360.0f, 0.5f },
        { 9, BOOT, "Boots", "Sprinters of the Champion"       , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(boots, BOOT)
    return boots[index];
}

static Item chest(const int index)
{
    const Item chests[] = {
        {  0, CHEST, "Chests", "Small Locked Chest"               , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, CHEST, "Chests", "Large Locked Chest"               , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, CHEST, "Chests", "Broken Chest"                     , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, CHEST, "Chests", "Small Locked Safe"                , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, CHEST, "Chests", "Large Locked Safe"                , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, CHEST, "Chests", "Broken Safe"                      , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, CHEST, "Chests", "Broken Safe"                      , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, CHEST, "Chests", "Crystal Enclosure"                , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, CHEST, "Chests", "Broken Crystal Enclosure"         , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, CHEST, "Chests", "Gold Pouch"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, CHEST, "Chests", "Mystic Gold Pouch"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, CHEST, "Chests", "Expensive Gold Pouch"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, CHEST, "Chests", "Hungry Gold Pouch"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, CHEST, "Chests", "Barrel of Common Goods"           , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, CHEST, "Chests", "Barrel of Rare Goods"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 15, CHEST, "Chests", "Broken Barrel of Common Goods"    , "", 25.0f, 1, 360.0f, 0.5f },
        { 16, CHEST, "Chests", "Broken Barrel of Expensive Goods" , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(chests, CHEST)
    return chests[index];
}

static Item flesh(const int index)
{
    (void) index;
    return xzitem();
}

static Item food(const int index)
{
    const Item foods[] = {
        {  0, FOOD, "Food", "Cramp Lion"                        , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, FOOD, "Food", "Stag Ear"                          , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, FOOD, "Food", "Luminescent Lichen"                , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, FOOD, "Food", "Orange Jelly Drop"                 , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, FOOD, "Food", "Mossy Hair Lichen"                 , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, FOOD, "Food", "Scaly Swamp Russula"               , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, FOOD, "Food", "Hypoxylon Lichen"                  , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, FOOD, "Food", "Questionable Felt Lichen"          , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, FOOD, "Food", "Marasmiellus Spindles"             , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, FOOD, "Food", "Powdered Pored Bolete"             , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, FOOD, "Food", "Fly Blewit"                        , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, FOOD, "Food", "Snow-white Egg Bird's Nest Fungus" , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, FOOD, "Food", "Viscid Destroyer Fungi"            , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, FOOD, "Food", "Tumbling Waxcap"                   , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, FOOD, "Food", "Cloudy Widow"                      , "", 25.0f, 1, 360.0f, 0.5f },
        { 15, FOOD, "Food", "Green-belted Mans Fingers"         , "", 25.0f, 1, 360.0f, 0.5f },
        { 16, FOOD, "Food", "Hygroscopic Waxycap"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 17, FOOD, "Food", "Onger"                             , "", 25.0f, 1, 360.0f, 0.5f },
        { 18, FOOD, "Food", "Nilery"                            , "", 25.0f, 1, 360.0f, 0.5f },
        { 19, FOOD, "Food", "Kime"                              , "", 25.0f, 1, 360.0f, 0.5f },
        { 20, FOOD, "Food", "Sun Guanabana"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 21, FOOD, "Food", "Iggeomquat"                        , "", 25.0f, 1, 360.0f, 0.5f },
        { 22, FOOD, "Food", "River Mango"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 23, FOOD, "Food", "Klorang"                           , "", 25.0f, 1, 360.0f, 0.5f },
        { 24, FOOD, "Food", "Sun Cawesh"                        , "", 25.0f, 1, 360.0f, 0.5f },
        { 25, FOOD, "Food", "Summer Babaco"                     , "", 25.0f, 1, 360.0f, 0.5f },
        { 26, FOOD, "Food", "Dragon Bean"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 27, FOOD, "Food", "Kastrialon"                        , "", 25.0f, 1, 360.0f, 0.5f },
        { 28, FOOD, "Food", "Teobaco"                           , "", 25.0f, 1, 360.0f, 0.5f },
        { 29, FOOD, "Food", "Wrurreorang"                       , "", 25.0f, 1, 360.0f, 0.5f },
        { 30, FOOD, "Food", "Eale"                              , "", 25.0f, 1, 360.0f, 0.5f },
        { 31, FOOD, "Food", "Ogliaguaro"                        , "", 25.0f, 1, 360.0f, 0.5f },
        { 32, FOOD, "Food", "Natine"                            , "", 25.0f, 1, 360.0f, 0.5f },
        { 33, FOOD, "Food", "Lembas"                            , "", 25.0f, 1, 360.0f, 0.5f },
        { 34, FOOD, "Food", "Lembas Delight"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 35, FOOD, "Food", "Klegriariac Bonbons"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 36, FOOD, "Food", "Crown Honey Snacks"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 37, FOOD, "Food", "Barrel of Mead"                    , "", 25.0f, 1, 360.0f, 0.5f },
        { 38, FOOD, "Food", "Lump of Honey Sugar"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 39, FOOD, "Food", "River Mango Pie"                   , "", 25.0f, 1, 360.0f, 0.5f },
        { 40, FOOD, "Food", "Natine Chocolate Bar"              , "", 25.0f, 1, 360.0f, 0.5f },
        { 41, FOOD, "Food", "Bruggearind Fortune Cake"          , "", 25.0f, 1, 360.0f, 0.5f },
        { 42, FOOD, "Food", "Slossimelo Sweet Bun"              , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(foods, FOOD)
    return foods[index];
}

static Item glove(const int index)
{
    const Item gloves[] = {
        { 0, GLOVE, "Gloves", "Honor's Embroided Handguards"   , "", 25.0f, 1, 360.0f, 0.5f },
        { 1, GLOVE, "Gloves", "Quilted Gloves"                 , "", 25.0f, 1, 360.0f, 0.5f },
        { 2, GLOVE, "Gloves", "Vengeance Quilted Hands"        , "", 25.0f, 1, 360.0f, 0.5f },
        { 3, GLOVE, "Gloves", "Gladiator's Gloves of Immortal" , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(gloves, GLOVE)
    return gloves[index];
}

static Item hat(const int index)
{
    const Item hats[] = {
        {  0, HAT, "Hat", "Pot of Cooking"                       , "", 25.0f, 1, 360.0f, 0.5f },
        {  1, HAT, "Hat", "Soldier's Titanium Headcover"         , "", 25.0f, 1, 360.0f, 0.5f },
        {  2, HAT, "Hat", "Plate Faceguard of Condemned Worlds"  , "", 25.0f, 1, 360.0f, 0.5f },
        {  3, HAT, "Hat", "Vindication Steel Headcover"          , "", 25.0f, 1, 360.0f, 0.5f },
        {  4, HAT, "Hat", "Ivory Gaze of Fleeting Sorrow"        , "", 25.0f, 1, 360.0f, 0.5f },
        {  5, HAT, "Hat", "Vindication Faceguard of Slaughter"   , "", 25.0f, 1, 360.0f, 0.5f },
        {  6, HAT, "Hat", "Hide Helm of Unholy Visions"          , "", 25.0f, 1, 360.0f, 0.5f },
        {  7, HAT, "Hat", "Steel Cap of Relentless Visions"      , "", 25.0f, 1, 360.0f, 0.5f },
        {  8, HAT, "Hat", "Steel Facemask of Relentless Visions" , "", 25.0f, 1, 360.0f, 0.5f },
        {  9, HAT, "Hat", "Desolation Cloth Coif"                , "", 25.0f, 1, 360.0f, 0.5f },
        { 10, HAT, "Hat", "Wizard's Deep Blue Hat"               , "", 25.0f, 1, 360.0f, 0.5f },
        { 11, HAT, "Hat", "Dunce Cap"                            , "", 25.0f, 1, 360.0f, 0.5f },
        { 12, HAT, "Hat", "Metal Head Ring"                      , "", 25.0f, 1, 360.0f, 0.5f },
        { 13, HAT, "Hat", "Crown of the Blackthorn"              , "", 25.0f, 1, 360.0f, 0.5f },
        { 14, HAT, "Hat", "Crown of the Shattered Plain's King"  , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(hats, HAT)
    return hats[index];
}

static Item key(const int index)
{
    const Item keys[] = {
        { 0, KEY, "Key", "Silver Key", "", 25.0f, 1, 360.0f, 0.5f },
        { 1, KEY, "Key", "Iron Key"  , "", 25.0f, 1, 360.0f, 0.5f },
        { 2, KEY, "Key", "ID Card"   , "", 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(keys, KEY)
    return keys[index];
}

static Item light(const int index)
{
    (void) index;
    return xzitem();
}

static Item longwep(const int index)
{
    (void) index;
    return xzitem();
}

static Item medwep(const int index)
{
    (void) index;
    return xzitem();
}

static Item money(const int index)
{
    (void) index;
    return xzitem();
}

static Item music(const int index)
{
    (void) index;
    return xzitem();
}

static Item potion(const int index)
{
    (void) index;
    return xzitem();
}

static Item ring(const int index)
{
    (void) index;
    return xzitem();
}

static Item rock(const int index)
{
    (void) index;
    return xzitem();
}

static Item scroll(const int index)
{
    const Item scrolls[] = {
        {  0, SCROLL, "Scroll", "Odal"  , "Home"   , 25.0f, 1, 360.0f, 0.5f },
        {  1, SCROLL, "Scroll", "Daeg"  , "Day"    , 25.0f, 1, 360.0f, 0.5f },
        {  2, SCROLL, "Scroll", "Jera"  , "Harvest", 25.0f, 1, 360.0f, 0.5f },
        {  3, SCROLL, "Scroll", "Lagu"  , "Water"  , 25.0f, 1, 360.0f, 0.5f },
        {  4, SCROLL, "Scroll", "Man"   , "Human"  , 25.0f, 1, 360.0f, 0.5f },
        {  5, SCROLL, "Scroll", "Eh"    , "Horse"  , 25.0f, 1, 360.0f, 0.5f },
        {  6, SCROLL, "Scroll", "Beorc" , "Puriry" , 25.0f, 1, 360.0f, 0.5f },
        {  7, SCROLL, "Scroll", "Tyr"   , "Arrow"  , 25.0f, 1, 360.0f, 0.5f },
        {  8, SCROLL, "Scroll", "Sigel" , "Sun"    , 25.0f, 1, 360.0f, 0.5f },
        {  9, SCROLL, "Scroll", "Elhaz" , "Elk"    , 25.0f, 1, 360.0f, 0.5f },
        { 10, SCROLL, "Scroll", "Peorth", "Dice"   , 25.0f, 1, 360.0f, 0.5f },
        { 11, SCROLL, "Scroll", "Eoh"   , "Yew"    , 25.0f, 1, 360.0f, 0.5f },
        { 12, SCROLL, "Scroll", "Ng"    , "Hero"   , 25.0f, 1, 360.0f, 0.5f },
        { 13, SCROLL, "Scroll", "Is"    , "Ice"    , 25.0f, 1, 360.0f, 0.5f },
        { 14, SCROLL, "Scroll", "Nyd"   , "Need"   , 25.0f, 1, 360.0f, 0.5f },
        { 15, SCROLL, "Scroll", "Hagal" , "Hail"   , 25.0f, 1, 360.0f, 0.5f },
        { 16, SCROLL, "Scroll", "Wynn"  , "Wind"   , 25.0f, 1, 360.0f, 0.5f },
        { 17, SCROLL, "Scroll", "Gifu"  , "Gift"   , 25.0f, 1, 360.0f, 0.5f },
        { 18, SCROLL, "Scroll", "Ken"   , "Wood"   , 25.0f, 1, 360.0f, 0.5f },
        { 19, SCROLL, "Scroll", "Rad"   , "Wheel"  , 25.0f, 1, 360.0f, 0.5f },
        { 20, SCROLL, "Scroll", "As"    , "God"    , 25.0f, 1, 360.0f, 0.5f },
        { 21, SCROLL, "Scroll", "Thorn" , "Giant"  , 25.0f, 1, 360.0f, 0.5f },
        { 22, SCROLL, "Scroll", "Ur"    , "Ox"     , 25.0f, 1, 360.0f, 0.5f },
        { 23, SCROLL, "Scroll", "Feoh"  , "Cattle" , 25.0f, 1, 360.0f, 0.5f },
    };
    CHECK(scrolls, SCROLL)
    return scrolls[index];
}

static Item shield(const int index)
{
    (void) index;
    return xzitem();
}

static Item shortwep(const int index)
{
    (void) index;
    return xzitem();
}

static Item tool(const int index)
{
    (void) index;
    return xzitem();
}

static Item wand(const int index)
{
    (void) index;
    return xzitem();
}

static Item none(void)
{
    Item item = xzitem();
    item.c = NONE;
    return item;
}

Item xitnew(const Classification c, const int index)
{
    switch(c)
    {
    case     AMMO: return     ammo(index);
    case   AMULET: return   amulet(index);
    case    ARMOR: return    armor(index);
    case     BOOK: return     book(index);
    case     BOOT: return     boot(index);
    case    CHEST: return    chest(index);
    case    FLESH: return    flesh(index);
    case     FOOD: return     food(index);
    case    GLOVE: return    glove(index);
    case      HAT: return      hat(index);
    case      KEY: return      key(index);
    case    LIGHT: return    light(index);
    case  LONGWEP: return  longwep(index);
    case   MEDWEP: return   medwep(index);
    case    MONEY: return    money(index);
    case    MUSIC: return    music(index);
    case   POTION: return   potion(index);
    case     RING: return     ring(index);
    case     ROCK: return     rock(index);
    case   SCROLL: return   scroll(index);
    case   SHIELD: return   shield(index);
    case SHORTWEP: return shortwep(index);
    case     TOOL: return     tool(index);
    case     WAND: return     wand(index);

    default: case NONE: return none();
    }
}

Item xitrand(void)
{
    const Classification c = (Classification) (rand() % (int) CLASSIFICATIONS);
    const int index = rand() % xcindices(c);
    return xitnew(c, index);
}
