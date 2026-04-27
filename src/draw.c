#include "draw.h"
#include "colors.h"
#include "fonts.h"
#include "inventory.h"
#include "textures.h"
#include "log.h"
#include "enemy.h"

Framebuffer fb;

void initFramebuffer(uint32_t *addr, uint32_t pitch, uint32_t width, uint32_t height, uint32_t bpp) {
    fb.addr  = addr;
    fb.pitch = pitch;
    fb.width = width;
    fb.height = height;
    fb.bpp   = bpp;
}

static uint32_t getFontCharWidth(unsigned char *font_data) {
    if (*(uint32_t*)font_data == 0x864ab572)
        return ((psf2_header_t *)font_data)->width;
    else if (font_data[0] == 0x36 && font_data[1] == 0x04)
        return 8;
    return 8;
}

static uint32_t strPixelWidth(unsigned char *font_data, const char *str) {
    uint32_t len = 0;
    while (str[len]) len++;
    return len * getFontCharWidth(font_data);
}

// drawPixel
//
// Draws one pixel on the screen at the given (x,y) location. The color is a
// 3-byte RGB color value. Each byte controls the intensity of the red, gre>
// and blue for the given pixel. For example, RGB for light orange is 0xE09>
//
//          ----- Red intensity
//          |
//          |  ---- Green intensity
//          |  |
//          |  |  --- Blue intensity
//          |  |  |
//          /\ /\ /\
// color: 0xE0 96 67
//
// For example, to color a pixel light orange at location (5,5):
//
//   drawPixel(5,5,0xE09667);
//
void drawPixel(int x, int y, int color) {
    fb.addr[x + (y * (fb.width))] = (uint32_t)color;
}

// Bresenham's line algorithm - uses the slope & thresholds to determine which direction (x or y) to draw the next bit
void drawLine(int x0, int y0, int x1, int y1, int color) {
    int dx = (x1 > x0 ? x1 - x0 : x0 - x1);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -(y1 > y0 ? y1 - y0 : y0 - y1);
	int sy = y0 < y1 ? 1 : -1;
	int error = dx + dy;

	 while (1) {
        drawPixel(x0, y0, color);
        int e2 = 2 * error;                    // doubled error for threshold comparisons with integers

        if (e2 >= dy) {                        // error is past threshold, step in x direction
            if (x0 == x1) break;               // reached end of line
            error += dy;
            x0 += sx;
        }
        if (e2 <= dx) {                        // error is past threshold, step in y direction
            if (y0 == y1) break;               // reached end of line
            error += dx;
            y0 += sy;
        }
    }
}

void drawInt(unsigned char *font_data, int value, int x, int y, int color) {
    char buf[12];
    int i = 0;
    int n = value;

    if (n == 0) {
        buf[i++] = '0';
    } else {
        if (n < 0) { buf[i++] = '-'; n = -n; }
        char tmp[10];
        int j = 0;
        while (n > 0) { tmp[j++] = '0' + (n % 10); n /= 10; }
        while (j > 0) buf[i++] = tmp[--j];
    }
    buf[i] = '\0';
    drawString(font_data, buf, x, y, color);
}

void drawFraction(unsigned char *font_data, int cur, int max, int x, int y, int color) {
    char buf[24];
    int j = 0;
    char tmp[10];
    int k, n;

    // current
    n = cur; k = 0;
    if (n == 0) buf[j++] = '0';
    else { while(n>0){tmp[k++]='0'+(n%10);n/=10;} while(k>0) buf[j++]=tmp[--k]; }

    buf[j++] = ' '; buf[j++] = '/'; buf[j++] = ' ';

    // max
    n = max; k = 0;
    if (n == 0) buf[j++] = '0';
    else { while(n>0){tmp[k++]='0'+(n%10);n/=10;} while(k>0) buf[j++]=tmp[--k]; }

    buf[j] = '\0';

    int width = strPixelWidth(font_data, buf);
    int centeredX = x + width; // caller passes base x, we center from there
    drawString(font_data, buf, x - width / 2, y, color);
}

