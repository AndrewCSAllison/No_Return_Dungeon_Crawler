#ifndef TEXTURES_H
#define TEXTURES_H

#include "grid.h"
#include "items.h"
#include "player.h"
#include "enemy.h"

// TILES
#include "textures/tiles/floor_48x48.c"
#include "textures/tiles/wall_48x48.c"
#include "textures/tiles/door_closed_48x48.c"
#include "textures/tiles/door_open_48x48.c"
#include "textures/tiles/ladder_48x48.c"
#include "textures/tiles/fire_48x48.c"
#include "textures/tiles/acid_48x48.c"
#include "textures/tiles/spikes_48x48.c"
#include "textures/tiles/chest_48x48.c"
#include "textures/tiles/altar_48x48.c"
#include "textures/tiles/cauldron_48x48.c"
#include "textures/tiles/bookshelf_48x48.c"
#include "textures/tiles/coffin_48x48.c"
#include "textures/tiles/statue_48x48.c"
#include "textures/tiles/table_48x48.c"

// ITEMS
#include "textures/items/emptyInventory_64x64.c"
#include "textures/items/minorHealthCharm_64x64.c"
#include "textures/items/majorHealthCharm_64x64.c"
#include "textures/items/superiorHealthCharm_64x64.c"
#include "textures/items/minorAttackCharm_64x64.c"
#include "textures/items/majorAttackCharm_64x64.c"
#include "textures/items/superiorAttackCharm_64x64.c"
#include "textures/items/minorDefenseCharm_64x64.c"
#include "textures/items/majorDefenseCharm_64x64.c"
#include "textures/items/superiorDefenseCharm_64x64.c"
#include "textures/items/minorSpeedCharm_64x64.c"
#include "textures/items/majorSpeedCharm_64x64.c"
#include "textures/items/superiorSpeedCharm_64x64.c"
#include "textures/items/minorLuckCharm_64x64.c"
#include "textures/items/majorLuckCharm_64x64.c"
#include "textures/items/superiorLuckCharm_64x64.c"
#include "textures/items/legendarySoulstone_64x64.c"
#include "textures/items/leatherTunic_64x64.c"
#include "textures/items/chainmailPlate_64x64.c"
#include "textures/items/draconicArmor_64x64.c"
#include "textures/items/rustyDagger_64x64.c"
#include "textures/items/steelLongsword_64x64.c"
#include "textures/items/runicBlade_64x64.c"
#include "textures/items/woodenShield_64x64.c"
#include "textures/items/kiteShield_64x64.c"
#include "textures/items/aegisShield_64x64.c"
#include "textures/items/clothWraps_64x64.c"
#include "textures/items/leatherBoots_64x64.c"
#include "textures/items/hermesSandals_64x64.c"
#include "textures/items/copperBand_64x64.c"
#include "textures/items/silverSignet_64x64.c"
#include "textures/items/midasTouch_64x64.c"
#include "textures/items/smallPotion_64x64.c"
#include "textures/items/mediumPotion_64x64.c"
#include "textures/items/largePotion_64x64.c"

// PLAYER
#include "textures/player/playerNorth1.c"
#include "textures/player/playerNorth2.c"
#include "textures/player/playerEast1.c"
#include "textures/player/playerEast2.c"
#include "textures/player/playerSouth1.c"
#include "textures/player/playerSouth2.c"
#include "textures/player/playerWest1.c"
#include "textures/player/playerWest2.c"

// ENEMIES
#include "textures/enemies/goblin_48x48.c"
#include "textures/enemies/goo_48x48.c"
#include "textures/enemies/mage_48x48.c"
#include "textures/enemies/magicShield_48x48.c"
#include "textures/enemies/skeleton_48x48.c"

// SPLASH ART
#include "textures/splashArt/startScreen.c"
#include "textures/splashArt/endScreen.c"

static const guint8 *tile_textures[] = {
    // Standard Navigation
	[FLOOR]       = floor_tex.pixel_data,
	[WALL]		  = wall_tex.pixel_data,
	[DOOR_CLOSED] = door_closed_tex.pixel_data,
	[DOOR_OPEN]   = door_open_tex.pixel_data,
	[LADDER]	  = ladder_tex.pixel_data,

	// Damage Tiles
	[FIRE]        = fire_tex.pixel_data,
	[ACID]        = acid_tex.pixel_data,
	[SPIKES]	  = spikes_tex.pixel_data,

	// Interactive Objects
	[CHEST]       = chest_tex.pixel_data,
	[ALTAR]       = altar_tex.pixel_data,
	[CAULDRON]    = cauldron_tex.pixel_data,

	// Decorative Objects
	[BOOKSHELF]   = bookshelf_tex.pixel_data,
	[COFFIN]	  = coffin_tex.pixel_data,
	[STATUE]	  = statue_tex.pixel_data,
	[TABLE]       = table_tex.pixel_data,
};

