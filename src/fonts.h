#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#define FONT_TEXT_WIDTH  16
#define FONT_TEXT_HEIGHT 16
#define FONT_HEAD_WIDTH  32
#define FONT_HEAD_HEIGHT 32

/**
 * PSF2 Header Structure
 */
typedef struct {
    uint32_t magic;         // 0x864ab572 (Check this to ensure font is valid)
    uint32_t version;       // Usually 0
    uint32_t header_size;   // Offset where glyph data starts (usually 32)
    uint32_t flags;         // 0 if no unicode table
    uint32_t length;        // Number of glyphs (usually 256)
    uint32_t char_size;     // Bytes per character (40 for 10x20, 64 for 16x32)
    uint32_t height;        // Height in pixels
    uint32_t width;         // Width in pixels
} psf2_header_t;

extern unsigned char ter_u20n_psf[];      // 10x20 font
extern unsigned int ter_u20n_psf_len;

extern unsigned char ter_u24n_psf[];      // 12x24 font
extern unsigned int ter_u24n_psf_len;

extern unsigned char ter_u14n_psf[];
extern unsigned int ter_u14n_psf_len;

extern unsigned char* FontLog;
extern unsigned char* FontStandard;
extern unsigned char* FontHeader;

void initFonts();

#endif
