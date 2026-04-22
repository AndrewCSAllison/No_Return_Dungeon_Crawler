#include "grid.h"
#include "colors.h"
#include "maps.h"
#include "random.h"
#include "util.h"

TileType grid[GRID_ROWS][GRID_COLS];

Vec2 generateDoor() {
    int placed = 0;
    int attempts = 0;

	int final_x = 0;
	int final_y = 0;

	Vec2 spawnPos;

    while (!placed && attempts < 200) {
        attempts++;

        int side = rand(4);
		int x, y; 	   // Wall slow where the entrance door goes
        int cx, cy;    // The check slot (one tile inside the room)

        switch(side) {
            case 0: // TOP
                x = rand(14) + 1; // Pick a grid slot between index 1-14 inclusive
                y = 0;
                cx = x;
				cy = 1;
                break;
            case 1: // BOTTOM
                x = rand(14) + 1;
                y = GRID_ROWS - 1;
                cx = x;
				cy = GRID_ROWS - 2;
                break;
            case 2: // LEFT
                x = 0;
                y = rand(14) + 1;
                cx = 1;
				cy = y;
                break;
            case 3: // RIGHT
            default:
                x = GRID_COLS - 1;
                y = rand(14) + 1;
                cx = GRID_COLS - 2;
				cy = y;
                break;
        }

        // VALIDATION:
        // 1. Target (x,y) must be a WALL (don't overwrite floor/items)
        // 2. Neighbor (cx,cy) must be FLOOR (so player isn't blocked)
        if (grid[y][x] == WALL && grid[cy][cx] == FLOOR) {
            grid[y][x] = DOOR_CLOSED;

			final_x = x;
			final_y = y;
			placed = 1;
        }
    }

    // EMERGENCY FALLBACK: If 200 attempts fail, force a door in a standard spot
    if (!placed) {
        grid[GRID_ROWS/2][0] = DOOR_CLOSED; // Middle of left wall

	}

	spawnPos.x = final_x;
	spawnPos.y = final_y;

	return spawnPos;
}

void generateLadder(int px, int py) {
    int placed = 0;
    int attempts = 0;

    while (!placed && attempts < 300) {
        attempts++;

        // 1. Pick a completely random spot on the map
        int lx = rand(GRID_COLS);
        int ly = rand(GRID_ROWS);

        // 2. Calculate Manhattan Distance from player
        int dist = ABS(lx - px) + ABS(ly - py);

        // 3. VALIDATION:
        // Must be a FLOOR tile AND far enough away
        if (grid[ly][lx] == FLOOR && dist > 10) {
            grid[ly][lx] = LADDER;
            placed = 1;
        }
    }

    // EMERGENCY FALLBACK
    // If we can't find a spot > 10 away, just take any floor
    if (!placed) {
        for (int y = 0; y < GRID_ROWS; y++) {
            for (int x = 0; x < GRID_COLS; x++) {
                if (grid[y][x] == FLOOR) {
                    grid[y][x] = LADDER;
                    return;
                }
            }
        }
    }
}

void loadNextRoom() {
    int pick = rand(8);
    size_t totalBytes = sizeof(grid);

    memcpy(grid, mapCatalog[pick], totalBytes);
}
