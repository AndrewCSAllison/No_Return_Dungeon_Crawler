#ifndef INVENTORY_H
#define INVENTORY_H

#include "items.h"

#define INVENTORY_ROWS 3
#define INVENTORY_COLS 4
#define INVENTORY_SIZE 12
#define EQUIPMENT_COLS 1
#define EQUIPMENT_ROWS 5

extern Item inventory[INVENTORY_SIZE];
extern signed char selectedIndex;
extern signed char grabbedIndex;

void initInventory();
void addItem(Item *item);
void removeItem(unsigned char index);
void moveItem(signed char fromIndex, signed char toIndex);
void equipItem();
void moveCursor(signed char dx, signed char dy);
void grabOrSwap();

#endif