static const guint8 *item_textures[] = {
	[ITEM_ID_NONE]	   				 = emptyInventory_tex.pixel_data,

	// ITEM_CHARM_STAT
	[ITEM_ID_MINOR_HEALTH_CHARM] 	 = minorHealthCharm_tex.pixel_data,
	[ITEM_ID_MAJOR_HEALTH_CHARM] 	 = majorHealthCharm_tex.pixel_data,
	[ITEM_ID_SUPERIOR_HEALTH_CHARM]  = superiorHealthCharm_tex.pixel_data,
	[ITEM_ID_MINOR_ATTACK_CHARM]	 = minorAttackCharm_tex.pixel_data,
	[ITEM_ID_MAJOR_ATTACK_CHARM]	 = majorAttackCharm_tex.pixel_data,
	[ITEM_ID_SUPERIOR_ATTACK_CHARM]  = superiorAttackCharm_tex.pixel_data,
	[ITEM_ID_MINOR_DEFENSE_CHARM]	 = minorDefenseCharm_tex.pixel_data,
	[ITEM_ID_MAJOR_DEFENSE_CHARM]	 = majorDefenseCharm_tex.pixel_data,
	[ITEM_ID_SUPERIOR_DEFENSE_CHARM] = superiorDefenseCharm_tex.pixel_data,
	[ITEM_ID_MINOR_SPEED_CHARM]		 = minorSpeedCharm_tex.pixel_data,
	[ITEM_ID_MAJOR_SPEED_CHARM]		 = majorSpeedCharm_tex.pixel_data,
	[ITEM_ID_SUPERIOR_SPEED_CHARM]	 = superiorSpeedCharm_tex.pixel_data,
	[ITEM_ID_MINOR_LUCK_CHARM]		 = minorLuckCharm_tex.pixel_data,
	[ITEM_ID_MAJOR_LUCK_CHARM]		 = majorLuckCharm_tex.pixel_data,
	[ITEM_ID_SUPERIOR_LUCK_CHARM]	 = superiorLuckCharm_tex.pixel_data,

	// ITEM_CHARM_LEGENDARY
	[ITEM_ID_LEGENDARY_SOULSTONE]	 = legendarySoulstone_tex.pixel_data,

	// ITEM_EQUIPMENT - STAT_HEALTH
	[ITEM_ID_LEATHER_TUNIC]			 = leatherTunic_tex.pixel_data,
    [ITEM_ID_CHAINMAIL_PLATE]        = chainmailPlate_tex.pixel_data,
    [ITEM_ID_DRACONIC_ARMOR]         = draconicArmor_tex.pixel_data,

    // ITEM_EQUIPMENT - STAT_ATTACK
    [ITEM_ID_RUSTY_DAGGER]           = rustyDagger_tex.pixel_data,
    [ITEM_ID_STEEL_LONGSWORD]        = steelLongsword_tex.pixel_data,
    [ITEM_ID_RUNIC_BLADE]            = runicBlade_tex.pixel_data,

    // ITEM_EQUIPMENT - STAT_DEFENSE
    [ITEM_ID_WOODEN_SHIELD]          = woodenShield_tex.pixel_data,
    [ITEM_ID_KITE_SHIELD]            = kiteShield_tex.pixel_data,
    [ITEM_ID_AEGIS_SHIELD]           = aegisShield_tex.pixel_data,

    // ITEM_EQUIPMENT - STAT_SPEED
    [ITEM_ID_CLOTH_WRAPS]            = clothWraps_tex.pixel_data,
    [ITEM_ID_LEATHER_BOOTS]          = leatherBoots_tex.pixel_data,
    [ITEM_ID_HERMES_SANDALS]         = hermesSandals_tex.pixel_data,

    // ITEM_EQUIPMENT - STAT_LUCK
    [ITEM_ID_COPPER_BAND]            = copperBand_tex.pixel_data,
    [ITEM_ID_SILVER_SIGNET]          = silverSignet_tex.pixel_data,
    [ITEM_ID_MIDAS_TOUCH]            = midasTouch_tex.pixel_data,

    // ITEM_HEALING - Restore HP
    [ITEM_ID_SMALL_POTION]           = smallPotion_tex.pixel_data,
    [ITEM_ID_MEDIUM_POTION]          = mediumPotion_tex.pixel_data,
    [ITEM_ID_LARGE_POTION]           = largePotion_tex.pixel_data,
};

static const guint8 *player_textures[] = {
	[NORTH1] = playerNorth1_tex.pixel_data,
	[NORTH2] = playerNorth2_tex.pixel_data,
	[EAST1]  = playerEast1_tex.pixel_data,
	[EAST2]  = playerEast2_tex.pixel_data,
	[SOUTH1] = playerSouth1_tex.pixel_data,
	[SOUTH2] = playerSouth2_tex.pixel_data,
	[WEST1]  = playerWest1_tex.pixel_data,
	[WEST2]  = playerWest2_tex.pixel_data,
};

static const guint8 *enemy_textures[] = {
	[GOBLIN]		= goblin_tex.pixel_data,
	[GOO]			= goo_tex.pixel_data,
	[MAGE]			= mage_tex.pixel_data,
	[MAGIC_SHIELD]	= magicShield_tex.pixel_data,
	[SKELETON]		= skeleton_tex.pixel_data,
};

static const guint8 *startScreen = startScreen_tex.pixel_data;
static const guint8 *endScreen = endScreen_tex.pixel_data;

#endif
