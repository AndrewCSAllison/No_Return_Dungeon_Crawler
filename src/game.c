#include "game.h"
#include "grid.h"
#include "player.h"
#include "inventory.h"
#include "render.h"
#include "log.h"
#include "maps.h"
#include "enemy.h"

GameState currentState = STATE_START;

void switchState(GameState newState) {
    currentState = newState;
}

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

	logMessage(buildLog("Entered a new ", mapNames[room], 0));

	renderGrid();
    renderPlayer();
    renderSidebar();
}
