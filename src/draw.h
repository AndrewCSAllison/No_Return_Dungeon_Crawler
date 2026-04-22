#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>
#include "grid.h"

#define SCREEN_WIDTH 1024
#define SCREEN_LENGTH 768
#define SIDEBAR_START 768
#define INVENTORY_START 512

typedef unsigned char guint8;
typedef unsigned int guint;

typedef struct {
    uint32_t *addr; 	// Address in memory of framebuffer for easy referencing
    uint32_t pitch;		// Number of bytes representing one full row of pixels in memory
    uint32_t width;		// Width of screen
    uint32_t height;	// Height of screen
    uint32_t bpp;		// Bits per pixel
} Framebuffer;

extern Framebuffer fb;

void initFramebuffer(uint32_t *addr, uint32_t pitch, uint32_t width, uint32_t height, uint32_t bpp);
void drawPixel(int x, int y, int color);
void drawLine(int x1, int y1, int x2, int y2, int color);
void drawRect(int x, int y, int width, int height, int color);
void drawTile(int row, int col, TileType type);
void drawPlayer(int row, int col);
void drawInventory();
void drawChar(unsigned char *font_data, char c, int x, int y, int color);
void drawString(unsigned char *font_data, const char *str, int x, int y, int color);
void drawGimpSprite(int x, int y, const guint8 *pixel_data, int width, int height);
void whiteScreen();

#endif
