#include "player.h"
#include "random.h"
#include "grid.h"
#include "inventory.h"

// Define the global player instance
Player player;

void initPlayer(int startX, int startY) {
    player.x = startX;
    player.y = startY;
    player.charm_points = 0;

    int pool = 100;
    int stats[5]; // Simplified for HP, ATK, DEF, SPD, LUCK

    // Randomly distribute the 100-point pool
    // This method is more balanced than the previous linear subtraction
    for (int i = 0; i < 4; i++) {
        if (pool > 0) {
            stats[i] = rand(pool / 2); // Prevent one stat from eating the whole pool
            pool -= stats[i];
        } else {
            stats[i] = 0;
        }
    }
    stats[4] = pool; // Remaining goes to Luck

    // Base stats of 100 + random bonuses
    player.hp   = 100 + stats[0];
    player.atk  = 100 + stats[1];
    player.def  = 100 + stats[2];
    player.spd  = 100 + stats[3];
    player.luck = 100 + stats[4];

    initInventory();
}

void movePlayer(int dx, int dy) {
    int next_x = player.x + dx;
    int next_y = player.y + dy;

    // 1. Boundary Check
    if (next_x < 0 || next_x >= GRID_COLS || next_y < 0 || next_y >= GRID_ROWS) {
        return;
    }

    // 2. Collision Check
    // We stop at Walls and Closed Doors
    if (grid[next_y][next_x] == WALL || grid[next_y][next_x] == DOOR_CLOSED) {
        return;
    }

    // 3. Update Position
    player.x = next_x;
    player.y = next_y;
}
