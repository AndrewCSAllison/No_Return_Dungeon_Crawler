#ifndef GAME_H
#define GAME_H

#define MAX_DAMAGE_SPAWNS 12
#define MAX_CHEST_SPAWNS 5

typedef enum {
    STATE_START,
    STATE_EXPLORE,
    STATE_INVENTORY,
    STATE_DEATH,
} GameState;

extern GameState currentState;

void switchState(GameState newState);
void interactLadder();
void spawnDamageTiles(int count);
void spawnChests(int count);
void attackEnemy();
void lootChest();
void dealDamageTiles();

#endif
