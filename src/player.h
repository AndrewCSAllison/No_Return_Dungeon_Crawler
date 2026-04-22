#ifndef PLAYER_H
#define PLAYER_H

#include "items.h"

#define PLAYER_SIZE 32

typedef struct {
    int x;
    int y;
    int hp;
    int atk;
    int def;
    int spd;
    int luck;
    int charm_points;
    Item equipment[5];
    Item inventory[10];
} Player;

// Global instance defined in player.c
extern Player player;

// Function prototypes
void initPlayer(int startX, int startY);
void movePlayer(int dx, int dy);

#endif