// Draw a given char at a set x,y position with a certain color & font
void drawChar(unsigned char *font_data, char c, int x, int y, int color) {
    if (!font_data) return;

    // --- PSF2 Check (Magic: 0x864ab572) ---
    // Note: PSF2 stores magic in Little Endian as 72 b5 4a 86
    if (*(uint32_t*)font_data == 0x864ab572) {
        psf2_header_t *h = (psf2_header_t *)font_data;

        // Find the start of glyphs and jump to character 'c'
        unsigned char *glyphs = font_data + h->header_size;
        unsigned char *glyph = glyphs + ((unsigned char)c * h->char_size);

        uint32_t bpr = (h->width + 7) / 8; // Bytes per row

        for (uint32_t row = 0; row < h->height; row++) {
            for (uint32_t col = 0; col < h->width; col++) {
                // PSF2 can have widths that span multiple bytes
                unsigned char bits = glyph[row * bpr + (col / 8)];
                if (bits & (0x80 >> (col % 8))) {
                    drawPixel(x + col, y + row, color);
                }
            }
        }
    }
    // --- PSF1 Check (Magic: 0x36 0x04) ---
    else if (font_data[0] == 0x36 && font_data[1] == 0x04) {
        // PSF1 Header is always 4 bytes.
        // byte 3: flags, byte 4: height
        uint32_t height = font_data[3];

        // PSF1 is always 8 pixels wide (1 byte per row)
        unsigned char *glyphs = font_data + 4;
        unsigned char *glyph = glyphs + ((unsigned char)c * height);

        for (uint32_t row = 0; row < height; row++) {
            unsigned char bits = glyph[row];
            for (uint32_t col = 0; col < 8; col++) {
                if (bits & (0x80 >> col)) {
                    drawPixel(x + col, y + row, color);
                }
            }
        }
    }
}


// Draws a null-terminated string using the specified font.
void drawString(unsigned char *font_data, const char *str, int x, int y, int color) {
    if (!font_data || !str) return;

    uint32_t char_width = 0;
    uint32_t char_height = 0;

    // Detect version to get dimensions safely
    if (*(uint32_t*)font_data == 0x864ab572) { // PSF2
        psf2_header_t *h = (psf2_header_t *)font_data;
        char_width = h->width;
        char_height = h->height;
    }
    else if (font_data[0] == 0x36 && font_data[1] == 0x04) { // PSF1
        char_width = 8;        // PSF1 is always 8px wide
        char_height = font_data[3]; // Height is stored in the 4th byte
    } else {
        return; // Unknown font format
    }

    int cur_x = x;
    while (*str) {
        if (*str == '\n') {
            cur_x = x;
            y += char_height;
        } else {
            drawChar(font_data, *str, cur_x, y, color);
            cur_x += char_width;
        }
        str++;
    }
}

void drawRect(int x, int y, int width, int height, int color) {
    for (int row = y; row < y + height; row++)
        for (int col = x; col < x + width; col++)
            drawPixel(col, row, color);
}

void drawGimpSprite(int x, int y, const guint8 *pixel_data, int width, int height) {
    for (int row = 0; row < height; row++)
        for (int col = 0; col < width; col++) {
            int i = (row * width + col) * 4;
            if (pixel_data[i+3] == 0) continue;
            drawPixel(x + col, y + row,
                ((uint32_t)pixel_data[i]   << 16) |
                ((uint32_t)pixel_data[i+1] << 8)  |
                 (uint32_t)pixel_data[i+2]);
        }
}

// Draw a singular tile to the screen at a given (row,col) index
void drawTile(int row, int col, TileType type) {
    int x = col * TILE_SIZE;
    int y = row * TILE_SIZE;
    if (tile_textures[type])
        drawGimpSprite(x, y, tile_textures[type], TILE_SIZE, TILE_SIZE);
    else if (type == ENEMY) {
        drawGimpSprite(x, y, tile_textures[FLOOR], TILE_SIZE, TILE_SIZE);
	} else {
		drawRect(x, y, TILE_SIZE, TILE_SIZE, COLOR_RED);
	}
}

// Draws the location of the player to the screen (updates on input)
void drawPlayer(int row, int col, Direction dir) {
    int x = col * TILE_SIZE;
    int y = row * TILE_SIZE;
    drawGimpSprite(x, y, player_textures[dir], TILE_SIZE, TILE_SIZE);
}

void drawInventorySlot(unsigned char index) {
    int col = index % INVENTORY_COLS;
    int row = index / INVENTORY_COLS;
    int x = SIDEBAR_START + (col * INVENTORY_BOX_SIZE);
    int y = INVENTORY_START + (row * INVENTORY_BOX_SIZE);

    if (player.inventory[index].itemID < MAX_ITEM_ID && item_textures[player.inventory[index].itemID]) {
        drawGimpSprite(x, y, item_textures[player.inventory[index].itemID], INVENTORY_BOX_SIZE, INVENTORY_BOX_SIZE);
    } else {
        drawRect(x, y, INVENTORY_BOX_SIZE, INVENTORY_BOX_SIZE, COLOR_RED);
    }
}

