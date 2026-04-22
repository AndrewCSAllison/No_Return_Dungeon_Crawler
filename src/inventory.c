#include "inventory.h"
#include "items.h"

Item inventory[INVENTORY_SIZE];

void initInventory() {
	for (int i = 0; i < 16; i++) {
		inventory[i].name = "";
		inventory[i].itemID = 0;
		inventory[i].type = 0;
		inventory[i].stat = 0;
		inventory[i].tier = 0;
		inventory[i].value = 0;
	}
}

void addItem(Item *item) {
	for (int i = 0; i < INVENTORY_SIZE; i++) {
		if (inventory[i].type == 0) {
			inventory[i] = *item;
			return;
		}
	}
}

void removeItem(unsigned char index) {
	if (index < INVENTORY_SIZE) {
		inventory[index].name = "Nothing";
        inventory[index].itemID = 0;
		inventory[index].type = 0;
		inventory[index].stat = 0;
		inventory[index].tier = 0;
		inventory[index].value = 0;
	}
}

void moveItem(unsigned char fromIndex, unsigned char toIndex) {
	if (fromIndex < INVENTORY_SIZE && toIndex < INVENTORY_SIZE) {
		inventory[toIndex] = inventory[fromIndex];
		removeItem(fromIndex);
	}
}
