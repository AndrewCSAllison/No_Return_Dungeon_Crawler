#include "enemy.h"
#include "player.h"
#include "random.h"
#include "grid.h"

Enemy enemies[MAX_ENEMIES];
int enemyCount = 0;

int getEnemyAtTile(int x, int y) {
    int i;
    for (i = 0; i < enemyCount; i++) {
        if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
            return i;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// findFreeTile
// Scans the grid for a random FLOOR tile using reservoir sampling so every
// free tile has equal probability. Returns (-1,-1) if none available.
// ---------------------------------------------------------------------------
Vec2 findFreeTile(void) {
    Vec2 result;
    result.x = -1;
    result.y = -1;

    int count = 0;
    int r, c;

    for (r = 0; r < GRID_ROWS; r++) {
        for (c = 0; c < GRID_COLS; c++) {
            if (grid[r][c] == FLOOR) {
                count++;
                if ((int)rand(count) == 0) {
                    result.x = c;
                    result.y = r;
                }
            }
        }
    }

    return result;
}

// ---------------------------------------------------------------------------
// rollEnemyStats
// Distributes a stat budget across hp, atk, def, spd, lck.
// Budget = (playerBaseTotal * scale%) / 100
// Each stat gets a floor of 1 to avoid zero values.
// ---------------------------------------------------------------------------
void rollEnemyStats(Enemy *e, int playerBaseTotal, int isElite) {
    int scale  = isElite ? ELITE_STAT_SCALE : REGULAR_STAT_SCALE;
    int budget = (playerBaseTotal * scale) / 100;

    // Guarantee at least 10% of budget per stat before pool distribution
    int minPerStat = budget / 10;
    int pool = budget - (minPerStat * 5);
    if (pool < 0) pool = 0;

    int stats[5];
    int i;

    for (i = 0; i < 4; i++) {
        if (pool > 0) {
            stats[i] = (int)rand(pool / 2 + 1);
            pool -= stats[i];
        } else {
            stats[i] = 0;
        }
    }
    stats[4] = pool;

    e->maxHp = minPerStat + 1 + stats[0];
    e->hp    = e->maxHp;
    e->atk   = minPerStat + 1 + stats[1];
    e->def   = minPerStat + 1 + stats[2];
    e->spd   = minPerStat + 1 + stats[3];
    e->lck   = minPerStat + 1 + stats[4];

    if (e->maxHp > MAX_STAT) { e->maxHp = MAX_STAT; e->hp = MAX_STAT; }
    if (e->atk   > MAX_STAT)   e->atk   = MAX_STAT;
    if (e->def   > MAX_STAT)   e->def   = MAX_STAT;
    if (e->spd   > MAX_STAT)   e->spd   = MAX_STAT;
    if (e->lck   > MAX_STAT)   e->lck   = MAX_STAT;
}

// ---------------------------------------------------------------------------
// spawnEnemies
// Places up to `count` enemies on free FLOOR tiles, stamping each position
// as ENEMY so subsequent findFreeTile calls skip it naturally.
//
// Elite chance: base 5% scaling up to 30% at luck=999.
// Formula: eliteChance = 5 + (25 * playerLuck) / MAX_STAT
// ---------------------------------------------------------------------------
void spawnEnemies(int count, int playerBaseTotal, int playerLuck) {
    int eliteChance = 5 + (25 * playerLuck) / MAX_STAT;
    if (eliteChance > 30) eliteChance = 30;

    enemyCount = 0;
    int i;

    for (i = 0; i < count && enemyCount < MAX_ENEMIES; i++) {
        Vec2 pos = findFreeTile();
        if (pos.x == -1) break;

		EnemyType type = rand(5);
        Enemy *e = &enemies[enemyCount];

        e->x       = (char)pos.x;
        e->y       = (char)pos.y;
        e->isElite = ((int)rand(100) < eliteChance) ? 1 : 0;
        e->alive   = 1;
		e->type    = type;

        rollEnemyStats(e, playerBaseTotal, e->isElite);

        // Stamp tile so no other spawner lands here
        grid[pos.y][pos.x] = ENEMY;

        enemyCount++;
    }
}

// ---------------------------------------------------------------------------
// clearEnemies
// Restores all ENEMY tiles to FLOOR and resets the enemy array.
// ---------------------------------------------------------------------------
void clearEnemies(void) {
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].alive || enemies[i].hp > 0) {
            grid[(int)enemies[i].y][(int)enemies[i].x] = FLOOR;
        }
        enemies[i].x       = 0;
        enemies[i].y       = 0;
        enemies[i].maxHp   = 0;
        enemies[i].hp      = 0;
        enemies[i].atk     = 0;
        enemies[i].def     = 0;
        enemies[i].spd     = 0;
        enemies[i].lck     = 0;
        enemies[i].isElite = 0;
        enemies[i].alive   = 0;
    }
    enemyCount = 0;
}
