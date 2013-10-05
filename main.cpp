#include "Game.h"

int main() {
	Game game; 

	if (!game.init()){
		system( "pause" );
		return -1;
	}
    
	game.start();
	
    return 0;
}