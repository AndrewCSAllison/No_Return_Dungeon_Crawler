#ifndef GAME_H
#define GAME_H

typedef enum {
    STATE_START,
    STATE_EXPLORE,
    STATE_INVENTORY,
    STATE_DEATH,
} GameState;

extern GameState currentState;

void switchState(GameState newState);
void interactLadder();

#endif
