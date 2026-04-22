#ifndef ITEMS_H
#define ITEMS_H

#define NUM_STATS 5
#define LUCK_INFLUENCE 0.66f   // Higher = more consistent high rolls
#define VARIANCE_PERCENT 0.20f // Items vary by +/- 20%

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
	unsigned char itemID;  // unqiue identifier for mapping items -> bitmap for images
    ItemType type;
    StatType stat;         // which stat it affects (ignored for healing/legendary)
    unsigned char tier;    // 1-3
    unsigned char value;   // base value, rolled with some variance on pickup
} Item;

extern Item inventory[];					// player.c
extern Item equipment[];					// player.c
extern const Item ITEM_CATALOG[];			// items.c
extern const unsigned char CATALOG_SIZE;	// items.c

unsigned char rollStat(unsigned char baseValue, unsigned char luck);
unsigned char rollTier(unsigned char luck);

#endif
