#ifndef ITEMS_H
#define ITEMS_H

#define NUM_STATS 5
#define MAX_ITEM_ID 100
#define LUCK_INFLUENCE 66   // Higher = more consistent high rolls
#define VARIANCE_PERCENT 20 // Items vary by +/- 20%

typedef enum {
	ITEM_ID_NONE 				  =  0,
    // ITEM_CHARM_STAT
    ITEM_ID_MINOR_HEALTH_CHARM    = 10,
    ITEM_ID_MAJOR_HEALTH_CHARM    = 11,
    ITEM_ID_SUPERIOR_HEALTH_CHARM = 12,
    ITEM_ID_MINOR_ATTACK_CHARM    = 13,
    ITEM_ID_MAJOR_ATTACK_CHARM    = 14,
    ITEM_ID_SUPERIOR_ATTACK_CHARM = 15,
    ITEM_ID_MINOR_DEFENSE_CHARM   = 16,
    ITEM_ID_MAJOR_DEFENSE_CHARM   = 17,
    ITEM_ID_SUPERIOR_DEFENSE_CHARM= 18,
    ITEM_ID_MINOR_SPEED_CHARM     = 19,
    ITEM_ID_MAJOR_SPEED_CHARM     = 20,
    ITEM_ID_SUPERIOR_SPEED_CHARM  = 21,
    ITEM_ID_MINOR_LUCK_CHARM      = 22,
    ITEM_ID_MAJOR_LUCK_CHARM      = 23,
    ITEM_ID_SUPERIOR_LUCK_CHARM   = 24,

    // ITEM_CHARM_LEGENDARY
    ITEM_ID_LEGENDARY_SOULSTONE   = 30,

    // ITEM_EQUIPMENT - Chestplates
    ITEM_ID_LEATHER_TUNIC         = 40,
    ITEM_ID_CHAINMAIL_PLATE       = 41,
    ITEM_ID_DRACONIC_ARMOR        = 42,

    // ITEM_EQUIPMENT - Swords
    ITEM_ID_RUSTY_DAGGER          = 50,
    ITEM_ID_STEEL_LONGSWORD       = 51,
    ITEM_ID_RUNIC_BLADE           = 52,

    // ITEM_EQUIPMENT - Shields
    ITEM_ID_WOODEN_SHIELD         = 60,
    ITEM_ID_KITE_SHIELD           = 61,
    ITEM_ID_AEGIS_SHIELD          = 62,

    // ITEM_EQUIPMENT - Boots
    ITEM_ID_CLOTH_WRAPS           = 70,
    ITEM_ID_LEATHER_BOOTS         = 71,
    ITEM_ID_HERMES_SANDALS        = 72,

    // ITEM_EQUIPMENT - Rings
    ITEM_ID_COPPER_BAND           = 80,
    ITEM_ID_SILVER_SIGNET         = 81,
    ITEM_ID_MIDAS_TOUCH           = 82,

    // ITEM_HEALING
    ITEM_ID_SMALL_POTION          = 90,
    ITEM_ID_MEDIUM_POTION         = 91,
    ITEM_ID_LARGE_POTION          = 92,
} ItemID;

typedef enum {
	ITEM_NONE,
    ITEM_CHARM_STAT,      // permanent stat boost, auto-apply
    ITEM_CHARM_LEGENDARY, // 3 allocatable points
    ITEM_EQUIPMENT,       // slotted, one per stat, tiered
    ITEM_HEALING,         // tiered potions, restores HP with some variance
} ItemType;

typedef enum {
	STAT_NONE,
	STAT_HEALTH,	// Chestplates
    STAT_ATTACK,	// Swords
	STAT_DEFENSE,	// Shields
	STAT_SPEED,		// Boots
	STAT_LUCK,		// Rings
} StatType;

typedef struct {
	const char *name;
	ItemID itemID;  // unqiue identifier for mapping items -> bitmap for images
    ItemType type;
    StatType stat;         // which stat it affects (ignored for healing/legendary)
    unsigned int tier;    // 1-3
    unsigned int value;   // base value, rolled with some variance on pickup
} Item;

extern Item inventory[];					// player.c
extern Item equipment[];					// player.c
extern const Item ITEM_CATALOG[];			// items.c
extern const unsigned char CATALOG_SIZE;	// items.c

static const Item* findInCatalog(ItemType type, StatType stat, unsigned int tier);
unsigned int getBaseValueFromCatalog(ItemType type, StatType stat, unsigned int tier);
const char* getNameFromCatalog(ItemType type, StatType stat, unsigned int tier);
unsigned int getIDFromCatalog(ItemType type, StatType stat, unsigned int tier);
unsigned int rollStat(unsigned int baseValue, unsigned int luck);
unsigned int rollTier(unsigned int luck);
Item generateItem(unsigned int luck);

#endif
