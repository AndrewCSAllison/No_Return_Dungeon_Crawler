#include "items.h"
#include "random.h"

// ---------------------------------------------------------------------------
// Catalog
// ---------------------------------------------------------------------------
const Item ITEM_CATALOG[] = {

    // --- ITEM_CHARM_STAT (Permanent Boosts) ---
    {"Minor Health Charm",     ITEM_ID_MINOR_HEALTH_CHARM,     ITEM_CHARM_STAT, STAT_HEALTH,  1,  3},
    {"Major Health Charm",     ITEM_ID_MAJOR_HEALTH_CHARM,     ITEM_CHARM_STAT, STAT_HEALTH,  2,  5},
    {"Superior Health Charm",  ITEM_ID_SUPERIOR_HEALTH_CHARM,  ITEM_CHARM_STAT, STAT_HEALTH,  3, 10},

    {"Minor Attack Charm",     ITEM_ID_MINOR_ATTACK_CHARM,     ITEM_CHARM_STAT, STAT_ATTACK,  1,  3},
    {"Major Attack Charm",     ITEM_ID_MAJOR_ATTACK_CHARM,     ITEM_CHARM_STAT, STAT_ATTACK,  2,  5},
    {"Superior Attack Charm",  ITEM_ID_SUPERIOR_ATTACK_CHARM,  ITEM_CHARM_STAT, STAT_ATTACK,  3, 10},

    {"Minor Defense Charm",    ITEM_ID_MINOR_DEFENSE_CHARM,    ITEM_CHARM_STAT, STAT_DEFENSE, 1,  3},
    {"Major Defense Charm",    ITEM_ID_MAJOR_DEFENSE_CHARM,    ITEM_CHARM_STAT, STAT_DEFENSE, 2,  5},
    {"Superior Defense Charm", ITEM_ID_SUPERIOR_DEFENSE_CHARM, ITEM_CHARM_STAT, STAT_DEFENSE, 3, 10},

    {"Minor Speed Charm",      ITEM_ID_MINOR_SPEED_CHARM,      ITEM_CHARM_STAT, STAT_SPEED,   1,  3},
    {"Major Speed Charm",      ITEM_ID_MAJOR_SPEED_CHARM,      ITEM_CHARM_STAT, STAT_SPEED,   2,  5},
    {"Superior Speed Charm",   ITEM_ID_SUPERIOR_SPEED_CHARM,   ITEM_CHARM_STAT, STAT_SPEED,   3, 10},

    {"Minor Luck Charm",       ITEM_ID_MINOR_LUCK_CHARM,       ITEM_CHARM_STAT, STAT_LUCK,    1,  3},
    {"Major Luck Charm",       ITEM_ID_MAJOR_LUCK_CHARM,       ITEM_CHARM_STAT, STAT_LUCK,    2,  5},
    {"Superior Luck Charm",    ITEM_ID_SUPERIOR_LUCK_CHARM,    ITEM_CHARM_STAT, STAT_LUCK,    3, 10},

    // --- ITEM_CHARM_LEGENDARY ---
    {"Legendary Soulstone",    ITEM_ID_LEGENDARY_SOULSTONE,    ITEM_CHARM_LEGENDARY, STAT_ALL, 3, 10},

    // --- ITEM_EQUIPMENT ---
    // Chestplates (STAT_HEALTH)
    {"Leather Tunic",          ITEM_ID_LEATHER_TUNIC,          ITEM_EQUIPMENT, STAT_HEALTH,  1, 15},
    {"Chainmail Plate",        ITEM_ID_CHAINMAIL_PLATE,        ITEM_EQUIPMENT, STAT_HEALTH,  2, 35},
    {"Draconic Armor",         ITEM_ID_DRACONIC_ARMOR,         ITEM_EQUIPMENT, STAT_HEALTH,  3, 80},
    // Swords (STAT_ATTACK)
    {"Rusty Dagger",           ITEM_ID_RUSTY_DAGGER,           ITEM_EQUIPMENT, STAT_ATTACK,  1, 10},
    {"Steel Longsword",        ITEM_ID_STEEL_LONGSWORD,        ITEM_EQUIPMENT, STAT_ATTACK,  2, 25},
    {"Runic Blade",            ITEM_ID_RUNIC_BLADE,            ITEM_EQUIPMENT, STAT_ATTACK,  3, 60},
    // Shields (STAT_DEFENSE)
    {"Wooden Shield",          ITEM_ID_WOODEN_SHIELD,          ITEM_EQUIPMENT, STAT_DEFENSE, 1,  8},
    {"Kite Shield",            ITEM_ID_KITE_SHIELD,            ITEM_EQUIPMENT, STAT_DEFENSE, 2, 22},
    {"Aegis Shield",           ITEM_ID_AEGIS_SHIELD,           ITEM_EQUIPMENT, STAT_DEFENSE, 3, 55},
    // Boots (STAT_SPEED)
    {"Cloth Wraps",            ITEM_ID_CLOTH_WRAPS,            ITEM_EQUIPMENT, STAT_SPEED,   1,  5},
    {"Leather Boots",          ITEM_ID_LEATHER_BOOTS,          ITEM_EQUIPMENT, STAT_SPEED,   2, 25},
    {"Hermes Sandals",         ITEM_ID_HERMES_SANDALS,         ITEM_EQUIPMENT, STAT_SPEED,   3, 50},
    // Rings (STAT_LUCK)
    {"Copper Band",            ITEM_ID_COPPER_BAND,            ITEM_EQUIPMENT, STAT_LUCK,    1,  2},
    {"Silver Signet",          ITEM_ID_SILVER_SIGNET,          ITEM_EQUIPMENT, STAT_LUCK,    2,  8},
    {"Midas Touch",            ITEM_ID_MIDAS_TOUCH,            ITEM_EQUIPMENT, STAT_LUCK,    3, 25},

    // --- ITEM_HEALING ---
    {"Small Potion",           ITEM_ID_SMALL_POTION,           ITEM_HEALING, STAT_HEALTH,    1,  25},
    {"Medium Potion",          ITEM_ID_MEDIUM_POTION,          ITEM_HEALING, STAT_HEALTH,    2,  60},
    {"Large Potion",           ITEM_ID_LARGE_POTION,           ITEM_HEALING, STAT_HEALTH,    3, 150},
};

