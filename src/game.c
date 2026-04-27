#include "game.h"
#include "grid.h"
#include "player.h"
#include "inventory.h"
#include "render.h"
#include "log.h"
#include "maps.h"
#include "enemy.h"
#include "random.h"
#include "util.h"
#include "draw.h"

GameState currentState = STATE_START;

void switchState(GameState newState) {
    currentState = newState;
}

// ---------------------------------------------------------------------------
// spawnDamageTiles
// Places up to `count` damage tiles on free FLOOR tiles.
// Randomly selects FIRE, ACID, or SPIKES for each placement.
// ---------------------------------------------------------------------------
void spawnDamageTiles(int count) {
    int i;
    TileType damageTypes[3] = { FIRE, ACID, SPIKES };

    for (i = 0; i < count; i++) {
        Vec2 pos = findFreeTile();
        if (pos.x == -1) break;
        grid[pos.y][pos.x] = damageTypes[rand(3)];
    }
}

// ---------------------------------------------------------------------------
// spawnChests
// Places up to `count` CHEST tiles on free FLOOR tiles.
// ---------------------------------------------------------------------------
void spawnChests(int count) {
    int i;
    for (i = 0; i < count; i++) {
        Vec2 pos = findFreeTile();
        if (pos.x == -1) break;
        grid[pos.y][pos.x] = CHEST;
    }
}

// ---------------------------------------------------------------------------
// interactLadder
// ---------------------------------------------------------------------------
void interactLadder() {
    if (grid[player.y][player.x] != LADDER) return;

    const char *mapNames[NUM_MAPS] = {
        "Vault", "Crypt", "Apothecary", "Library",
        "Shrine Room", "Dining Hall", "Hallway", "Maze"
    };

    int room = loadNextRoom();
    Vec2 spawn = generateDoor();
    setPlayerPos(spawn.x, spawn.y);
    generateLadder(spawn.x, spawn.y);

    // Spawn stage entities after room is loaded
    int enemyCount  = 4 + (player.luck * (MAX_ENEMY_SPAWNS - 4)) / MAX_STAT;
	int damageCount = 4 + (player.luck * (MAX_DAMAGE_SPAWNS - 4)) / MAX_STAT;
	int chestCount  = 1 + (player.luck * (MAX_CHEST_SPAWNS - 1)) / MAX_STAT;

	spawnEnemies(enemyCount, getPlayerBaseTotal(), player.luck);
	spawnDamageTiles(damageCount);
    spawnChests(chestCount);

    logMessage(buildLog("Entered a new ", mapNames[room], 0));
    renderGrid();
    renderPlayer();
    renderSidebar();
}

void attackEnemy() {
    int tx = player.x;
    int ty = player.y;

    switch (player.currentDirection) {
        case NORTH1: case NORTH2: ty -= 1; break;
        case SOUTH1: case SOUTH2: ty += 1; break;
        case EAST1:  case EAST2:  tx += 1; break;
        case WEST1:  case WEST2:  tx -= 1; break;
    }

    if (grid[ty][tx] != ENEMY) return;

    int idx = getEnemyAtTile(tx, ty);
    if (idx == -1) return;

    Enemy *e = &enemies[idx];

    int playerDmg = ABS(player.atk - e->def) / 2;
    int enemyDmg  = ABS(e->atk - player.def) / 5;
    if (playerDmg < 1) playerDmg = 1;
    if (enemyDmg  < 1) enemyDmg  = 1;

    // Crit chance: scales from 0% at luck=0 to 25% at luck=999
    int critChance = (player.luck * 25) / MAX_STAT;
    int playerCrit = ((int)rand(100) < critChance);
    if (playerCrit) playerDmg *= 2;

    int playerDodge = (player.spd - e->spd) * 100 / MAX_STAT;
    int enemyDodge  = (e->spd - player.spd) * 100 / MAX_STAT;
    if (playerDodge < 0)  playerDodge = 0;
    if (playerDodge > 40) playerDodge = 40;
    if (enemyDodge  < 0)  enemyDodge  = 0;
    if (enemyDodge  > 40) enemyDodge  = 40;

    // Player attacks
    if ((int)rand(100) >= enemyDodge) {
        e->hp -= playerDmg;
        if (playerCrit)
            logMessage(buildLog("Critical hit for ", 0, playerDmg));
        else if (playerDmg <= 1)
            logMessage(buildLog("Your attack was absorbed!", 0, 0));
        else
            logMessage(buildLog("You hit for ", 0, playerDmg));
    } else {
        logMessage(buildLog("Your attack missed!", 0, 0));
    }

    // Enemy attacks
    if ((int)rand(100) >= playerDodge) {
        player.hp -= enemyDmg;
        if (enemyDmg <= 1)
            logMessage(buildLog("The attack was absorbed!", 0, 0));
        else
            logMessage(buildLog("Enemy hit for ", 0, enemyDmg));
    } else {
        logMessage(buildLog("You dodged!", 0, 0));
    }

    // Resolve deaths
    if (e->hp <= 0) {
        e->hp    = 0;
        e->alive = 0;
        grid[ty][tx] = FLOOR;
        logMessage(buildLog("Enemy defeated!", 0, 0));
        Item reward = generateItem(player.luck, e->isElite);
        if (reward.itemID != ITEM_ID_NONE)
            logMessage(buildLog("Dropped: ", reward.name, 0));
        addItem(&reward);
    }

    if (player.hp <= 0) {
        player.hp = 0;
        logMessage(buildLog("You have died.", 0, 0));
        switchState(STATE_DEATH);
    }
}

void lootChest() {
    int tx = player.x;
    int ty = player.y;

    switch (player.currentDirection) {
        case NORTH1: case NORTH2: ty -= 1; break;
        case SOUTH1: case SOUTH2: ty += 1; break;
        case EAST1:  case EAST2:  tx += 1; break;
        case WEST1:  case WEST2:  tx -= 1; break;
    }

    if (tx < 0 || tx >= GRID_COLS || ty < 0 || ty >= GRID_ROWS) return;
    if (grid[ty][tx] != CHEST) return;

    grid[ty][tx] = FLOOR;
	drawTile(ty, tx, FLOOR);

    Item reward = generateChestItem(player.luck);
    logMessage(buildLog("You opened a chest!", 0, 0));
    if (reward.itemID != ITEM_ID_NONE)
        logMessage(buildLogFull("Found: ", reward.name, " +", reward.value));
    addItem(&reward);
}

void dealDamageTiles() {
    TileType tile = grid[player.y][player.x];

    int damage = 0;

    switch (tile) {
        case FIRE:   damage = 15;                              break;  // flat high damage
        case ACID:   damage = (player.maxHp * 8) / 100;       break;  // 8% max HP
        case SPIKES: damage = 5;                               break;  // flat low damage
        default: return;
    }

    if (damage < 1) damage = 1;

    player.hp -= damage;

    switch (tile) {
        case FIRE:   logMessage(buildLog("You are burning! ", 0, damage));   break;
        case ACID:   logMessage(buildLog("Acid burns you! ",  0, damage));   break;
        case SPIKES: logMessage(buildLog("Spikes pierce you! ", 0, damage)); break;
        default: break;
    }

    if (player.hp <= 0) {
        player.hp = 0;
        logMessage(buildLog("You have died.", 0, 0));
        switchState(STATE_DEATH);
    }
}
