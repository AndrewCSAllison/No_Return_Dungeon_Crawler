#ifndef GRID_H
#define GRID_H

#define GRID_COLS 16
#define GRID_ROWS 16
#define TILE_SIZE 48

typedef struct {
	int x;
	int y;
} Vec2;

typedef enum {
	// Standard Navigation
	FLOOR,          // 0
	WALL,           // 1
	DOOR_CLOSED,    // 2
	DOOR_OPEN,      // 3
	LADDER,         // 4

	// Damage
	FIRE,           // 5
	ACID,           // 6
	SPIKES,         // 7

	// Interactive Objects
	CHEST,          // 8
	STATUE,          // 9

	// Decorative Objects
	CAULDRON,       // 10
	BOOKSHELF,		// 11
	COFFIN,			// 12
	ALTAR,			// 13
	TABLE,			// 14
	ENEMY,
} TileType;

extern TileType grid[GRID_ROWS][GRID_COLS];

Vec2 generateDoor();
void generateLadder(int px, int py);
int loadNextRoom();

#endif
