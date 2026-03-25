#include "inventory.h"
#include "items.h"
#include "player.h"
#include "log.h"

signed char selectedIndex = 0;
signed char grabbedIndex = 127;

void initInventory() {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        player.inventory[i].name = "";
        player.inventory[i].itemID = ITEM_ID_NONE;
        player.inventory[i].type = ITEM_NONE;
        player.inventory[i].stat = STAT_NONE;
        player.inventory[i].tier = 0;
        player.inventory[i].value = 0;
    }
	player.inventory[0] = ITEM_CATALOG[14];
	player.inventory[1] = ITEM_CATALOG[20];
}

void addItem(Item *item) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player.inventory[i].type == ITEM_NONE) {
            player.inventory[i] = *item;
            return;
        }
    }
}

void removeItem(unsigned char index) {
    if (index < INVENTORY_SIZE) {
        player.inventory[index].name = "";
        player.inventory[index].itemID = ITEM_ID_NONE;
        player.inventory[index].type = ITEM_NONE;
        player.inventory[index].stat = STAT_NONE;
        player.inventory[index].tier = 0;
        player.inventory[index].value = 0;
    }
}

void moveItem(signed char fromIndex, signed char toIndex) {
    if (fromIndex < INVENTORY_SIZE && toIndex < INVENTORY_SIZE) {
        player.inventory[toIndex] = player.inventory[fromIndex];
        removeItem(fromIndex);
    }
}

void useItem() {
    if (selectedIndex < -5 || selectedIndex >= INVENTORY_SIZE) return;

    Item *item;
    int equipSlot = -1;

    if (selectedIndex < 0) {
        equipSlot = 4 - ((-selectedIndex) - 1);
        item = &player.equipment[equipSlot];
    } else {
        item = &player.inventory[selectedIndex];
    }

    if (item->itemID == ITEM_ID_NONE) return;

    // Unequip from equipment panel
    if (equipSlot != -1) {
        addItem(item);
        logMessage(buildLog("Unequipped ", item->name, 0));
        item->name   = "";
        item->itemID = ITEM_ID_NONE;
        item->type   = ITEM_NONE;
        item->stat   = STAT_NONE;
        item->tier   = 0;
        item->value  = 0;
        calculateStats();
        return;
    }

    switch(item->type) {
        case ITEM_EQUIPMENT:
        {
            int slot;
            switch(item->stat) {
                case STAT_HEALTH:  slot = 0; break;
                case STAT_ATTACK:  slot = 1; break;
                case STAT_DEFENSE: slot = 2; break;
                case STAT_SPEED:   slot = 3; break;
                case STAT_LUCK:    slot = 4; break;
                default: return;
            }
            if (player.equipment[slot].itemID != ITEM_ID_NONE) {
			    addItem(&player.equipment[slot]);
    			logMessage(buildLogFull("Swapped ", item->name, " +", item->value));
			} else {
    			logMessage(buildLogFull("Equipped ", item->name, " +", item->value));
			}
            player.equipment[slot] = *item;
            removeItem(selectedIndex);
            calculateStats();
            break;
        }
        case ITEM_HEALING:
            player.hp += item->value;
            if (player.hp > player.maxHp) player.hp = player.maxHp;
            logMessage(buildLog("Used ", item->name, 0));
            logMessage(buildLog("HP restored +", "", item->value));
            removeItem(selectedIndex);
            break;
        case ITEM_CHARM_STAT:
        {
            const char *statName;
            switch(item->stat) {
                case STAT_HEALTH:  statName = "HP";  break;
                case STAT_ATTACK:  statName = "ATK"; break;
                case STAT_DEFENSE: statName = "DEF"; break;
                case STAT_SPEED:   statName = "SPD"; break;
                case STAT_LUCK:    statName = "LCK"; break;
                default:           statName = "";    break;
            }
            modifyStat(item->stat, item->value);
            logMessage(buildLog("Used ", item->name, 0));
            logMessage(buildLog(statName, " +", item->value));
            removeItem(selectedIndex);
            break;
        }
        case ITEM_CHARM_LEGENDARY:
            modifyStat(STAT_HEALTH,  item->value);
            modifyStat(STAT_ATTACK,  item->value);
            modifyStat(STAT_DEFENSE, item->value);
            modifyStat(STAT_SPEED,   item->value);
            modifyStat(STAT_LUCK,    item->value);
            player.hp += item->value;
            if (player.hp > player.maxHp) player.hp = player.maxHp;
            logMessage(buildLog("Legendary consumed.", "", 0));
            logMessage(buildLog("All stats +", "", item->value));
            removeItem(selectedIndex);
            break;
        default:
            break;
    }
}

void grabOrSwap() {
    if (grabbedIndex == 127) {
        if (selectedIndex >= 0 && player.inventory[selectedIndex].itemID != ITEM_ID_NONE)
            grabbedIndex = selectedIndex;
    } else {
        if (selectedIndex >= 0) {
            Item tmp = player.inventory[grabbedIndex];
            player.inventory[grabbedIndex] = player.inventory[selectedIndex];
            player.inventory[selectedIndex] = tmp;
        }
        grabbedIndex = 127;
    }
}

void moveCursor(signed char fromIndex, signed char toIndex) {
	if (toIndex >= -5 && toIndex < INVENTORY_SIZE) {
		selectedIndex = toIndex;
	}
}
