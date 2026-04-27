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
				dealDamageTiles();
				renderStats();
				break;
           	case 's' :
				movePlayer(0,  1);
				setSouth();
				dealDamageTiles();
                renderStats();
				break;
           	case 'a' :
				movePlayer(-1, 0);
				setWest();
				dealDamageTiles();
                renderStats();
				break;
           	case 'd' :
				movePlayer( 1, 0);
				setEast();
				dealDamageTiles();
                renderStats();
				break;
			case '\t':
				switchState(STATE_INVENTORY);
				break;
			case ' ' :
				interactLadder();
				break;
			case 'e':
				attackEnemy();
				lootChest();
				renderPlayer();
				renderEnemies();
				renderStats();
				renderInventory();
   	   			break;
		}
		renderConsole();
        last_key = 0;
	}
}

void handleInventoryInput() {
    if (last_key) {
        switch(last_key) {
            case 'a' : moveCursor(selectedIndex, selectedIndex - 1); break;
            case 'd' : moveCursor(selectedIndex, selectedIndex + 1); break;
            case '\t': switchState(STATE_EXPLORE); break;
            case 'e' : useItem(); break;
        	case ' ' : grabOrSwap(); break;
			case '\b': tossItem(selectedIndex); break;
		}
        last_key = 0;
    }
}
