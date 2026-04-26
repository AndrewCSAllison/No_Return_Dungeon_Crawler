#include "items.h"
#include "grid.h"
#include "draw.h"
#include "player.h"
#include "colors.h"
#include "inventory.h"
#include "fonts.h"

static int last_px = -1;
static int last_py = -1;
static signed char lastInventoryIndex = -127;

void renderGrid() {
    for (int row = 0; row < GRID_ROWS; row++)
        for (int col = 0; col < GRID_COLS; col++)
            drawTile(row, col, grid[row][col]);
}

void renderPlayer() {
	// 1. If the player moved, erase their old ghost
    if (last_px != -1) {
        drawTile(last_py, last_px, grid[last_py][last_px]);
    }

    // 2. Draw the player at the new position
    drawPlayer(player.y, player.x, player.currentDirection);

    // 3. Save the position for the next frame
    last_px = player.x;
    last_py = player.y;
}

void renderStats() {
	drawStats();
}

void renderEquipment() {
	drawEquipment();
}

void renderInventory() {
	drawInventory();
}

void renderConsole() {
	drawConsole();
}

void renderSelectedSlot() {
    if (lastInventoryIndex != -127 && lastInventoryIndex != selectedIndex) {
        if (lastInventoryIndex < 0)
            drawEquipmentSlot(lastInventoryIndex);
        else
            drawInventorySlot(lastInventoryIndex);
    }
    drawSelectedSlot();
    lastInventoryIndex = selectedIndex;
}

void renderSidebar() {
	drawRect(SIDEBAR_START, 0, 256, 768, COLOR_SIDEBAR);
	drawString(ter_u20n_psf, "NO RETURN DUNGEON CRAWLER", SIDEBAR_START+4, 0, COLOR_TEXT);
	drawConsole();
	renderStats();
	renderEquipment();
	renderInventory();
}
