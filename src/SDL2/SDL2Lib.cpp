#include "../../include/SDL2Lib.hpp"
#include <iostream>

/* Canonical form */

SDL2Lib::SDL2Lib() 
: AGraphicLib()
{}

SDL2Lib::~SDL2Lib() {
	if (this->window) {
		this->close();
	}
	this->libDestructor();
    if (this->dlPtr) {
        dlclose(this->dlPtr);
    }
}

SDL2Lib& SDL2Lib::operator=(const SDL2Lib& ref) {
	AGraphicLib::operator=(ref);
	return (*this);
}

SDL2Lib::SDL2Lib(const SDL2Lib& ref)
: AGraphicLib(ref)
{}

/* Classic constructor */
SDL2Lib::SDL2Lib(int width, s32 height, const std::string title, const std::string path) 
: AGraphicLib(width, height, title, path)
{}


void SDL2Lib::processEvents(int *currentLib, s32 *isRunning) {
	SDL_Event event;

    while (this->winPollEvent(this->window, &event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			// this->close();
			*isRunning = 0;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
			std::cout << "SDL Key 1 pressed, close SDL window" << std::endl;
			*currentLib = 0;
			this->close();
		} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
			std::cout << "SDL Key 2 pressed" << std::endl;
			*currentLib = 1;
		}
    }
}