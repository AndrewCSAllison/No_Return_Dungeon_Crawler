#ifndef PLAYER_H
#define PLAYER_H

#include "items.h"

#define INVENTORY_SIZE 12
#define EQUIPMENT_SIZE 5
#define MAX_STAT 999

typedef enum {
	NORTH1,
	NORTH2,
	EAST1,
	EAST2,
	SOUTH1,
	SOUTH2,
	WEST1,
	WEST2,
} Direction;

typedef struct {
    char x;
    char y;
    Direction currentDirection;

    // Base stats (permanent, modified by charms)
    int baseHp;
    int baseAtk;
    int baseDef;
    int baseSpd;
    int baseLuck;

    // Derived stats (base + equipment bonuses)
    int maxHp;
    int hp;
    int atk;
    int def;
    int spd;
    int luck;

    Item equipment[EQUIPMENT_SIZE];
    Item inventory[INVENTORY_SIZE];
} Player;

extern Player player;

void initPlayer(unsigned char startX, unsigned char startY);
void setPlayerPos(unsigned char x, unsigned char y);
void movePlayer(signed char dx, signed char dy);
void modifyStat(StatType stat, int amount);
void calculateStats();
void setNorth();
void setEast();
void setSouth();
void setWest();
int getPlayerBaseTotal();

#endif
