#include "Item.h"

#include "util.h"

#include <assert.h>
#include <stdlib.h>

static const Item ammos[] = {
    { { 0,AMMO}, "Ammo", "Silver Bullets"    , "", 25.0f, 1, 360.0f, 0.05f },
    { { 1,AMMO}, "Ammo", "Regular Bullets"   , "", 25.0f, 1, 360.0f, 0.05f },
    { { 2,AMMO}, "Ammo", "Poorman's Bullets" , "", 25.0f, 1, 360.0f, 0.05f },
    { { 3,AMMO}, "Ammo", "Popcap Bullets"    , "", 25.0f, 1, 360.0f, 0.05f },
    { { 4,AMMO}, "Ammo", "Pearls"            , "", 25.0f, 1, 360.0f, 0.05f },
    { { 5,AMMO}, "Ammo", "River Pebbels"     , "", 25.0f, 1, 360.0f, 0.05f },
    { { 6,AMMO}, "Ammo", "Stone Bits"        , "", 25.0f, 1, 360.0f, 0.05f },
    { { 7,AMMO}, "Ammo", "Bits of Sand"      , "", 25.0f, 1, 360.0f, 0.05f },
    { { 8,AMMO}, "Ammo", "Hunter's Bow"      , "", 25.0f, 1, 360.0f, 0.05f },
    { { 9,AMMO}, "Ammo", "Orcish Bow"        , "", 25.0f, 1, 360.0f, 0.05f },
    { {10,AMMO}, "Ammo", "Peasant Bow"       , "", 25.0f, 1, 360.0f, 0.05f },
    { {11,AMMO}, "Ammo", "Elvish Bow"        , "", 25.0f, 1, 360.0f, 0.05f },
    { {12,AMMO}, "Ammo", "Sling"             , "", 25.0f, 1, 360.0f, 0.05f },
    { {13,AMMO}, "Ammo", "Crossbow"          , "", 25.0f, 1, 360.0f, 0.05f },
    { {14,AMMO}, "Ammo", "Mithril Arrows"    , "", 25.0f, 1, 360.0f, 0.05f },
    { {15,AMMO}, "Ammo", "Iron Arrows"       , "", 25.0f, 1, 360.0f, 0.05f },
    { {16,AMMO}, "Ammo", "Gold Tip Arrows"   , "", 25.0f, 1, 360.0f, 0.05f },
    { {17,AMMO}, "Ammo", "Bone Arrows"       , "", 25.0f, 1, 360.0f, 0.05f },
    { {18,AMMO}, "Ammo", "Adament Arrows"    , "", 25.0f, 1, 360.0f, 0.05f },
    { {19,AMMO}, "Ammo", "Barbed Arrows"     , "", 25.0f, 1, 360.0f, 0.05f },
    { {20,AMMO}, "Ammo", "Dart"              , "", 25.0f, 1, 360.0f, 0.05f },
    { {21,AMMO}, "Ammo", "Shurikens"         , "", 25.0f, 1, 360.0f, 0.05f },
    { {22,AMMO}, "Ammo", "Boomerang"         , "", 25.0f, 1, 360.0f, 0.05f },
    { {23,AMMO}, "Ammo", "One-Handed Cannon" , "", 25.0f, 1, 360.0f, 0.05f },
    { {24,AMMO}, "Ammo", "Two-Handed Cannon" , "", 25.0f, 1, 360.0f, 0.05f },
};

