#ifndef ENEMY_H
#define ENEMY_H

#include "grid.h"
#include "items.h"

// ---------------------------------------------------------------------------
// Tuning constants
// ---------------------------------------------------------------------------
#define MAX_ENEMIES         16
#define MAX_STAT            999

// Stat budget as a percentage of player base total
#define REGULAR_STAT_SCALE  35      // Regular enemy = 35% of player base total
#define ELITE_STAT_SCALE    70      // Elite enemy   = 70% of player base total

// Spawn count caps (dynamic, chosen at stage load)
#define MAX_ENEMY_SPAWNS    16

// Minimum gap (in tiles) between any two spawned entities
#define MIN_SPAWN_DISTANCE   0

// ---------------------------------------------------------------------------
// Enemy struct
// ---------------------------------------------------------------------------
typedef struct {
    char x;
    char y;
    int  maxHp;
    int  hp;
    int  atk;
    int  def;
    int  spd;
    int  lck;
    int  isElite;   // 0 = regular, 1 = elite
    int  alive;
} Enemy;

// ---------------------------------------------------------------------------
// Extern declarations
// ---------------------------------------------------------------------------
extern Enemy enemies[MAX_ENEMIES];
extern int   enemyCount;   // Active enemies this stage (<=MAX_ENEMY_SPAWNS)

// ---------------------------------------------------------------------------
// Function declarations
// ---------------------------------------------------------------------------

int getEnemyAtTile(int x, int y);

// Returns a Vec2 of a random FLOOR tile; (-1,-1) if none found.
Vec2 findFreeTile(void);

// Allocates stats for an enemy from a total budget across 5 stats.
// isElite selects the scale factor (REGULAR_STAT_SCALE vs ELITE_STAT_SCALE).
void rollEnemyStats(Enemy *e, int playerBaseTotal, int isElite);

// Spawns up to count enemies, using player base total for stat scaling.
// Elite chance per spawn is derived from playerLuck.
void spawnEnemies(int count, int playerBaseTotal, int playerLuck);

// Clears all enemies and resets enemyCount.
void clearEnemies(void);

#endif
