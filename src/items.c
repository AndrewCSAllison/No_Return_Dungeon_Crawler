#include "items.h"
#include "random.h"

const Item ITEM_CATALOG[] = {

    // --- ITEM_CHARM_STAT (Permanent Boosts) ---
    {"Minor Health Charm",    10, ITEM_CHARM_STAT, STAT_HEALTH,  1, 3},
    {"Major Health Charm",    11, ITEM_CHARM_STAT, STAT_HEALTH,  2, 5},
    {"Superior Health Charm", 12, ITEM_CHARM_STAT, STAT_HEALTH,  3, 10},

	{"Minor Attack Charm",    13, ITEM_CHARM_STAT, STAT_ATTACK, 1, 3},
	{"Major Attack Charm",    14, ITEM_CHARM_STAT, STAT_ATTACK, 2, 5},
	{"Superior Attack Charm", 15, ITEM_CHARM_STAT, STAT_ATTACK, 3, 10},

	{"Minor Defense Charm",   16, ITEM_CHARM_STAT, STAT_DEFENSE, 1, 3},
	{"Major Defense Charm",   17, ITEM_CHARM_STAT, STAT_DEFENSE, 2, 5},
	{"Superior Defense Charm",18, ITEM_CHARM_STAT, STAT_DEFENSE, 3, 10},

	{"Minor Speed Charm",     19, ITEM_CHARM_STAT, STAT_SPEED, 1, 3},
	{"Major Speed Charm",     20, ITEM_CHARM_STAT, STAT_SPEED, 2, 5},
	{"Superior Speed Charm",  21, ITEM_CHARM_STAT, STAT_SPEED, 3, 10},

	{"Minor Luck Charm",      22, ITEM_CHARM_STAT, STAT_LUCK, 1, 3},
	{"Major Luck Charm",      23, ITEM_CHARM_STAT, STAT_LUCK, 2, 5},
	{"Superior Luck Charm",   24, ITEM_CHARM_STAT, STAT_LUCK, 3, 10},

    // --- ITEM_CHARM_LEGENDARY (3 Allocatable Points) ---
    {"Legendary Soulstone",  30, ITEM_CHARM_LEGENDARY, STAT_HEALTH, 3, 3},

    // --- ITEM_EQUIPMENT (Slotted Equipment, Tiered I-III) ---
    // STAT_HEALTH: Chestplates
    {"Leather Tunic",      40, ITEM_EQUIPMENT, STAT_HEALTH,  1, 15},
    {"Chainmail Plate",    41, ITEM_EQUIPMENT, STAT_HEALTH,  2, 35},
    {"Draconic Armor",     42, ITEM_EQUIPMENT, STAT_HEALTH,  3, 80},

    // STAT_ATTACK: Swords
    {"Rusty Dagger",       50, ITEM_EQUIPMENT, STAT_ATTACK,  1, 10},
    {"Steel Longsword",    51, ITEM_EQUIPMENT, STAT_ATTACK,  2, 25},
    {"Runic Blade",        52, ITEM_EQUIPMENT, STAT_ATTACK,  3, 60},

    // STAT_DEFENSE: Shields
    {"Wooden Shield",      60, ITEM_EQUIPMENT, STAT_DEFENSE, 1, 8},
    {"Kite Shield",        61, ITEM_EQUIPMENT, STAT_DEFENSE, 2, 22},
    {"Aegis Shield",       62, ITEM_EQUIPMENT, STAT_DEFENSE, 3, 55},

    // STAT_SPEED: Boots
    {"Cloth Wraps",        70, ITEM_EQUIPMENT, STAT_SPEED,   1, 5},
    {"Leather Boots",      71, ITEM_EQUIPMENT, STAT_SPEED,   2, 25},
    {"Hermes Sandles",     72, ITEM_EQUIPMENT, STAT_SPEED,   3, 50},

    // STAT_LUCK: Rings
    {"Copper Band",        80, ITEM_EQUIPMENT, STAT_LUCK,    1, 2},
    {"Silver Signet",      81, ITEM_EQUIPMENT, STAT_LUCK,    2, 8},
    {"Midas Touch",        82, ITEM_EQUIPMENT, STAT_LUCK,    3, 25},

    // --- ITEM_HEALING (Tiered Potions) ---
    {"Small Potion",       90, ITEM_HEALING, STAT_HEALTH,    1, 25},
    {"Medium Potion",      91, ITEM_HEALING, STAT_HEALTH,    2, 60},
    {"Large Potion",  	   92, ITEM_HEALING, STAT_HEALTH,    3, 150}
};