const unsigned char CATALOG_SIZE = sizeof(ITEM_CATALOG) / sizeof(ITEM_CATALOG[0]);

// ---------------------------------------------------------------------------
// Catalog lookup
// ---------------------------------------------------------------------------
const Item *findInCatalog(ItemType type, StatType stat, unsigned int tier) {
    unsigned int i;
    for (i = 0; i < CATALOG_SIZE; i++) {
        if (ITEM_CATALOG[i].type != type) continue;
        if (ITEM_CATALOG[i].tier != tier) continue;
        if (type == ITEM_HEALING || type == ITEM_CHARM_LEGENDARY) {
            return &ITEM_CATALOG[i];
        }
        if (ITEM_CATALOG[i].stat == stat) {
            return &ITEM_CATALOG[i];
        }
    }
    return 0;
}

unsigned int getBaseValueFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item *item = findInCatalog(type, stat, tier);
    return item ? item->value : 0;
}

const char *getNameFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item *item = findInCatalog(type, stat, tier);
    return item ? item->name : "Unknown Item";
}

unsigned int getIDFromCatalog(ItemType type, StatType stat, unsigned int tier) {
    const Item *item = findInCatalog(type, stat, tier);
    return item ? item->itemID : 0;
}

// ---------------------------------------------------------------------------
// Rolling
// ---------------------------------------------------------------------------
unsigned int rollStat(unsigned int baseValue, unsigned int luck) {
    int maxVariance = (int)((baseValue * VARIANCE_PERCENT) / 100);
    if (maxVariance < MIN_VARIANCE) maxVariance = MIN_VARIANCE;

    int minRoll = (maxVariance * (int)luck * LUCK_INFLUENCE) / 25500;
    int remainingRange = maxVariance - minRoll;

    if (remainingRange <= 0) {
        return baseValue + (unsigned int)minRoll;
    }

    return baseValue + (unsigned int)minRoll + (unsigned int)rand(remainingRange + 1);
}