void drawEquipmentSlot(signed char index) {
    int i = 4 - ((-index) - 1);
    int x = SIDEBAR_START + 15;
    int y = EQUIPMENT_START + 40 + (i * EQUIPMENT_BOX_SIZE) + (i * 14);
    if (player.equipment[i].itemID < MAX_ITEM_ID && item_textures[player.equipment[i].itemID]) {
        drawGimpSprite(x, y, item_textures[player.equipment[i].itemID], EQUIPMENT_BOX_SIZE, EQUIPMENT_BOX_SIZE);
    } else {
        drawRect(x, y, EQUIPMENT_BOX_SIZE, EQUIPMENT_BOX_SIZE, COLOR_SIDEBAR);
    }
}

void drawSelectedSlot() {
    int x, y;
    if (selectedIndex < 0) {
        int i = 4 - ((-selectedIndex) - 1);
        x = SIDEBAR_START + 15;
        y = EQUIPMENT_START + 40 + (i * EQUIPMENT_BOX_SIZE) + (i * 14);
    } else {
        int col = selectedIndex % INVENTORY_COLS;
        int row = selectedIndex / INVENTORY_COLS;
        x = SIDEBAR_START + (col * INVENTORY_BOX_SIZE);
        y = INVENTORY_START + (row * INVENTORY_BOX_SIZE);
    }
    // Top
    drawLine(x, y,     x + INVENTORY_BOX_SIZE - 1, y,     COLOR_WHITE);
    drawLine(x, y + 1, x + INVENTORY_BOX_SIZE - 1, y + 1, COLOR_WHITE);
    // Bottom
    drawLine(x, y + INVENTORY_BOX_SIZE - 1, x + INVENTORY_BOX_SIZE - 1, y + INVENTORY_BOX_SIZE - 1, COLOR_WHITE);
    drawLine(x, y + INVENTORY_BOX_SIZE - 2, x + INVENTORY_BOX_SIZE - 1, y + INVENTORY_BOX_SIZE - 2, COLOR_WHITE);
    // Left
    drawLine(x,     y, x,     y + INVENTORY_BOX_SIZE - 1, COLOR_WHITE);
    drawLine(x + 1, y, x + 1, y + INVENTORY_BOX_SIZE - 1, COLOR_WHITE);
    // Right
    drawLine(x + INVENTORY_BOX_SIZE - 1, y, x + INVENTORY_BOX_SIZE - 1, y + INVENTORY_BOX_SIZE - 1, COLOR_WHITE);
    drawLine(x + INVENTORY_BOX_SIZE - 2, y, x + INVENTORY_BOX_SIZE - 2, y + INVENTORY_BOX_SIZE - 1, COLOR_WHITE);
}

void drawInventory() {
	drawString(ter_u20n_psf, "INVENTORY", SIDEBAR_START+82, INVENTORY_START-20, COLOR_TEXT);
    int start_x = SIDEBAR_START;
    int start_y = INVENTORY_START;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int col = i % INVENTORY_COLS;
        int row = i / INVENTORY_COLS;
        int x = start_x + (col * INVENTORY_BOX_SIZE);
        int y = start_y + (row * INVENTORY_BOX_SIZE);

        // Ensure the ID is within the bounds of your texture array
        // and that a texture actually exists for this ID
		if (player.inventory[i].itemID < MAX_ITEM_ID && item_textures[player.inventory[i].itemID]) {
            drawGimpSprite(x, y, item_textures[player.inventory[i].itemID],INVENTORY_BOX_SIZE, INVENTORY_BOX_SIZE);
        } else {
            // Debug fallback: If a texture is missing, draw a red box
            drawRect(x, y, INVENTORY_BOX_SIZE, INVENTORY_BOX_SIZE, COLOR_RED);
        }
    }
}

void drawEquipment() {
	const char *statLabel;
	int labelWidth;
	int centeredX;
    drawString(ter_u20n_psf, "EQUIPMENT", SIDEBAR_START+4, EQUIPMENT_START, COLOR_TEXT);
    for (int i = 0; i < 5; i++) {
        int x = SIDEBAR_START + 15;
        int y = EQUIPMENT_START + 40 + (i * EQUIPMENT_BOX_SIZE)+(i * 14);
        switch(i) {
            case 0: statLabel = "CHESTPLATE"; break;
            case 1: statLabel = "SWORD";      break;
            case 2: statLabel = "SHIELD";     break;
            case 3: statLabel = "BOOTS";      break;
            case 4: statLabel = "RING";       break;
            default: statLabel = "NONE";      break;
        }
        labelWidth = strPixelWidth(ter_u14n_psf, statLabel);
		centeredX = x + (EQUIPMENT_BOX_SIZE - labelWidth) / 2;
		drawString(ter_u14n_psf, statLabel, centeredX, y - 14, COLOR_TEXT);
		if (player.equipment[i].itemID < MAX_ITEM_ID && item_textures[player.equipment[i].itemID]) {
            drawGimpSprite(x, y, item_textures[player.equipment[i].itemID], EQUIPMENT_BOX_SIZE, EQUIPMENT_BOX_SIZE);
        } else {
            drawRect(x, y, EQUIPMENT_BOX_SIZE, EQUIPMENT_BOX_SIZE, COLOR_RED);
        }
    }
}

