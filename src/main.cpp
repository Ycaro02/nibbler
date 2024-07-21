#include "../include/Nibbler.hpp"
#include "../include/SFLib.hpp"
#include "../include/SDL2Lib.hpp"
#include <iostream>

void GameLoop(Nibbler &ctx) {
	// AGraphicLib *currentLib = ctx.libs[ctx.currentLib];
	AGraphicLib *currentLib = NULL;
	
	while (ctx.getIsRunning()) {
		currentLib = ctx.getCurrentLib();
		if (!currentLib->isOpen()) {
			if (!currentLib->windowCreate()) {
				std::cerr << "Failed to create window" << std::endl;
				return ;
			}
		}
		currentLib->clear();

		for (s32 i = 0; i < ctx.getHeight(); i++) {
			for (s32 j = 0; j < ctx.getWidth(); j++) {
				currentLib->colorTile(j, i, 0,0,0, 255);
			}
		}


		currentLib->display();
		currentLib->processEvents(ctx);
	}
}

int main() {
	Nibbler ctx = Nibbler(10, 10);

	ctx.DisplayBoardFD0();
	GameLoop(ctx);
	return (0);
}

