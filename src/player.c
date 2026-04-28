#include "player.h"
#include "random.h"
#include "grid.h"
#include "inventory.h"
#include "util.h"

Player player;

int isBlocking(TileType t) {
    switch(t) {
        case WALL:
        case DOOR_CLOSED:
        case CHEST:
        case STATUE:
        case CAULDRON:
        case BOOKSHELF:
        case COFFIN:
        case ALTAR:
        case TABLE:
		case ENEMY:
            return 1;
        default:
            return 0;
    }
}

void modifyStat(StatType stat, int amount) {
    switch(stat) {
        case STAT_HEALTH:  player.baseHp   += amount; break;
        case STAT_ATTACK:  player.baseAtk  += amount; break;
        case STAT_DEFENSE: player.baseDef  += amount; break;
        case STAT_SPEED:   player.baseSpd  += amount; break;
        case STAT_LUCK:    player.baseLuck += amount; break;
        default: break;
    }
    calculateStats();
}

void calculateStats() {
    // Clamp base stats
    player.baseHp   = CLAMP(player.baseHp,   1, MAX_STAT);
    player.baseAtk  = CLAMP(player.baseAtk,  1, MAX_STAT);
    player.baseDef  = CLAMP(player.baseDef,  1, MAX_STAT);
    player.baseSpd  = CLAMP(player.baseSpd,  1, MAX_STAT);
    player.baseLuck = CLAMP(player.baseLuck, 1, MAX_STAT);

    // Reset derived to base
    player.maxHp = player.baseHp;
    player.atk   = player.baseAtk;
    player.def   = player.baseDef;
    player.spd   = player.baseSpd;
    player.luck  = player.baseLuck;

    // Add equipment bonuses
    for (int i = 0; i < EQUIPMENT_SIZE; i++) {
        if (player.equipment[i].itemID == ITEM_ID_NONE) continue;
        switch(player.equipment[i].stat) {
            case STAT_HEALTH:  player.maxHp += player.equipment[i].value; break;
            case STAT_ATTACK:  player.atk   += player.equipment[i].value; break;
            case STAT_DEFENSE: player.def   += player.equipment[i].value; break;
            case STAT_SPEED:   player.spd   += player.equipment[i].value; break;
            case STAT_LUCK:    player.luck  += player.equipment[i].value; break;
            default: break;
        }
    }

    // Clamp derived stats
    player.maxHp = CLAMP(player.maxHp, 1, MAX_STAT);
    player.atk   = CLAMP(player.atk,   1, MAX_STAT);
    player.def   = CLAMP(player.def,   1, MAX_STAT);
    player.spd   = CLAMP(player.spd,   1, MAX_STAT);
    player.luck  = CLAMP(player.luck,  1, MAX_STAT);
    player.hp    = CLAMP(player.hp,    0, player.maxHp);
}

void initPlayer(unsigned char startX, unsigned char startY) {
    player.x = startX;
    player.y = startY;
    player.currentDirection = SOUTH1;

    unsigned char pool = 100;
    int stats[5];

    for (int i = 0; i < 4; i++) {
        if (pool > 0) {
            stats[i] = rand(pool / 2);
            pool -= stats[i];
        } else {
            stats[i] = 0;
        }
    }
    stats[4] = pool;

    player.baseHp   = 150 + stats[0];
    player.baseAtk  = 150 + stats[1];
    player.baseDef  = 150 + stats[2];
    player.baseSpd  = 150 + stats[3];
    player.baseLuck = 150 + stats[4];

    for (int i = 0; i < EQUIPMENT_SIZE; i++) {
        player.equipment[i].name   = "";
        player.equipment[i].itemID = ITEM_ID_NONE;
        player.equipment[i].type   = ITEM_NONE;
        player.equipment[i].stat   = STAT_NONE;
        player.equipment[i].tier   = 0;
        player.equipment[i].value  = 0;
    }

    calculateStats();
    player.hp = player.maxHp;

    initInventory();
}

void movePlayer(signed char dx, signed char dy) {
    signed char nextX = player.x + dx;
    signed char nextY = player.y + dy;
    if (nextX < 0 || nextX >= GRID_COLS || nextY < 0 || nextY >= GRID_ROWS) return;
    if (isBlocking(grid[nextY][nextX])) return;
    player.x = nextX;
    player.y = nextY;
}

void setPlayerPos(unsigned char x, unsigned char y) {
	player.x = x;
	player.y = y;
}

void setNorth() {
    player.currentDirection = (player.currentDirection == NORTH1) ? NORTH2 : NORTH1;
}
void setEast() {
    player.currentDirection = (player.currentDirection == EAST1) ? EAST2 : EAST1;
}
void setSouth() {
    player.currentDirection = (player.currentDirection == SOUTH1) ? SOUTH2 : SOUTH1;
}
void setWest() {
    player.currentDirection = (player.currentDirection == WEST1) ? WEST2 : WEST1;
}

int getPlayerBaseTotal() {
    return player.baseHp + player.baseAtk + player.baseDef + player.baseSpd + player.baseLuck;
}