void drawStats() {
    const char *labels[5] = { "HEALTH POINTS", "ATTACK", "DEFENSE", "SPEED", "LUCK" };
    int values[5] = { player.hp, player.atk, player.def, player.spd, player.luck };

    drawRect(STATS_X, EQUIPMENT_START, (SIDEBAR_START + 256) - STATS_X, EQUIPMENT_BOX_SIZE * 5 + 14 * 4 + 40, COLOR_SIDEBAR);

    int titleWidth = strPixelWidth(ter_u20n_psf, "STATS");
    int titleX = STATS_X + (((SIDEBAR_START + 256) - STATS_X) - titleWidth) / 2;
    drawString(ter_u20n_psf, "STATS", titleX, EQUIPMENT_START, COLOR_TEXT);

    int colWidth = (SIDEBAR_START + 256) - STATS_X;

    for (int i = 0; i < 5; i++) {
        int y = STATS_Y + (i * STATS_ROW_HEIGHT);

        int labelWidth = strPixelWidth(ter_u14n_psf, labels[i]);
        int centeredX = STATS_X + (colWidth - labelWidth) / 2;
        drawString(ter_u14n_psf, labels[i], centeredX, y, COLOR_TEXT);

        int midX = STATS_X + colWidth / 2;
        if (i == 0) {
            drawFraction(ter_u14n_psf, player.hp, player.maxHp, midX, y + 16, COLOR_TEXT);
        } else {
            // reuse drawFraction with equal values to get centering, or just drawInt
            char buf[12];
            int n = values[i], k = 0; char tmp[10]; int jj = 0;
            if (n == 0) buf[jj++] = '0';
            else { while(n>0){tmp[k++]='0'+(n%10);n/=10;} while(k>0) buf[jj++]=tmp[--k]; }
            buf[jj] = '\0';
            int intWidth = strPixelWidth(ter_u14n_psf, buf);
            drawInt(ter_u14n_psf, values[i], midX - intWidth / 2, y + 16, COLOR_TEXT);
        }
    }
}

void drawConsole() {
    int consoleHeight = LOG_LINE_HEIGHT + (QUEUE_SIZE * LOG_LINE_HEIGHT) + 8;
    int colWidth = (SIDEBAR_START + 256) - LOG_X - 4;
    drawRect(LOG_X, LOG_Y, colWidth + 4, consoleHeight, COLOR_SIDEBAR);

    int titleWidth = strPixelWidth(ter_u20n_psf, "CONSOLE");
    int titleX = LOG_X + (colWidth - titleWidth) / 2;
    drawString(ter_u20n_psf, "CONSOLE", titleX, LOG_Y, COLOR_TEXT);

    int charWidth = getFontCharWidth(ter_u14n_psf);
    int maxChars = colWidth / charWidth;

    for (int i = 0; i < gameLog.count; i++) {
        int idx = (gameLog.head + i) % QUEUE_SIZE;
        if (gameLog.data[idx]) {
            char buf[64];
            int j = 0;
            while (j < maxChars - 1 && gameLog.data[idx][j]) {
                buf[j] = gameLog.data[idx][j];
                j++;
            }
            buf[j] = '\0';
            drawString(ter_u14n_psf, buf, LOG_X, LOG_Y + 20 + (i * LOG_LINE_HEIGHT), COLOR_TEXT);
        }
    }
}

void drawEnemies() {
    int i;
    for (i = 0; i < enemyCount; i++) {
        if (enemies[i].alive) {
            drawGimpSprite(enemies[i].x * TILE_SIZE, enemies[i].y * TILE_SIZE, enemy_textures[enemies[i].type], TILE_SIZE, TILE_SIZE);
        } else {
			drawTile(enemies[i].y, enemies[i].x, FLOOR);
		}
    }
}

void whiteScreen() {
    for (int y = 0; y < (int)fb.height; y++)
        for (int x = 0; x < (int)fb.width; x++)
            drawPixel(x, y, COLOR_WHITE);
}
