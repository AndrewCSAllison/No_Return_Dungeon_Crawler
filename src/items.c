#include "items.h"
#include "random.h"

/* * ITEM_CATALOG Definition
 * Values are scaled based on a 100/255 base/max stat ratio.
 * Tiers 1-3 provide progressively higher base values.
 */
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
    {"Wooden Lid",         60, ITEM_EQUIPMENT, STAT_DEFENSE, 1, 8},
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

/**
 * Internal helper to find a matching item in the catalog.
 * Returns a pointer to the Item or NULL if not found.
 */
static const Item* find_in_catalog(ItemType type, StatType stat, unsigned char tier) {
    for (unsigned char i = 0; i < CATALOG_SIZE; i++) {
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

unsigned char getBaseValueFromCatalog(ItemType type, StatType stat, unsigned char tier) {
    const Item* item = find_in_catalog(type, stat, tier);
    return item ? item->value : 0;
}

const char* getNameFromCatalog(ItemType type, StatType stat, unsigned char tier) {
    const Item* item = find_in_catalog(type, stat, tier);
    return item ? item->name : "Unknown Item";
}

unsigned char getIDFromCatalog(ItemType type, StatType stat, unsigned char tier) {
    const Item* item = find_in_catalog(type, stat, tier);
    return item ? item->itemID : 0;
}

unsigned char rollStats(unsigned char baseValue, unsigned char luck) {
    float luckFactor = luck / 255.0f;
    int maxVariance = (int)(baseValue * VARIANCE_PERCENT);

    // This is the "floor" of our random roll (based on player luck)
    int minRoll = (int)(maxVariance * luckFactor * LUCK_INFLUENCE);

    // The range of the remaining randomness to roll for
    int remainingRange = maxVariance - minRoll;

    return baseValue + minRoll + (unsigned char)rand(remainingRange + 1);
}

// Weighted thresholds for different tiered items
unsigned char rollTier(unsigned char luck) {
    float luckFactor = luck / 255.0f;

    // We roll a value from 0-100 to determine "rarity"
    unsigned int roll = rand(101);

    // Base thresholds (Higher number = harder to hit)
    // At 0 luck: Tier 3 is top 5%, Tier 2 is top 25%, else Tier 1
    int t3_threshold = 95;
    int t2_threshold = 75;

    // Apply Luck: High luck lowers the threshold (makes higher tiers easier to hit)
    // With max luck and a 20-point swing, T3 drops from 95 to 75.
    t3_threshold -= (int)(20 * luckFactor);
    t2_threshold -= (int)(30 * luckFactor);

    if (roll >= t3_threshold) return 3;
    if (roll >= t2_threshold) return 2;

    return 1;
}

Item generateItem(unsigned char luck) {
    Item newItem;

    // 1. Roll Type (Weighted: 60% Equip, 30% Potion, 10% Charm)
    unsigned int typeRoll = rand(100);
	if (typeRoll < 10) {
		newItem.type = ITEM_CHARM_STAT;
	else if (typeRoll < 40) {
		newItem.type = ITEM_HEALING;
	} else {
		newItem.type = ITEM_EQUIPMENT;
	}

    // 2. Roll Tier (The Luck-Influenced Threshold)
    newItem.tier = rollTier(luck);

    // 3. Roll Stat (Ignore if Healing/Legendary)
    if (newItem.type != ITEM_HEALING) {
        newItem.stat = (StatType)rand(NUM_STATS);
    } else {
        newItem.stat = STAT_HEALTH; // Potions always heal
    }

    // 4. Assemble Final Value & Name
    unsigned char baseValue = getBaseValueFromCatalog(newItem.type, newItem.stat, newItem.tier);
    newItem.value = rollStats(baseValue, luck);
    newItem.name = getNameFromCatalog(newItem.type, newItem.stat, newItem.tier);
	newItem.itemID = getIDFromCatalog(newItem.type, newItem.stat, newItem.tier);

    return newItem;
}