static const Item amulets[] = {
    { { 0,AMULET}, "Amulet", "Emblem of Reincarnation"           , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,AMULET}, "Amulet", "Aura of Endurance"                 , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,AMULET}, "Amulet", "Whisper of Panic"                  , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,AMULET}, "Amulet", "Fortified Secrets of Power"        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,AMULET}, "Amulet", "Enhanced Aspect of Auras"          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,AMULET}, "Amulet", "Mighty Mark of Hunger"             , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,AMULET}, "Amulet", "Vampiric Hymn of Magical Defenses" , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,AMULET}, "Amulet", "Gift of Prime Loss"                , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,AMULET}, "Amulet", "Emblem of Strengthened Storms"     , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,AMULET}, "Amulet", "Hymn of Eternal Potency"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,AMULET}, "Amulet", "Spark of Sight"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,AMULET}, "Amulet", "Enchantment of Success"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,AMULET}, "Amulet", "Grace of Haste"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,AMULET}, "Amulet", "Fortified Charge of Cover"         , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,AMULET}, "Amulet", "Lesser Bond of Cover"              , "", 25.0f, 1, 360.0f, 0.5f },
    { {15,AMULET}, "Amulet", "Brilliant Infusion of Danger"      , "", 25.0f, 1, 360.0f, 0.5f },
    { {16,AMULET}, "Amulet", "Light Seal of Disbelief"           , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item armors[] = {
    { { 0,ARMOR}, "Armor", "Bronze Chainmail of Immortal Souls"     , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,ARMOR}, "Armor", "Bronze Chainmail of Divine Bloodlust"   , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,ARMOR}, "Armor", "Mithril Chainmail of Chestguard"        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,ARMOR}, "Armor", "Adamant Chainmail of Pride's Fall"      , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,ARMOR}, "Armor", "Bronze Chainmail of the Lone Wolf"      , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,ARMOR}, "Armor", "Mithril Chainmail of Delusions"         , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,ARMOR}, "Armor", "Gold Chainmail of Relentless Voices"    , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,ARMOR}, "Armor", "Mithril Chainmail of Recoil"            , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,ARMOR}, "Armor", "Charcoal Chainmail of Broken Bones"     , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,ARMOR}, "Armor", "Steel Chainmail of Riddles"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,ARMOR}, "Armor", "Adamant Chainmail of Unholy Blight"     , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,ARMOR}, "Armor", "Poly Chainmail of Dark Magic"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,ARMOR}, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,ARMOR}, "Armor", "Bronze Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,ARMOR}, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {15,ARMOR}, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {16,ARMOR}, "Armor", "Copper Chain Rings"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {17,ARMOR}, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {18,ARMOR}, "Armor", "Gold Chain Rings"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {19,ARMOR}, "Armor", "Mithril Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {20,ARMOR}, "Armor", "Charcoal Chain Rings"                   , "", 25.0f, 1, 360.0f, 0.5f },
    { {21,ARMOR}, "Armor", "Steel Chain Rings"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { {22,ARMOR}, "Armor", "Adamant Chain Rings"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {23,ARMOR}, "Armor", "Poly Chain Rings"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {24,ARMOR}, "Armor", "Greatcloak of Blind Whispers"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {25,ARMOR}, "Armor", "Drape of Blessed Glory"                 , "", 25.0f, 1, 360.0f, 0.5f },
    { {26,ARMOR}, "Armor", "Wool Mantle of Distant Powers"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {27,ARMOR}, "Armor", "Possessed Scaled Shroud"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {28,ARMOR}, "Armor", "Cloth Greatcloak of Haunted Justice"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {29,ARMOR}, "Armor", "Exile Embroided Wrap"                   , "", 25.0f, 1, 360.0f, 0.5f },
    { {30,ARMOR}, "Armor", "Warlord's Padded Greatcloak"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {31,ARMOR}, "Armor", "Lightning Mantle of Secrets"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {32,ARMOR}, "Armor", "Dragon Shroud of the Talon"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {33,ARMOR}, "Armor", "Doom's Wool Drape"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { {34,ARMOR}, "Armor", "Piece of Cloth"                         , "", 25.0f, 1, 360.0f, 0.5f },
    { {35,ARMOR}, "Armor", "Dirty Rags"                             , "", 25.0f, 1, 360.0f, 0.5f },
    { {36,ARMOR}, "Armor", "Copper Chestguard of Fleeting Kings"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {37,ARMOR}, "Armor", "Bronze Chestguard of Hallowed Wars"     , "", 25.0f, 1, 360.0f, 0.5f },
    { {38,ARMOR}, "Armor", "Steel Breastplate"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { {39,ARMOR}, "Armor", "Studded Steel Breastplate"              , "", 25.0f, 1, 360.0f, 0.5f },
    { {40,ARMOR}, "Armor", "Mithril Chestgaurd of Conquered Worlds" , "", 25.0f, 1, 360.0f, 0.5f },
    { {41,ARMOR}, "Armor", "Steel Plated Cuirass"                   , "", 25.0f, 1, 360.0f, 0.5f },
    { {42,ARMOR}, "Armor", "Heavy Steel Plated Cuirass"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {43,ARMOR}, "Armor", "Storm-Forged Steel Cuirass of the Gods" , "", 25.0f, 1, 360.0f, 0.5f },
    { {44,ARMOR}, "Armor", "Common Clothes"                         , "", 25.0f, 1, 360.0f, 0.5f },
    { {45,ARMOR}, "Armor", "Kimono of the Apprentice"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {46,ARMOR}, "Armor", "Kimono of Festivities"                  , "", 25.0f, 1, 360.0f, 0.5f },
    { {47,ARMOR}, "Armor", "Kimono of the Western Sea"              , "", 25.0f, 1, 360.0f, 0.5f },
    { {48,ARMOR}, "Armor", "Kimono of the Fisherman"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {49,ARMOR}, "Armor", "Common Headscarf"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {50,ARMOR}, "Armor", "Desert Headscarf"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {51,ARMOR}, "Armor", "Red Cape of Riding"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {52,ARMOR}, "Armor", "Iron Chainmail"                         , "", 25.0f, 1, 360.0f, 0.5f },
    { {53,ARMOR}, "Armor", "Gold Chainmail"                         , "", 25.0f, 1, 360.0f, 0.5f },
    { {54,ARMOR}, "Armor", "Steel Chaimail"                         , "", 25.0f, 1, 360.0f, 0.5f },
    { {55,ARMOR}, "Armor", "Bronze Chainmail"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {56,ARMOR}, "Armor", "Charcoal Chainmail"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {57,ARMOR}, "Armor", "Wooden Chest Armor"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {58,ARMOR}, "Armor", "Hide Skin"                              , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item books[] = {
    { { 0,BOOK}, "Book", "Duke Of The Nation"           , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,BOOK}, "Book", "Criminal Of The Night"        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,BOOK}, "Book", "Humans Without Duty"          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,BOOK}, "Book", "Descendants Of The Day"       , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,BOOK}, "Book", "Slaves And Friends"           , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,BOOK}, "Book", "Defenders And Swindlers"      , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,BOOK}, "Book", "Ruins With Money"             , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,BOOK}, "Book", "Sword Of History"             , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,BOOK}, "Book", "Dead At The Beginning"        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,BOOK}, "Book", "Cleaning Up My Destiny"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,BOOK}, "Book", "Defender With Pride"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,BOOK}, "Book", "Human Without Courage"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,BOOK}, "Book", "Blacksmiths Of Hell"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,BOOK}, "Book", "Invaders Of The Great"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,BOOK}, "Book", "Slaves And Phantoms"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {15,BOOK}, "Book", "Heroes And Heirs"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {16,BOOK}, "Book", "Vengeance Of Heaven"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {17,BOOK}, "Book", "Culmination Of Fire"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {18,BOOK}, "Book", "Commanding The Depths"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {19,BOOK}, "Book", "Searching In Nightmares"      , "", 25.0f, 1, 360.0f, 0.5f },
    { {20,BOOK}, "Book", "Butcher Of The Ancestors"     , "", 25.0f, 1, 360.0f, 0.5f },
    { {21,BOOK}, "Book", "Priestess Of The Great"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {22,BOOK}, "Book", "Hunters Without A Goal"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {23,BOOK}, "Book", "Wolves Without Courage"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {24,BOOK}, "Book", "Bandits And Foreigners"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {25,BOOK}, "Book", "Strangers And Foes"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {26,BOOK}, "Book", "Crossbow Of The Ancestors"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {27,BOOK}, "Book", "Luck Of Dread"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {28,BOOK}, "Book", "Challenging The Immortals"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {29,BOOK}, "Book", "Dwelling In The Emperor"      , "", 25.0f, 1, 360.0f, 0.5f },
    { {30,BOOK}, "Book", "Criminal Of Utopia"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {31,BOOK}, "Book", "Friend Without Time"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {32,BOOK}, "Book", "Enemies With Silver"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {33,BOOK}, "Book", "Priests Of The Plague"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {34,BOOK}, "Book", "Giants And Heirs"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {35,BOOK}, "Book", "Hunters And Mice"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {36,BOOK}, "Book", "Nation Without Flaws"         , "", 25.0f, 1, 360.0f, 0.5f },
    { {37,BOOK}, "Book", "Revenge Of The Frontline"     , "", 25.0f, 1, 360.0f, 0.5f },
    { {38,BOOK}, "Book", "Running In The Hunter"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {39,BOOK}, "Book", "Rescue At The Mist"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {40,BOOK}, "Book", "Gangster With Immortality"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {41,BOOK}, "Book", "Blacksmith Of Freedom"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {42,BOOK}, "Book", "Wives Without Direction"      , "", 25.0f, 1, 360.0f, 0.5f },
    { {43,BOOK}, "Book", "Women With Silver"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {44,BOOK}, "Book", "Spies And Strangers"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {45,BOOK}, "Book", "Thieves And Kings"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {46,BOOK}, "Book", "World Of The Ancients"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {47,BOOK}, "Book", "Effect Of Hope"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {48,BOOK}, "Book", "Deceiving The Forest"         , "", 25.0f, 1, 360.0f, 0.5f },
    { {49,BOOK}, "Book", "Remember The Forest"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {50,BOOK}, "Book", "Woman Of Glory"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {51,BOOK}, "Book", "Agent With Silver"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {52,BOOK}, "Book", "Foes Of The River"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {53,BOOK}, "Book", "Robots With Money"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {54,BOOK}, "Book", "Women And Criminals"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {55,BOOK}, "Book", "Officers And Rats"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {56,BOOK}, "Book", "Country Of Glory"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {57,BOOK}, "Book", "Will Of My Imagination"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {58,BOOK}, "Book", "Traces In The Jungle"         , "", 25.0f, 1, 360.0f, 0.5f },
    { {59,BOOK}, "Book", "Dancing In The Animals"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {60,BOOK}, "Book", "Assassin Of Stone"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {61,BOOK}, "Book", "Criminal Of Earth"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {62,BOOK}, "Book", "Blacksmiths Of The Stockades" , "", 25.0f, 1, 360.0f, 0.5f },
    { {63,BOOK}, "Book", "Humans With Pride"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {64,BOOK}, "Book", "Owls And Dogs"                , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item boots[] = {
    { {0,BOOT}, "Boots", "Footpads of Binding Lands"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {1,BOOT}, "Boots", "Sabatons of Conquered Damnation" , "", 25.0f, 1, 360.0f, 0.5f },
    { {2,BOOT}, "Boots", "Crazed Bronzed Greatboots"       , "", 25.0f, 1, 360.0f, 0.5f },
    { {3,BOOT}, "Boots", "Grieving Adamantite Sabatons"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {4,BOOT}, "Boots", "Ebon Skeletal Greaves"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {5,BOOT}, "Boots", "Iron Walkers of Hallowed Souls"  , "", 25.0f, 1, 360.0f, 0.5f },
    { {6,BOOT}, "Boots", "Thunderfury Skeletal Greaves"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {7,BOOT}, "Boots", "Hollow Feet of Nature"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {8,BOOT}, "Boots", "Challenger's Quilted Boots"      , "", 25.0f, 1, 360.0f, 0.5f },
    { {9,BOOT}, "Boots", "Sprinters of the Champion"       , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item chests[] = {
    { { 0,CHEST}, "Chest", "Small Locked Chest"               , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,CHEST}, "Chest", "Large Locked Chest"               , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,CHEST}, "Chest", "Broken Chest"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,CHEST}, "Chest", "Small Locked Safe"                , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,CHEST}, "Chest", "Large Locked Safe"                , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,CHEST}, "Chest", "Broken Safe"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,CHEST}, "Chest", "Crystal Enclosure"                , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,CHEST}, "Chest", "Broken Crystal Enclosure"         , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,CHEST}, "Chest", "Gold Pouch"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,CHEST}, "Chest", "Mystic Gold Pouch"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,CHEST}, "Chest", "Expensive Gold Pouch"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,CHEST}, "Chest", "Hungry Gold Pouch"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,CHEST}, "Chest", "Barrel of Common Goods"           , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,CHEST}, "Chest", "Barrel of Rare Goods"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,CHEST}, "Chest", "Broken Barrel of Common Goods"    , "", 25.0f, 1, 360.0f, 0.5f },
    { {15,CHEST}, "Chest", "Broken Barrel of Expensive Goods" , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item fleshes[] = {
    { { 0,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {33,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {34,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {35,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {36,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {37,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {38,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {39,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {40,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {41,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {42,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {43,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {44,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {45,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {46,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {47,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {48,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {49,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {50,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {51,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {52,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {53,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {54,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {55,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {56,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {57,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {58,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {59,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {60,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
    { {61,FLESH}, "Flesh", "Rotting Flesh", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item foods[] = {
    { { 0,FOOD}, "Food", "Cramp Lion"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,FOOD}, "Food", "Stag Ear"                          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,FOOD}, "Food", "Luminescent Lichen"                , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,FOOD}, "Food", "Orange Jelly Drop"                 , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,FOOD}, "Food", "Mossy Hair Lichen"                 , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,FOOD}, "Food", "Scaly Swamp Russula"               , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,FOOD}, "Food", "Hypoxylon Lichen"                  , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,FOOD}, "Food", "Questionable Felt Lichen"          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,FOOD}, "Food", "Marasmiellus Spindles"             , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,FOOD}, "Food", "Powdered Pored Bolete"             , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,FOOD}, "Food", "Fly Blewit"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,FOOD}, "Food", "Snow-white Egg Bird's Nest Fungus" , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,FOOD}, "Food", "Viscid Destroyer Fungi"            , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,FOOD}, "Food", "Tumbling Waxcap"                   , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,FOOD}, "Food", "Cloudy Widow"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { {15,FOOD}, "Food", "Green-belted Mans Fingers"         , "", 25.0f, 1, 360.0f, 0.5f },
    { {16,FOOD}, "Food", "Hygroscopic Waxycap"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {17,FOOD}, "Food", "Onger"                             , "", 25.0f, 1, 360.0f, 0.5f },
    { {18,FOOD}, "Food", "Nilery"                            , "", 25.0f, 1, 360.0f, 0.5f },
    { {19,FOOD}, "Food", "Kime"                              , "", 25.0f, 1, 360.0f, 0.5f },
    { {20,FOOD}, "Food", "Sun Guanabana"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {21,FOOD}, "Food", "Iggeomquat"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { {22,FOOD}, "Food", "River Mango"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {23,FOOD}, "Food", "Klorang"                           , "", 25.0f, 1, 360.0f, 0.5f },
    { {24,FOOD}, "Food", "Sun Cawesh"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { {25,FOOD}, "Food", "Summer Babaco"                     , "", 25.0f, 1, 360.0f, 0.5f },
    { {26,FOOD}, "Food", "Dragon Bean"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {27,FOOD}, "Food", "Kastrialon"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { {28,FOOD}, "Food", "Teobaco"                           , "", 25.0f, 1, 360.0f, 0.5f },
    { {29,FOOD}, "Food", "Wrurreorang"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { {30,FOOD}, "Food", "Eale"                              , "", 25.0f, 1, 360.0f, 0.5f },
    { {31,FOOD}, "Food", "Ogliaguaro"                        , "", 25.0f, 1, 360.0f, 0.5f },
    { {32,FOOD}, "Food", "Natine"                            , "", 25.0f, 1, 360.0f, 0.5f },
    { {33,FOOD}, "Food", "Lembas"                            , "", 25.0f, 1, 360.0f, 0.5f },
    { {34,FOOD}, "Food", "Lembas Delight"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {35,FOOD}, "Food", "Klegriariac Bonbons"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {36,FOOD}, "Food", "Crown Honey Snacks"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {37,FOOD}, "Food", "Barrel of Mead"                    , "", 25.0f, 1, 360.0f, 0.5f },
    { {38,FOOD}, "Food", "Lump of Honey Sugar"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {39,FOOD}, "Food", "River Mango Pie"                   , "", 25.0f, 1, 360.0f, 0.5f },
    { {40,FOOD}, "Food", "Natine Chocolate Bar"              , "", 25.0f, 1, 360.0f, 0.5f },
    { {41,FOOD}, "Food", "Bruggearind Fortune Cake"          , "", 25.0f, 1, 360.0f, 0.5f },
    { {42,FOOD}, "Food", "Slossimelo Sweet Bun"              , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item gloves[] = {
    { {0,GLOVE}, "Gloves", "Honor's Embroided Handguards"   , "", 25.0f, 1, 360.0f, 0.5f },
    { {1,GLOVE}, "Gloves", "Quilted Gloves"                 , "", 25.0f, 1, 360.0f, 0.5f },
    { {2,GLOVE}, "Gloves", "Vengeance Quilted Hands"        , "", 25.0f, 1, 360.0f, 0.5f },
    { {3,GLOVE}, "Gloves", "Gladiator's Gloves of Immortal" , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item hats[] = {
    { { 0,HAT}, "Hat", "Pot of Cooking"                       , "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,HAT}, "Hat", "Soldier's Titanium Headcover"         , "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,HAT}, "Hat", "Plate Faceguard of Condemned Worlds"  , "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,HAT}, "Hat", "Vindication Steel Headcover"          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,HAT}, "Hat", "Ivory Gaze of Fleeting Sorrow"        , "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,HAT}, "Hat", "Vindication Faceguard of Slaughter"   , "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,HAT}, "Hat", "Hide Helm of Unholy Visions"          , "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,HAT}, "Hat", "Steel Cap of Relentless Visions"      , "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,HAT}, "Hat", "Steel Facemask of Relentless Visions" , "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,HAT}, "Hat", "Desolation Cloth Coif"                , "", 25.0f, 1, 360.0f, 0.5f },
    { {10,HAT}, "Hat", "Wizard's Deep Blue Hat"               , "", 25.0f, 1, 360.0f, 0.5f },
    { {11,HAT}, "Hat", "Dunce Cap"                            , "", 25.0f, 1, 360.0f, 0.5f },
    { {12,HAT}, "Hat", "Metal Head Ring"                      , "", 25.0f, 1, 360.0f, 0.5f },
    { {13,HAT}, "Hat", "Crown of the Blackthorn"              , "", 25.0f, 1, 360.0f, 0.5f },
    { {14,HAT}, "Hat", "Crown of the Shattered Plain's King"  , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item keys[] = {
    { {0,KEY}, "Key", "Silver Key", "", 25.0f, 1, 360.0f, 0.5f },
    { {1,KEY}, "Key", "Iron Key"  , "", 25.0f, 1, 360.0f, 0.5f },
    { {2,KEY}, "Key", "ID Card"   , "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item lights[] = {
    { {0,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {1,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {2,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {3,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {4,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {5,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {6,LIGHT}, "Light", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item letter[] = {
    { {0,LETTER}, "Letter", "Letter to ...", "This is a sample letter\n" "This is a sample letter\n", 0,0,0,0 },
    { {1,LETTER}, "Letter", "Letter to ...", "Different quests have different letters.\n" "Different quests have different letters.\n", 0,0,0,0 },
};

static const Item longweps[] = {
    { { 0,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,LONGWEP}, "Longwep", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item medweps[] = {
    { {0,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {1,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {2,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {3,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {4,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {5,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {6,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {7,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {8,MEDWEP}, "Medwep", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item monies[] = {
    { { 0,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {33,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {34,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {35,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {36,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {37,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {38,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {39,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {40,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {41,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {42,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {43,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {44,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {45,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {46,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {47,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {48,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {49,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {50,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {51,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {52,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {53,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {54,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {55,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {56,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {57,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {58,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {59,MONEY}, "Money", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item musics[] = {
    { { 0,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,MUSIC}, "Music", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item potions[] = {
    { { 0,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {33,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {34,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {35,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {36,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {37,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {38,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {39,POTION}, "Potion", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item rings[] = {
    { { 0,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {33,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {34,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {35,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {36,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {37,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {38,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {39,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {40,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {41,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {42,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {43,RING}, "Ring", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item rocks[] = {
    { {0,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {1,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {2,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {3,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {4,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {5,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {6,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {7,ROCK}, "Rock", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item scrolls[] = {
    { { 0,SCROLL}, "Scroll", "Odal"  , "Home"   , 25.0f, 1, 360.0f, 0.5f },
    { { 1,SCROLL}, "Scroll", "Daeg"  , "Day"    , 25.0f, 1, 360.0f, 0.5f },
    { { 2,SCROLL}, "Scroll", "Jera"  , "Harvest", 25.0f, 1, 360.0f, 0.5f },
    { { 3,SCROLL}, "Scroll", "Lagu"  , "Water"  , 25.0f, 1, 360.0f, 0.5f },
    { { 4,SCROLL}, "Scroll", "Man"   , "Human"  , 25.0f, 1, 360.0f, 0.5f },
    { { 5,SCROLL}, "Scroll", "Eh"    , "Horse"  , 25.0f, 1, 360.0f, 0.5f },
    { { 6,SCROLL}, "Scroll", "Beorc" , "Puriry" , 25.0f, 1, 360.0f, 0.5f },
    { { 7,SCROLL}, "Scroll", "Tyr"   , "Arrow"  , 25.0f, 1, 360.0f, 0.5f },
    { { 8,SCROLL}, "Scroll", "Sigel" , "Sun"    , 25.0f, 1, 360.0f, 0.5f },
    { { 9,SCROLL}, "Scroll", "Elhaz" , "Elk"    , 25.0f, 1, 360.0f, 0.5f },
    { {10,SCROLL}, "Scroll", "Peorth", "Dice"   , 25.0f, 1, 360.0f, 0.5f },
    { {11,SCROLL}, "Scroll", "Eoh"   , "Yew"    , 25.0f, 1, 360.0f, 0.5f },
    { {12,SCROLL}, "Scroll", "Ng"    , "Hero"   , 25.0f, 1, 360.0f, 0.5f },
    { {13,SCROLL}, "Scroll", "Is"    , "Ice"    , 25.0f, 1, 360.0f, 0.5f },
    { {14,SCROLL}, "Scroll", "Nyd"   , "Need"   , 25.0f, 1, 360.0f, 0.5f },
    { {15,SCROLL}, "Scroll", "Hagal" , "Hail"   , 25.0f, 1, 360.0f, 0.5f },
    { {16,SCROLL}, "Scroll", "Wynn"  , "Wind"   , 25.0f, 1, 360.0f, 0.5f },
    { {17,SCROLL}, "Scroll", "Gifu"  , "Gift"   , 25.0f, 1, 360.0f, 0.5f },
    { {18,SCROLL}, "Scroll", "Ken"   , "Wood"   , 25.0f, 1, 360.0f, 0.5f },
    { {19,SCROLL}, "Scroll", "Rad"   , "Wheel"  , 25.0f, 1, 360.0f, 0.5f },
    { {20,SCROLL}, "Scroll", "As"    , "God"    , 25.0f, 1, 360.0f, 0.5f },
    { {21,SCROLL}, "Scroll", "Thorn" , "Giant"  , 25.0f, 1, 360.0f, 0.5f },
    { {22,SCROLL}, "Scroll", "Ur"    , "Ox"     , 25.0f, 1, 360.0f, 0.5f },
    { {23,SCROLL}, "Scroll", "Feoh"  , "Cattle" , 25.0f, 1, 360.0f, 0.5f },
};

static const Item shields[] = {
    { {0,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {1,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {2,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {3,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {4,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {5,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {6,SHIELD}, "Shield", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item shortweps[] = {
    { { 0,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,SHORTWEP}, "Shortwep", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item tools[] = { // PRYING OPEN MY THIRD EYE.
    { { 0,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,TOOL}, "Tool", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static const Item wands[] = {
    { { 0,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 1,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 2,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 3,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 4,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 5,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 6,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 7,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 8,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { { 9,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {10,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {11,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {12,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {13,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {14,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {15,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {16,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {17,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {18,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {19,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {20,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {21,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {22,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {23,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {24,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {25,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {26,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {27,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {28,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {29,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {30,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {31,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {32,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {33,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {34,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {35,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {36,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {37,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {38,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {39,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {40,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {41,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {42,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {43,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {44,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {45,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {46,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {47,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
    { {48,WAND}, "Wand", "", "", 25.0f, 1, 360.0f, 0.5f },
};

static void check(const Classification c, const Item items[], const int len)
{
    assert(len == c_max_indices(c));
    for(int i = 0; i < len; i++)
        assert(items[i].id.index == i);
}

Item i_none(void)
{
    static Item zero;
    Item item = zero;
    item.id.clas = NONE;
    return item;
}

Item i_new(const Identification id)
{
    switch(id.clas)
    {
    // Do not use default.
    // Will remind of any new items that are missing.
    case CLASSIFICATIONS:
    case     NONE: return i_none();
    case     AMMO: return ammos     [ id.index ];
    case   AMULET: return amulets   [ id.index ];
    case    ARMOR: return armors    [ id.index ];
    case     BOOK: return books     [ id.index ];
    case     BOOT: return boots     [ id.index ];
    case    CHEST: return chests    [ id.index ];
    case    FLESH: return fleshes   [ id.index ];
    case     FOOD: return foods     [ id.index ];
    case    GLOVE: return gloves    [ id.index ];
    case      HAT: return hats      [ id.index ];
    case      KEY: return keys      [ id.index ];
    case   LETTER: return letter    [ id.index ];
    case    LIGHT: return lights    [ id.index ];
    case  LONGWEP: return longweps  [ id.index ];
    case   MEDWEP: return medweps   [ id.index ];
    case    MONEY: return monies    [ id.index ];
    case    MUSIC: return musics    [ id.index ];
    case   POTION: return potions   [ id.index ];
    case     RING: return rings     [ id.index ];
    case     ROCK: return rocks     [ id.index ];
    case   SCROLL: return scrolls   [ id.index ];
    case   SHIELD: return shields   [ id.index ];
    case SHORTWEP: return shortweps [ id.index ];
    case     TOOL: return tools     [ id.index ];
    case     WAND: return wands     [ id.index ];
    }
    return i_none();
}

Item i_rand(void)
{
    const Classification c = c_rand();
    const int index = rand() % c_max_indices(c);
    const Identification id = { index, c };
    return i_new(id);
}

void i_test(void)
{
    check(AMMO    , ammos    , u_len(ammos)    );
    check(AMULET  , amulets  , u_len(amulets)  );
    check(ARMOR   , armors   , u_len(armors)   );
    check(BOOK    , books    , u_len(books)    );
    check(BOOT    , boots    , u_len(boots)    );
    check(CHEST   , chests   , u_len(chests)   );
    check(FLESH   , fleshes  , u_len(fleshes)  );
    check(FOOD    , foods    , u_len(foods)    );
    check(GLOVE   , gloves   , u_len(gloves)   );
    check(HAT     , hats     , u_len(hats)     );
    check(KEY     , keys     , u_len(keys)     );
    check(LETTER  , letter   , u_len(letter)   );
    check(LIGHT   , lights   , u_len(lights)   );
    check(LONGWEP , longweps , u_len(longweps) );
    check(MEDWEP  , medweps  , u_len(medweps)  );
    check(MONEY   , monies   , u_len(monies)   );
    check(MUSIC   , musics   , u_len(musics)   );
    check(POTION  , potions  , u_len(potions)  );
    check(RING    , rings    , u_len(rings)    );
    check(ROCK    , rocks    , u_len(rocks)    );
    check(SCROLL  , scrolls  , u_len(scrolls)  );
    check(SHIELD  , shields  , u_len(shields)  );
    check(SHORTWEP, shortweps, u_len(shortweps));
    check(TOOL    , tools    , u_len(tools)    );
    check(WAND    , wands    , u_len(wands)    );
}
