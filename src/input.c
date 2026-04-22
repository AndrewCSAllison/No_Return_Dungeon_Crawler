#include "interrupt.h"
#include "player.h"

void handleInput() {
	if (last_key) {
		switch(last_key) {
            case 'w': movePlayer(0, -1); break;
            case 's': movePlayer(0,  1); break;
            case 'a': movePlayer(-1, 0); break;
            case 'd': movePlayer( 1, 0); break;
        }
        last_key = 0;
	}
}
