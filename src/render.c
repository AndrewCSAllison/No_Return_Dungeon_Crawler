#include "grid.h"
#include "draw.h"
#include "player.h"

static int last_px = -1;
static int last_py = -1;

void renderGrid() {
    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
            drawTile(row, col, grid[row][col]);
}

void renderInventory() {
	drawInventory();
}

void renderPlayer() {
    // 1. If the player moved, erase their old ghost
    if (last_px != -1 && (last_px != player.x || last_py != player.y)) {
        drawTile(last_py, last_px, grid[last_py][last_px]);
    }

    // 2. Draw the player at the new position
    drawPlayer(player.y, player.x);

    // 3. Save the position for the next frame
    last_px = player.x;
    last_py = player.y;
}
