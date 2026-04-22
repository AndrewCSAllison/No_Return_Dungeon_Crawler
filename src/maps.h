#ifndef MAPS_H
#define MAPS_H

#include "grid.h"

#define NUM_MAPS 8

extern const TileType vault[GRID_ROWS][GRID_COLS];
extern const TileType shrine[GRID_ROWS][GRID_COLS];
extern const TileType crypt[GRID_ROWS][GRID_COLS];
extern const TileType apothecary[GRID_ROWS][GRID_COLS];
extern const TileType dining[GRID_ROWS][GRID_COLS];
extern const TileType library[GRID_ROWS][GRID_COLS];
extern const TileType hallway[GRID_ROWS][GRID_COLS];
extern const TileType maze[GRID_ROWS][GRID_COLS];

extern const TileType* mapCatalog[NUM_MAPS];

#endif
