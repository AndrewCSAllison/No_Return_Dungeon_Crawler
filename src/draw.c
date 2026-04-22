#include "draw.h"
#include "colors.h"
#include "player.h"
#include "fonts.h"
#include "inventory.h"
#include "textures.h"

Framebuffer fb;

void initFramebuffer(uint32_t *addr, uint32_t pitch, uint32_t width, uint32_t height, uint32_t bpp) {
    fb.addr  = addr;
    fb.pitch = pitch;
    fb.width = width;
    fb.height = height;
    fb.bpp   = bpp;
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
    else
        drawRect(x, y, TILE_SIZE, TILE_SIZE, COLOR_RED); // fallback
}

// Draws the location of the player to the screen (updates on input)
void drawPlayer(int row, int col) {
    int x = (col * TILE_SIZE) + ((TILE_SIZE - PLAYER_SIZE) / 2);
    int y = (row * TILE_SIZE) + ((TILE_SIZE - PLAYER_SIZE) / 2);
    drawRect(x, y, PLAYER_SIZE, PLAYER_SIZE, COLOR_BLUE);
}

void drawInventory() {
    int start_x = SIDEBAR_START;
    int start_y = INVENTORY_START;

    for (int i = 0; i < 16; i++) {
        // Calculate grid position (assuming 4 columns)
        int col = i % 4;
        int row = i / 4;

        int x = start_x + (col * INVENTORY_BOX_SIZE);
        int y = start_y + (row * INVENTORY_BOX_SIZE);

        // Color logic
        int color = (inventory[i].type == ITEM_NONE) ? COLOR_RED : COLOR_BLUE;

        drawRect(x, y, INVENTORY_BOX_SIZE, INVENTORY_BOX_SIZE, color);
    }
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

void whiteScreen() {
    for (int y = 0; y < (int)fb.height; y++)
        for (int x = 0; x < (int)fb.width; x++)
            drawPixel(x, y, COLOR_WHITE);
}