const unsigned char CATALOG_SIZE = sizeof(ITEM_CATALOG) / sizeof(ITEM_CATALOG[0]);

const Item* findInCatalog(ItemType type, StatType stat, unsigned int tier) {
    for (unsigned int i = 0; i < CATALOG_SIZE; i++) {
        if (ITEM_CATALOG[i].type == type && ITEM_CATALOG[i].tier == tier) {
            // Special cases: Healing and Legendary don't care about the stat field
            if (type == ITEM_HEALING || type == ITEM_CHARM_LEGENDARY) {
                return &ITEM_CATALOG[i];
            }
            // Standard check for Equipment and Stat Charms
            if (ITEM_CATALOG[i].stat == stat) {
                return &ITEM_CATALOG[i];
            }
        }
    }
    return 0; // Not found
}

unsigned int getBaseValueFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item* item = findInCatalog(type, stat, tier);
    return item ? item->value : 0;
}

const char* getNameFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item* item = findInCatalog(type, stat, tier);
    return item ? item->name : "Unknown Item";
}

unsigned int getIDFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item* item = findInCatalog(type, stat, tier);
    return item ? item->itemID : 0;
}

unsigned int rollStat(unsigned int baseValue, unsigned int luck) {
    int maxVariance = (baseValue * VARIANCE_PERCENT) / 100;

    // (maxVariance * luck * 66) / 25500
    int minRoll = (maxVariance * luck * LUCK_INFLUENCE) / 25500;
    int remainingRange = maxVariance - minRoll;

    if (remainingRange <= 0) {
        return baseValue + minRoll;
    }

    // UPDATED: Now passing (remainingRange + 1) as the parameter
    return baseValue + minRoll + (unsigned int)rand(remainingRange + 1);
}

unsigned int rollTier(unsigned int luck) {
    // UPDATED: Now passing 101 as the parameter to get 0-100
    unsigned int roll = rand(101);

    int t3_threshold = 95;
    int t2_threshold = 75;

    t3_threshold -= (20 * luck) / 255;
    t2_threshold -= (30 * luck) / 255;

    if (roll >= (unsigned int)t3_threshold) return 3;
    if (roll >= (unsigned int)t2_threshold) return 2;

    return 1;
}

Item generateItem(unsigned int luck) {
    Item newItem;

    // 1. Roll Type - UPDATED: rand(100)
    unsigned int typeRoll = rand(100);
    if (typeRoll < 10) {
        newItem.type = ITEM_CHARM_STAT;
    } else if (typeRoll < 40) {
        newItem.type = ITEM_HEALING;
    } else {
        newItem.type = ITEM_EQUIPMENT;
    }

    // 2. Roll Tier
    newItem.tier = rollTier(luck);

    // 3. Roll Stat - UPDATED: rand(NUM_STATS)
    if (newItem.type != ITEM_HEALING) {
        // Returns 1 through NUM_STATS
        newItem.stat = (StatType)(rand(NUM_STATS) + 1);
    } else {
        newItem.stat = STAT_HEALTH;
    }

    // 4. Assemble Final Item
    unsigned int baseValue = getBaseValueFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.value = rollStat(baseValue, luck);
    newItem.name = getNameFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.itemID = getIDFromCatalog(newItem.type, newItem.stat, newItem.tier);

    return newItem;
}
