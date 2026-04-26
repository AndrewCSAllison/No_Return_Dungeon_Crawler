#include "interrupt.h"
#include "player.h"
#include "inventory.h"
#include "game.h"
#include "render.h"

void handleInput() {
	if (last_key) {
		switch(last_key) {
           	case 'w' :
				movePlayer(0, -1);
				setNorth();
				break;
           	case 's' :
				movePlayer(0,  1);
				setSouth();
				break;
           	case 'a' :
				movePlayer(-1, 0);
				setWest();
				break;
           	case 'd' :
				movePlayer( 1, 0);
				setEast();
				break;
			case '\t':
				switchState(STATE_INVENTORY);
				break;
			case 'e' :
				interactLadder();
				break;
   	   	}
        last_key = 0;
	}
}

void handleInventoryInput() {
    if (last_key) {
        switch(last_key) {
            case 'a': moveCursor(selectedIndex, selectedIndex - 1); break;
            case 'd': moveCursor(selectedIndex, selectedIndex + 1); break;
            case '\t': switchState(STATE_EXPLORE); break;
            case 'e': useItem();
        	case ' ': grabOrSwap(); break;
		}
        last_key = 0;
    }
}
