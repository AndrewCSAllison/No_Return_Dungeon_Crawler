#ifndef INVENTORY_H
#define INVENTORY_H

#include "items.h"

#define INVENTORY_BOX_SIZE 64
#define INVENTORY_ROWS 4
#define INVENTORY_COLS 4
#define INVENTORY_SIZE 16

extern Item inventory[INVENTORY_SIZE];

void initInventory();
void addItem(Item *item);
void removeItem(unsigned char index);
void moveItem(unsigned char fromIndex, unsigned char toIndex);

#endif