unsigned int rollTier(unsigned int luck, int isElite) {
    unsigned int roll = rand(101);

    int t3 = isElite ? ELITE_T3_BASE : NORMAL_T3_BASE;
    int t2 = isElite ? ELITE_T2_BASE : NORMAL_T2_BASE;

    t3 -= (20 * (int)luck) / 255;
    t2 -= (30 * (int)luck) / 255;

    if ((int)roll >= t3) return 3;
    if ((int)roll >= t2) return 2;
    return 1;
}

// ---------------------------------------------------------------------------
// Item generation
// ---------------------------------------------------------------------------

/*
 * generateItem: produces a fully resolved Item for enemy drops.
 * Enemy drops are restricted to ITEM_CHARM_STAT and ITEM_HEALING only.
 * ITEM_EQUIPMENT is reserved for chests (see generateChestItem).
 *
 * Drop distribution:
 *   Elite:   5% legendary, 55% charm, 40% healing
 *   Regular: 0% legendary, 40% charm, 60% healing
 */
Item generateItem(unsigned int luck, int isElite) {
    Item newItem;
    unsigned int typeRoll = rand(100);

    if (isElite && typeRoll < LEGENDARY_DROP_CHANCE) {
        newItem.type   = ITEM_CHARM_LEGENDARY;
        newItem.stat   = STAT_ALL;
        newItem.tier   = 3;
        newItem.value  = getBaseValueFromCatalog(ITEM_CHARM_LEGENDARY, STAT_ALL, 3);
        newItem.name   = getNameFromCatalog(ITEM_CHARM_LEGENDARY, STAT_ALL, 3);
        newItem.itemID = (ItemID)getIDFromCatalog(ITEM_CHARM_LEGENDARY, STAT_ALL, 3);
        return newItem;
    }

    if (isElite) {
        // 55% charm, 40% healing (after legendary window)
        if (typeRoll < 55) newItem.type = ITEM_CHARM_STAT;
        else               newItem.type = ITEM_HEALING;
    } else {
        // 40% charm, 60% healing
        if (typeRoll < 40) newItem.type = ITEM_CHARM_STAT;
        else               newItem.type = ITEM_HEALING;
    }

    newItem.tier = rollTier(luck, isElite);

    if (newItem.type == ITEM_HEALING) {
        newItem.stat = STAT_HEALTH;
    } else {
        newItem.stat = (StatType)(rand(NUM_ROLLABLE_STATS) + 1);
    }

    unsigned int baseValue = getBaseValueFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.value  = rollStat(baseValue, luck);
    newItem.name   = getNameFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.itemID = (ItemID)getIDFromCatalog(newItem.type, newItem.stat, newItem.tier);

    return newItem;
}

/*
 * generateChestItem: produces a fully resolved Item for chest drops.
 * Chests drop ITEM_EQUIPMENT only — stat charms and potions come from enemies.
 * Luck influences tier odds via rollTier.
 */
Item generateChestItem(unsigned int luck) {
    Item newItem;

    newItem.type = ITEM_EQUIPMENT;
    newItem.tier = rollTier(luck, 0);
    newItem.stat = (StatType)(rand(NUM_ROLLABLE_STATS) + 1);

    unsigned int baseValue = getBaseValueFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.value  = rollStat(baseValue, luck);
    newItem.name   = getNameFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.itemID = (ItemID)getIDFromCatalog(newItem.type, newItem.stat, newItem.tier);

    return newItem;
}
