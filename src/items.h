#ifndef ITEMS_H
#define ITEMS_H

// ---------------------------------------------------------------------------
// Tuning constants
// ---------------------------------------------------------------------------
#define NUM_ROLLABLE_STATS    5     // HEALTH, ATTACK, DEFENSE, SPEED, LUCK
#define MAX_ITEM_ID           100
#define LUCK_INFLUENCE        66    // Higher = more consistent high rolls
#define VARIANCE_PERCENT      25    // Items vary by up to +20%
#define MIN_VARIANCE          1     // Floor so low-base items still get variance

// Normal drop tier thresholds (0-100 roll, luck shifts thresholds down)
#define NORMAL_T3_BASE        95
#define NORMAL_T2_BASE        75

// Elite drop tier thresholds (already skewed toward higher tiers)
#define ELITE_T3_BASE         80
#define ELITE_T2_BASE         55

// Legendary drop chance on elite kill (out of 100)
#define LEGENDARY_DROP_CHANCE 5

// ---------------------------------------------------------------------------
// Enums
// ---------------------------------------------------------------------------
typedef enum {
    ITEM_ID_NONE                   =  0,
    // ITEM_CHARM_STAT
    ITEM_ID_MINOR_HEALTH_CHARM     = 10,
    ITEM_ID_MAJOR_HEALTH_CHARM     = 11,
    ITEM_ID_SUPERIOR_HEALTH_CHARM  = 12,
    ITEM_ID_MINOR_ATTACK_CHARM     = 13,
    ITEM_ID_MAJOR_ATTACK_CHARM     = 14,
    ITEM_ID_SUPERIOR_ATTACK_CHARM  = 15,
    ITEM_ID_MINOR_DEFENSE_CHARM    = 16,
    ITEM_ID_MAJOR_DEFENSE_CHARM    = 17,
    ITEM_ID_SUPERIOR_DEFENSE_CHARM = 18,
    ITEM_ID_MINOR_SPEED_CHARM      = 19,
    ITEM_ID_MAJOR_SPEED_CHARM      = 20,
    ITEM_ID_SUPERIOR_SPEED_CHARM   = 21,
    ITEM_ID_MINOR_LUCK_CHARM       = 22,
    ITEM_ID_MAJOR_LUCK_CHARM       = 23,
    ITEM_ID_SUPERIOR_LUCK_CHARM    = 24,
    // ITEM_CHARM_LEGENDARY
    ITEM_ID_LEGENDARY_SOULSTONE    = 30,
    // ITEM_EQUIPMENT - Chestplates
    ITEM_ID_LEATHER_TUNIC          = 40,
    ITEM_ID_CHAINMAIL_PLATE        = 41,
    ITEM_ID_DRACONIC_ARMOR         = 42,
    // ITEM_EQUIPMENT - Swords
    ITEM_ID_RUSTY_DAGGER           = 50,
    ITEM_ID_STEEL_LONGSWORD        = 51,
    ITEM_ID_RUNIC_BLADE            = 52,
    // ITEM_EQUIPMENT - Shields
    ITEM_ID_WOODEN_SHIELD          = 60,
    ITEM_ID_KITE_SHIELD            = 61,
    ITEM_ID_AEGIS_SHIELD           = 62,
    // ITEM_EQUIPMENT - Boots
    ITEM_ID_CLOTH_WRAPS            = 70,
    ITEM_ID_LEATHER_BOOTS          = 71,
    ITEM_ID_HERMES_SANDALS         = 72,
    // ITEM_EQUIPMENT - Rings
    ITEM_ID_COPPER_BAND            = 80,
    ITEM_ID_SILVER_SIGNET          = 81,
    ITEM_ID_MIDAS_TOUCH            = 82,
    // ITEM_HEALING
    ITEM_ID_SMALL_POTION           = 90,
    ITEM_ID_MEDIUM_POTION          = 91,
    ITEM_ID_LARGE_POTION           = 92,
} ItemID;

typedef enum {
    ITEM_NONE,
    ITEM_CHARM_STAT,        // Permanent stat boost, auto-applied on pickup
    ITEM_CHARM_LEGENDARY,   // 3 allocatable points across any stats
    ITEM_EQUIPMENT,         // Slotted, one per stat slot, tiered I-III
    ITEM_HEALING,           // Tiered potions, restores HP with variance
} ItemType;

typedef enum {
    STAT_NONE    = 0,
    STAT_HEALTH  = 1,   // Chestplates
    STAT_ATTACK  = 2,   // Swords
    STAT_DEFENSE = 3,   // Shields
    STAT_SPEED   = 4,   // Boots
    STAT_LUCK    = 5,   // Rings
    STAT_ALL     = 6,   // Legendary only — not rollable
} StatType;

// ---------------------------------------------------------------------------
// Item struct
// ---------------------------------------------------------------------------
typedef struct {
    const char  *name;
    ItemID       itemID;    // Unique identifier, used for bitmap lookup
    ItemType     type;
    StatType     stat;      // Which stat it affects (ignored for HEALING/LEGENDARY)
    unsigned int tier;      // 1-3
    unsigned int value;     // Base value; rolled with variance on pickup
} Item;

// ---------------------------------------------------------------------------
// Extern declarations
// ---------------------------------------------------------------------------
extern Item                inventory[];     // player.c
extern Item                equipment[];     // player.c
extern const Item          ITEM_CATALOG[];  // items.c
extern const unsigned char CATALOG_SIZE;   // items.c

// ---------------------------------------------------------------------------
// Function declarations
// ---------------------------------------------------------------------------
const Item  *findInCatalog(ItemType type, StatType stat, unsigned int tier);
unsigned int getBaseValueFromCatalog(ItemType type, StatType stat, unsigned int tier);
const char  *getNameFromCatalog(ItemType type, StatType stat, unsigned int tier);
unsigned int getIDFromCatalog(ItemType type, StatType stat, unsigned int tier);

unsigned int rollStat(unsigned int baseValue, unsigned int luck);
unsigned int rollTier(unsigned int luck, int isElite);  // isElite: 0 = regular, 1 = elite

Item generateItem(unsigned int luck, int isElite);
Item generateChestItem(unsigned int luck);

#endif
